#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "message.hpp"

class TcpServer{
    int listening_port = -1;
    int client_port = -1; //vector for multiple clients
    struct addrinfo hints{}; //using {} here makes all values of hints default = 0 
    struct addrinfo *res=nullptr;

public:

    TcpServer(const std::string& port){
        //memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        /*
            int getaddrinfo(const char *node,        // e.g. "www.example.com" or IP
            const char *service,                     // e.g. "http" or port number
            const struct addrinfo *hints,
            struct addrinfo **res);

            here we have port in std::string so to convert from std::string to c string we use c_str()
            // address_info_status becomes 0 is getaddrinfo runs
        */

        int address_info_status = getaddrinfo(nullptr, port.c_str(), &hints, &res);

        if (address_info_status != 0) {
            throw std::runtime_error("getaddrinfo failed");
        }


        /*
            int socket(int __domain(ai_family), int __type(ai_socktype), int __protocol(ai_protocol))
            returns a non negative integer, if it is -1(set as default) then there is error
        */
        
        listening_port = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if(listening_port == -1){
            freeaddrinfo(res); //we free res because it has failed
            res = nullptr;
            throw std::runtime_error("socket failed");
        }

        /*
            int setsockopt(
                            int client_port[0],          // the socket file descriptor you already created
                            int level,           // what category of option we're changing
                            int optname,         // the specific option you want to set
                            const void *optval,  // pointer to the option value
                            socklen_t optlen     // size of that value in bytes
                        );

        */

        int yes = 1;
        int set_socket_options_err = setsockopt(listening_port, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
        if(set_socket_options_err == -1){
            freeaddrinfo(res);
            res = nullptr;
            throw std::runtime_error("socket option failed");
        }

        // int bind(int __fd, const sockaddr *__addr, socklen_t __len)
        int bind_status = bind(listening_port, res->ai_addr, res->ai_addrlen);
        if(bind_status == -1){
            freeaddrinfo(res);
            res = nullptr;
            throw std::runtime_error("bind failed");
        }

        int listening_status = listen(listening_port, 10);
        if(listening_status == -1){
            freeaddrinfo(res);
            res = nullptr;
            throw std::runtime_error("listening failed");
        }

        std::cout << "Server is listening on port " << port << "...\n";

    }

    void acceptClient() {
        struct sockaddr_storage client_addr;
        socklen_t addr_size = sizeof(client_addr);
        client_port = accept(listening_port, (struct sockaddr*)&client_addr, &addr_size);
        if (client_port == -1) {
            throw std::runtime_error("accept failed");
        }
        std::cout << "Client connected!\n";
    }

    // Diffie hellman key exchange
    // exchange_key(){}
    
    void sendData(const std::string& text) {
    Message msg;
    msg.encode(text);
    const auto& buffer = msg.getData();
    size_t totalSent = 0;
    size_t toSend = buffer.size();

    while (totalSent < toSend) {
        ssize_t sent = send(client_port, buffer.data() + totalSent, toSend - totalSent, 0);
        if (sent <= 0) {
            // error or connection closed
            std::cerr << "Send failed!\n";
            return;
        }
        totalSent += sent;
    }
}


    std::string receiveData() {
    char headerBuf[4];
    int received = 0;
    // First, receive exactly 4 bytes of header
    while (received < 4) {
        int n = recv(client_port, headerBuf + received, 4 - received, 0);
        if (n <= 0) return ""; // connection closed or error
        received += n;
    }
    // Convert header to integer
    int headerValue = (headerBuf[0] - '0') * 1000 +
                      (headerBuf[1] - '0') * 100  +
                      (headerBuf[2] - '0') * 10   +
                      (headerBuf[3] - '0');
    // Now receive exactly 'headerValue' bytes of payload
    std::string payload;
    payload.resize(headerValue);
    received = 0;
    while (received < headerValue) {
        int n = recv(client_port, &payload[received], headerValue - received, 0);
        if (n <= 0) return ""; // connection closed or error
        received += n;
    }

    return payload;
    }

    
    ~TcpServer() {
        // for (int fd : client_port) {
        //     if (fd != -1) close(fd);
        // }
        // client_port.clear();

        if (client_port != -1){
            close(client_port);
            client_port = -1;
        }

        if (listening_port != -1) {
            close(listening_port);
            listening_port = -1;
        }

        if (res) {
            freeaddrinfo(res);
            res = nullptr;
        }
        std::cout << "Server shutdown.\n";
    }

    void shutdown() {
    if (client_port != -1) {
        close(client_port);
        client_port = -1;
    }
    if (listening_port != -1) {
        close(listening_port);
        listening_port = -1;
    }
    if (res) {
        freeaddrinfo(res);
        res = nullptr;
    }
}


};

#endif