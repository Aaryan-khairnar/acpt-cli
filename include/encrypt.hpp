#ifndef ENCRYPT_HPP
#define ENCRYPT_HPP

#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <math.h>

class Encrypt{
long long p = 0;
long long q = 0;

public:

int checkprime(long int pr){
    int i;
    long int j = sqrt(pr);
    for (i = 2; i <= j; i++){
        if (pr % i == 0) return 0;
    }
    return 1;
}



};


#endif