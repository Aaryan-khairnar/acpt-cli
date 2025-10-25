#ifndef ENCRYPT_HPP
#define ENCRYPT_HPP

#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <random>

class rsa {
public:
  uint64_t p = 0;
  uint64_t q = 0;
  unsigned __int128 n = 0;
  unsigned __int128 phi = 0;
  unsigned __int128 d = 0;
  unsigned int e = 65537;
  
  uint64_t random_uint64() {
    std::random_device rd; // hardware entropy
    std::mt19937_64 gen(rd()); // 64-bit Mersenne Twister
    std::uniform_int_distribution<uint64_t> dis(
        (1ULL << 63),  // min ~ 2^63 to make it “big”
        UINT64_MAX // max
    );
    return dis(gen);
  }

  bool checkprime(long int pr){
    int i;
    long int j = sqrt(pr);
    for (i = 2; i <= j; i++){
        if (pr % i == 0) return 0;
    }
    return 1;
  }

  uint64_t generate_prime() {
    while (true) {
        uint64_t x = random_uint64() | 1; // make it odd
        if (checkprime(x)) return x;
    }
  }

  unsigned __int128 multiply(uint64_t p, uint64_t q){
    return (unsigned __int128)p * (unsigned __int128)q;
  }

  void print_uint128(unsigned __int128 n) {
    if (n == 0) { std::cout << 0; return; }
    std::string s;
    while(n > 0){
        s = char('0' + n % 10) + s;
        n /= 10;
    }
    std::cout << s;
  }

  unsigned __int128 gcd(unsigned __int128 a, unsigned __int128 b) {
    while (b != 0) {
        unsigned __int128 temp = b;
        b = a % b;
        a = temp;
    }
    return a;
  }

  unsigned __int128 mod_mul(unsigned __int128 a, unsigned __int128 b, unsigned __int128 mod) {
    return (a * b) % mod;
  }
  
  unsigned __int128 mod_exp(unsigned __int128 base, unsigned __int128 exp, unsigned __int128 mod) {
    unsigned __int128 result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp /= 2;
    }
    return result;
  }

  unsigned __int128 mod_inverse(unsigned __int128 e, unsigned __int128 phi) {
    __int128 t = 0, newt = 1;
    __int128 r = phi, newr = e;
    while (newr != 0) {
        __int128 quotient = r / newr;

        __int128 temp = newt;
        newt = t - quotient * newt;
        t = temp;

        temp = newr;
        newr = r - quotient * newr;
        r = temp;
    }
    if (r > 1) throw std::runtime_error("e is not invertible");
    if (t < 0) t += phi;
    return (unsigned __int128)t;
  }

  unsigned __int128 encrypt(unsigned __int128 msg){
    return mod_exp(msg, e, n);
  }

  unsigned __int128 decrypt(unsigned __int128 c){
    return mod_exp(c, d, n);
  }

};

class diffiehellman {
public:
  unsigned __int128 p = 0;
  unsigned __int128 g = 0;
  unsigned __int128 private_key = 0;
  unsigned __int128 public_key = 0;

  unsigned __int128 mod_exp(unsigned __int128 base, unsigned __int128 exp, unsigned __int128 mod){
    unsigned __int128 result = 1;
    base = base % mod;
    while(exp > 0){
        if(exp % 2 == 1)
        result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

  uint64_t random_uint64() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis((1ULL << 32), (1ULL << 63) - 1);
    return dis(gen);
  }

  void generate_private_key() {
    private_key = ((unsigned __int128)random_uint64() << 64) | random_uint64();
  }

  void compute_public_key() {
    public_key = mod_exp(g, private_key, p);
  }

  unsigned __int128 compute_shared(unsigned __int128 other_public) {
    return mod_exp(other_public, private_key, p);
  }

  void print_uint128(unsigned __int128 num){
    if(num==0){ std::cout << 0; return; }
    std::string s;
    while(num>0){
      s = char('0'+num%10) + s;
      num /= 10;
    }
    std::cout << s;
  }
};

#endif
