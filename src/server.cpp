#include "server.hpp"
#include <iostream>
#include <csignal>
#include <unistd.h>
#include <atomic>

std::atomic<bool> exitFlag(false);

void handleSigint(int) {
    exitFlag = true;
}

int main() {
    signal(SIGINT, handleSigint);

    try {
        TcpServer server("53490");
        server.acceptClient();
        std::cout << "Client connected. Ready to chat!\n";

        while (!exitFlag) {
            // Server waits for client message
            std::string msg = server.receiveData();
            if(msg.empty()) {
                std::cout << "Client disconnected.\n";
                break;
            }
            std::cout << "Client: " << msg << "\n";

            // Server sends a reply
            std::string reply;
            std::cout << "You: ";
            std::getline(std::cin, reply);
            server.sendData(reply);
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    std::cout << "Server exited cleanly.\n";
}
