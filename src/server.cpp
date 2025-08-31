#include "server.hpp"
#include <iostream>
#include <csignal>
#include <unistd.h>

TcpServer* serverPtr = nullptr; // global pointer to access the server in the signal handler

void handleSigint(int signum) {
    std::cout << "\nCaught Ctrl+C, shutting down server...\n";
    if(serverPtr) delete serverPtr; // destructor will close sockets
    exit(signum);
}

int main() {
    signal(SIGINT, handleSigint); // register Ctrl+C handler

    try {
        TcpServer server("53490");
        serverPtr = &server;

        server.acceptClient();
        std::cout << "Client connected. Ready to chat!\n";

        while (true) {
            std::string msg = server.receiveData();
            if(msg.empty()) {
                std::cout << "Client disconnected.\n";
                break;
            }
            std::cout << "Client: " << msg << "\n";

            std::string reply;
            std::cout << "Enter reply: ";
            std::getline(std::cin, reply);

            server.sendData(reply);
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    std::cout << "Server exited cleanly.\n";
}


// int main() {
//     try {
//         TcpServer server("53490");
//         server.acceptClient();
//         while (true) {
//             std::string msg = server.receiveData();
//         if(msg.empty()) {
//             std::cout << "Client disconnected\n";
//             break;
//         }
//         std::cout << "Client: " << msg << "\n";
//         server.sendData("Hello from server!");
// }

//     }
//     catch (const std::exception& e) {
//         std::cerr << e.what() << "\n";
//     }
// }
