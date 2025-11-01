#include "../include/client.hpp"
#include "../include/encrypt.hpp"
#include <atomic>
#include <csignal>
#include <iostream>

std::atomic<bool> exitFlag(false);

void handleSigint(int) { exitFlag = true; }

// Simple XOR encryption using 128-bit key
std::string xor_encrypt(const std::string &msg, unsigned __int128 key) {
  std::string out = msg;
  for (size_t i = 0; i < msg.size(); i++) {
    out[i] = msg[i] ^ ((key >> (8 * (i % 16))) & 0xFF);
  }
  return out;
}

int main() {
  signal(SIGINT, handleSigint);

  try {
    std::cout << "Enter server IP address: ";
    std::string serverIp;
    std::cin >> serverIp;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    TcpClient client(serverIp, "53490");
    std::cout << "Connected to server. Ready to chat!\n";

    // --- Diffie-Hellman key exchange ---
    diffiehellman dh_client;
    dh_client.p = 0xffffffffffffffffULL; // 64-bit prime // same prime
    dh_client.g = 5;

    dh_client.generate_private_key();
    dh_client.compute_public_key();

    // Receive server public key
    std::string serverPubStr = client.receiveData();
    unsigned __int128 serverPubKey = std::stoull(serverPubStr);

    // Send client public key
    std::string pubKeyStr;
    dh_client.print_uint128(dh_client.public_key);
    pubKeyStr = std::to_string((uint64_t)dh_client.public_key); // simplified
    client.sendData(pubKeyStr);

    unsigned __int128 shared_key = dh_client.compute_shared(serverPubKey);
    std::cout << "Shared key established.\n";

    while (!exitFlag) {
      // send message
      std::string message;
      std::cout << "You: ";
      std::getline(std::cin, message);
      std::string enc_msg = xor_encrypt(message, shared_key);
      client.sendData(enc_msg);

      // receive reply
      std::string enc_reply = client.receiveData();
      if (enc_reply.empty()) {
        std::cout << "Server disconnected.\n";
        break;
      }
      std::string reply = xor_encrypt(enc_reply, shared_key);
      std::cout << "Server: " << reply << "\n";
    }

  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
  }

  std::cout << "Client exited cleanly.\n";
}
