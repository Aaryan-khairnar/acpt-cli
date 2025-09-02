#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

/* 
header is of 4 bytes and maxBytes can be stored as 512 bytes
header stores the body length that is the current body length
data stores the header+bodyLength with maximum size of header+maxBytes

client attempts to send message:- It will encode header and put message into the data and send data
server gets the message, decodes the header, get the bodylength from the header and hence complete body
then server sends the message to all the clients connected to that room.

data array:
[0][1][2][3][4][5][6][7][8]...-
 H  H  H  H  M  M  M  M  M     (H = Header, M = Message)
 ↑           ↑
 Header      Message starts here (position 4)
 positions   
 0-3
*/

class Message{
std::vector <char> data;

public:
void encodeHeader(size_t length){
    data.clear();
    data.resize(4); 
    data[0] = '0' + (length / 1000) % 10;  // Thousands place
    data[1] = '0' + (length / 100) % 10;   // Hundreds place  
    data[2] = '0' + (length / 10) % 10;    // Tens place
    data[3] = '0' + (length / 1) % 10;     // Ones place
}

void encode(std::string message){
    encodeHeader(message.size());
    for(int i=0; i< message.size(); i++){
        data.emplace_back(message[i]);
    }
}

const std::vector<char>& getData() const {
    return data;
}

void sendData(const std::string& text, int& port) {
    encode(text);
    const auto& buffer = getData();
    size_t totalSent = 0;
    size_t toSend = buffer.size();
    while (totalSent < toSend) {
        ssize_t sent = send(port, buffer.data() + totalSent, toSend - totalSent, 0);
    if (sent <= 0) {
        // error or connection closed
        std::cerr << "Send failed!\n";
        return;
        }
        totalSent += sent;
    }
}

std::string receiveData(int& port){
    char headerBuf[8];
    int received = 0;
    // First, receive exactly 4 bytes of header
    while (received < 8) {
        int n = recv(port, headerBuf + received, 8 - received, 0);
        if (n <= 0) return ""; // connection closed or error
        received += n;
    }
    // Convert header to integer // CHANGE TO 8
    int headerValue = (headerBuf[0] - '0') * 1000 +
                  (headerBuf[1] - '0') * 100  +
                  (headerBuf[2] - '0') * 10   +
                  (headerBuf[3] - '0');
    // Now receive exactly 'headerValue' bytes of payload
    std::string payload;
    payload.resize(headerValue);
    received = 0;
    while (received < headerValue) {
        int n = recv(port, &payload[received], headerValue - received, 0);
        if (n <= 0) return ""; // connection closed or error
        received += n;
    }
        //decrypted_data = m.decrypt(payload)
        //return decrypted_data
        return payload;
}

};

#endif