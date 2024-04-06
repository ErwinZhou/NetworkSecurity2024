#include <cstdint>
#include <utility>
#include <iostream>
using namespace std;
class RSAUtils
{
public:
    RSAUtils();
    ~RSAUtils();

private:
    // uint64_t gcd(uint64_t a, uint64_t b);
    // pair<uint64_t, uint64_t> extendedEuclidean(uint64_t a, uint64_t b);
    // uint64_t modInverse(uint64_t a, uint64_t m);
    // uint64_t generatePrime();
    // uint64_t generateE(uint64_t phi);
    // uint64_t generateD(uint64_t e, uint64_t phi);
    uint64_t mulMod(uint64_t a, uint64_t b, uint64_t n);
    uint64_t powMod(uint64_t base, uint64_t pow, uint64_t n);

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