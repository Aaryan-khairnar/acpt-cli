#include "client.hpp"
#include <iostream>
#include <csignal>
#include <limits>
#include <string>
#include <atomic>


std::atomic<bool> exitFlag(false);

void handleSigint(int) {
    exitFlag = true;
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

        while (!exitFlag) {
            // Client sends first
            std::string message;
            std::cout << "You: ";
            std::getline(std::cin, message);
            client.sendData(message);

            // Client waits for reply
            std::string reply = client.receiveData();
            if(reply.empty()) {
                std::cout << "Server disconnected.\n";
                break;
            }
            std::cout << "Server: " << reply << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    std::cout << "Client exited cleanly.\n";
}
