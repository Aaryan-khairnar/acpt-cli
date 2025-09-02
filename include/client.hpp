#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <stdexcept>
#include "message.hpp"

class TcpClient{
    int connection_port = -1;
    struct addrinfo hints{}; //using {} here makes all values of hints default = 0 
    struct addrinfo *res=nullptr;
    Message m;

public:

TcpClient(const std::string& host, const std::string& port){
    hints.ai_family = AF_UNSPEC;     // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    int address_info_status = getaddrinfo(host.c_str(), port.c_str(), &hints, &res);
    if (address_info_status != 0) {
        throw std::runtime_error("getaddrinfo failed");
        return;
    }
    connection_port = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(connection_port == -1){
        freeaddrinfo(res); //we free res because it has failed
        res = nullptr;
        throw std::runtime_error("socket failed");
        return;
    }
    int connection_status = connect(connection_port, res->ai_addr, res->ai_addrlen);
    if (connection_status == -1) {
        freeaddrinfo(res);
        close(connection_port);
        connection_port = -1;
        throw std::runtime_error("connection failed");
    }
    std::cout << "Connected successfully!\n";
    }

    void sendData(const std::string& text){
        m.sendData(text, connection_port);
    }
    
    std::string receiveData(){
        return m.receiveData(connection_port);
    }
 
~TcpClient() {
    freeaddrinfo(res);
    if (connection_port != -1) {
        close(connection_port);
    }
    std::cout << "Connection closed.\n";
}

};

#endif 