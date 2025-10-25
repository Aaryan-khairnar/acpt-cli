#include "server.hpp"
#include "message.hpp"
#include "encrypt.hpp"
#include <iostream>
#include <csignal>
#include <atomic>

std::atomic<bool> exitFlag(false);

void handleSigint(int) {
    exitFlag = true;
}

// Simple XOR encryption using 128-bit key
std::string xor_encrypt(const std::string& msg, unsigned __int128 key) {
    std::string out = msg;
    for(size_t i=0; i<msg.size(); i++) {
        out[i] = msg[i] ^ ((key >> (8*(i%16))) & 0xFF);
    }
    return out;
}

int main() {
    signal(SIGINT, handleSigint);

    try {
        TcpServer server("53490");
        server.acceptClient();
        std::cout << "Client connected. Ready to chat!\n";

        // --- Diffie-Hellman key exchange ---
        diffiehellman dh_server, dh_client_placeholder;
        dh_server.p = 0xffffffffffffffffULL; // example 64-bit prime
        dh_server.g = 5;

        dh_server.generate_private_key();
        dh_server.compute_public_key();

        // Send server public key to client
        Message m;
        std::string pubKeyStr;
        dh_server.print_uint128(dh_server.public_key); pubKeyStr = "";
        dh_server.print_uint128(dh_server.public_key);
        pubKeyStr = std::to_string((uint64_t)dh_server.public_key); // simplified
        m.encode(pubKeyStr);
        server.sendData(pubKeyStr);

        // Receive client public key
        std::string clientPubStr = server.receiveData();
        unsigned __int128 clientPubKey = std::stoull(clientPubStr);
        unsigned __int128 shared_key = dh_server.compute_shared(clientPubKey);
        std::cout << "Shared key established.\n";

        while(!exitFlag) {
            std::string enc_msg = server.receiveData();
            if(enc_msg.empty()) {
                std::cout << "Client disconnected.\n";
                break;
            }

            // decrypt
            std::string msg = xor_encrypt(enc_msg, shared_key);
            std::cout << "Client: " << msg << "\n";

            // send reply
            std::string reply;
            std::cout << "You: ";
            std::getline(std::cin, reply);
            std::string enc_reply = xor_encrypt(reply, shared_key);
            server.sendData(enc_reply);
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    std::cout << "Server exited cleanly.\n";
}
