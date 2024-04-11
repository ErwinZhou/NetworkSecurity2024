#include <cstdint>
#include <utility>
#include <iostream>
#include <random>
using namespace std;
#ifndef SUCCESS
#define SUCCESS 1
#endif

#ifndef FAILURE
#define FAILURE 0
#endif
class RSAUtils
{
public:
    RSAUtils(){};
    ~RSAUtils(){};
    int init(int rounds = 100, bool defaultKey = true, bool highSecurity = false);
    pair<uint64_t, uint64_t> getPublicKey();
    pair<uint64_t, uint64_t> getPrivateKey();
    void setPublicKey(uint64_t e, uint64_t n);
    void setPrivateKey(uint64_t d, uint64_t n);
    void encrypt(uint64_t &plainText, uint64_t &cipherText);
    void decrypt(uint64_t &cipherText, uint64_t &plainText);

private:
    // enum for prime test approach
    enum
    {
        MILLER_RABIN = 0,
        EUCLIDEAN = 1,
        FERMAT = 2
    };
    // Basic APIs:Modular arithmetic and so on
    uint64_t mulMod(uint64_t a, uint64_t b, uint64_t n);
    uint64_t powMod(uint64_t base, uint64_t pow, uint64_t n);
    uint64_t EulerTotientFunction(uint64_t n);
    // Prime test algorithms
    bool MillerRabin(uint64_t n);
    /* Continuing to implement Fermat and Euclidan for Prime Test */
    bool primeTest(uint64_t n, int approach, int rounds = 100);
    // Generate random prime number
    uint64_t generateRamdomPrime(int bits, int rounds = 100);
    // Greatest common divisor
    uint64_t gcd(uint64_t &p, uint64_t &q);
    int generatePublicKey(uint64_t n, int rounds = 100);
    int generatePrivateKey(uint64_t n);
    int generateKeyPair(uint64_t n, int rounds = 100);

    /**
     * Advanced Standard for better security
     */
    // Find Prime with large factor for better security
    int findPrimeWithLargeFactor(int bits, int rounds);
    /**
     * Prime number p & q
     * n = p * q
     * phi = (p - 1) * (q - 1)
     * e: 1 < e < phi, gcd(e, phi) = 1
     * d: e * d mod phi = 1
     * Note: Although NIST and ANSI X9 has acquired the RSA key length of 1024 bits at least,
     *       this implementation is for fun only, so the key length is 64 bits stroing in uint64_t.
     */
    uint64_t p;
    uint64_t q;
    uint64_t n;
    uint64_t phi_n;
    uint64_t e;
    uint64_t d;
    /**
     * Public and Private Key
     * publicKey: (e, n)
     * privateKey: (d, n)
     */
    pair<uint64_t, uint64_t> publicKey;
    pair<uint64_t, uint64_t> privateKey;
};