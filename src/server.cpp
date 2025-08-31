#include <iostream>
#include "server.hpp"

int main() {
    try {
        TcpServer server("53490");
        server.acceptClient();
        while (true) {
            std::string msg = server.receiveData();
        if(msg.empty()) {
            std::cout << "Client disconnected\n";
            break;
        }
        std::cout << "Client: " << msg << "\n";
        server.sendData("Hello from server!");
}

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}