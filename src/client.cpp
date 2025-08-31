#include <iostream>
#include "client.hpp"

int main() {
    try {
        std::cout<< "Enter server ip address: ";
        std::string ipa;
        std::cin >> ipa;
        TcpClient client(ipa, "53490");
        client.sendData("helloworld");
        std::string reply = client.receiveData();
        if(!reply.empty()) {
            std::cout << "Server: " << reply << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}

