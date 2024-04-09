#include <cstdint>
#include <utility>
#include <iostream>
#include <random>
using namespace std;
class RSAUtils
{
public:
    RSAUtils(){};
    ~RSAUtils(){};
    pair<uint64_t, uint64_t> getPublicKey();
    pair<uint64_t, uint64_t> getPrivateKey();

private:
    // enum for prime test approach
    enum
    {
        MILLER_RABIN = 0,
        EUCLIDEAN = 1,
        FERMAT = 2
    };
    // enum for results for some operations
    enum
    {
        FAILURE = 0,
        SUCCESS = 1
    };
    // Basic APIs:Modular arithmetic and so on
    uint64_t
    mulMod(uint64_t a, uint64_t b, uint64_t n);
    uint64_t powMod(uint64_t base, uint64_t pow, uint64_t n);
    uint64_t EulerTotientFunction(uint64_t n);
    // Prime test algorithms
    bool MillerRabin(uint64_t n);
    /* Continuing to implement Fermat and Euclidan for Prime Test */
    bool primeTest(uint64_t n, int approach, int rounds);
    // Generate random prime number
    uint64_t generateRamdomPrime(char bits, int rounds);
    // Greatest common divisor
    uint64_t gcd(uint64_t &p, uint64_t &q);
    int generatePublicKey(uint64_t n, int rounds);
    int generatePrivateKey(uint64_t n);
    void generateKeyPair(uint64_t n);
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
    uint64_t phi;
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