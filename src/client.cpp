#include "client.hpp"
#include <iostream>
#include <csignal>
#include <limits>
#include <string>
#include <atomic>

std::atomic<bool> exitFlag(false);

void handleSigint(int) {
    exitFlag = true; // just signal the main loop
}

int main() {
    signal(SIGINT, handleSigint);

    try {
        std::cout << "Enter server IP address: ";
        std::string serverIp;
        std::cin >> serverIp;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        TcpClient client(serverIp, "53490");

        std::cout << "Connected to server. Start chatting!\n";

        // while (exitFlag) {
        //     std::string message;
        //     std::cout << "You: ";
        //     std::getline(std::cin, message);

        //     client.sendData(message);

        //     std::string reply = client.receiveData();
        //     if(reply.empty()) {
        //         std::cout << "Server disconnected.\n";
        //         break;
        //     }
        //     std::cout << "Server: " << reply << "\n";
        // }

        while (!exitFlag) {
            std::string message;
            std::getline(std::cin, message);
            client.sendData(message);

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

// int main() {
//     try {
//         std::cout<< "Enter server ip address: ";
//         std::string ipa;
//         std::cin >> ipa;
//         TcpClient client(ipa, "53490");
//         client.sendData("helloworld");
//         std::string reply = client.receiveData();
//         if(!reply.empty()) {
//             std::cout << "Server: " << reply << "\n";
//         }
//     } catch (const std::exception& e) {
//         std::cerr << e.what() << "\n";
//     }
// }
