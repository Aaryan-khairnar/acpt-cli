#include <iostream>
#include "client.hpp"

int main() {
    try {
        std::cout<< "Enter server ip address: ";
        std::string ipa;
        std::cin >> ipa;
        TcpClient client(ipa, "53490");
        client.receiveData();   // wait first
        client.sendData("helloworld");
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}

