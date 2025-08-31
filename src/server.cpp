#include <iostream>
#include "server.hpp"

int main() {
    try {
        TcpServer server("53490");
        server.acceptClient();
        server.sendData("Hello from server!");   // send first
        server.receiveData();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}