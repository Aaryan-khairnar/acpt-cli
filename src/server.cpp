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


// std::atomic<bool> exitFlag(false);

// void handleSigint(int) {
//     exitFlag = true; // just signal the main loop
// }

// int main() {
//     signal(SIGINT, handleSigint); // register Ctrl+C handler

//     try {
//         TcpServer server("53490");

//         server.acceptClient();
//         std::cout << "Client connected. Ready to chat!\n";

//         // while (!exitFlag) {
//         //     std::string msg = server.receiveData();
//         //     if(msg.empty()) {
//         //         std::cout << "Client disconnected.\n";
//         //         break;
//         //     }
//         //     std::cout << "Client: " << msg << "\n";

//         //     std::string reply;
//         //     std::cout << "Enter reply: ";
//         //     std::getline(std::cin, reply);

//         //     server.sendData(reply);
//         // }

//         while (!exitFlag) {
//             std::string msg = server.receiveData();
//             if(msg.empty()) {
//                 std::cout << "Client disconnected.\n";
//                 break;
//             }
//             std::cout << "Client: " << msg << "\n";

//         // automatically reply
//             server.sendData("Message received!");
//         }


//     } catch (const std::exception& e) {
//         std::cerr << e.what() << "\n";
//     }

//     std::cout << "Server exited cleanly.\n";
// }


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
