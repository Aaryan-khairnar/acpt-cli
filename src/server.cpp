#include <iostream>
#include "server.hpp"

int main() {
    try {
        TcpServer server("53490");
        server.acceptClient();
        server.receiveData();
        server.sendData("Hello from server!");
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}