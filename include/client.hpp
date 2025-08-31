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

        // Diffie hellman key exchange
        // exchange_key(){}
        }

        void sendData(const std::string& text) {
            Message msg;
            msg.encode(text);
            const auto& buffer = msg.getData();

            size_t totalSent = 0;
            size_t toSend = buffer.size();

            while (totalSent < toSend) {
                ssize_t sent = send(connection_port, buffer.data() + totalSent, toSend - totalSent, 0);
            if (sent <= 0) {
                // error or connection closed
                std::cerr << "Send failed!\n";
                return;
                }
                totalSent += sent;
            }
        }


        std::string receiveData(){
        char headerBuf[4];
        int received = 0;
        // First, receive exactly 4 bytes of header
        while (received < 4) {
            int n = recv(connection_port, headerBuf + received, 4 - received, 0);
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
            int n = recv(connection_port, &payload[received], headerValue - received, 0);
            if (n <= 0) return ""; // connection closed or error
            received += n;
        }
            //decrypted_data = m.decrypt(payload)
            //return decrypted_data

            return payload;
        }
     
    ~TcpClient() {
        freeaddrinfo(res);
        if (connection_port != -1) {
            close(connection_port);
        }
        std::cout << "Connection closed.\n";
    }

    void shutdown() {
    if (connection_port != -1) {
            close(connection_port);
        }
    if (res) {
        freeaddrinfo(res);
        res = nullptr;
    }
};

#endif 