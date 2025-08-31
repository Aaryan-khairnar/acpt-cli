#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstdlib>
#include <cstdio>
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

    //      implement diffie hellman
    //      implement RSA encryption

};

#endif