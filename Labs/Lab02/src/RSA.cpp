#include "RSA.h"

uint64_t RSAUtils::mulMod(uint64_t a, uint64_t b, uint64_t n)
{
    /**
     * Calculate (a * b)(mod n) in log(b), while the same time avoiding overflow.
     * By following:
     * (1) (a * b)(mod n) = ((a mod n) * (b mod n))(mod n)
     * (2) Multiplication--->Shifting and Adding
     * @param a: var1
     * @param b: var2
     * @param n: mod
     * @return res: (a * b)(mod n)
     */
    uint64_t res = 0;
    while (b > 0)
    {
        if (b & 1)
            res = (res + a) % n;
        if (a > n / 2)
            a = (a << 1) % n;
        else
            a = a << 1;
        b >>= 1;
    }
    return res % n;
}
uint64_t RSAUtils::powMod(uint64_t base, uint64_t pow, uint64_t n)
{
    /**
     * Calculate (base ^ pow)(mod n) in log(pow), while the same time avoiding overflow.
     * @param base: base
     * @param pow: exponent
     * @param n: mod
     * @return res: (base ^ pow)(mod n)
     */
    uint64_t res = 1;
    while (pow > 0)
    {
        if (pow & 1)
            // To avoid overflow, we use mulMod instead of res = res * base % n;
            res = mulMod(res, base, n);
        base = mulMod(base, base, n);
        pow >>= 1;
    }
    return res;
}
uint64_t RSAUtils::EulerTotientFunction(uint64_t n)
{
    /**
     * Calculate Euler Totient Function of n.
     * @param n: number
     * @return phi: Euler Totient Function of n
     */
    uint64_t phi = 1;
    // 1~(n-1) are coprime with n
    for (uint64_t i = 2; i < n; i++)
    {
        int64_t x, y;
        if (extendedGCD(i, n, x, y) == 1)
            phi++;
    }
    return phi;
}
uint64_t RSAUtils::extendedGCD(uint64_t a, uint64_t b, int64_t &x, int64_t &y)
{
    /**
     * Calculate the greatest common divisor of a and b using Extended Euclidean Algorithm.
     * @param a: var1
     * @param b: var2
     * @param x: a^-1(mod b)
     * @param y: b^-1(mod a)
     * @return gcd: greatest common divisor of a and b
     */
    if (b == 0)
    {
        x = 1;
        y = 0;
        return a;
    }
    uint64_t gcd = extendedGCD(b, a % b, x, y);
    int64_t temp = x;
    x = y;
    y = temp - static_cast<int64_t>(a / b) * y;
    return gcd;
}
uint64_t RSAUtils::modInverse(uint64_t a, uint64_t n)
{
    /**
     * Calculate the modular multiplicative inverse of a mod n.
     * @param a: var1
     * @param n: mod
     * @return modular multiplicative inverse of a mod n---a^-1(mod n)
     */
    int64_t x, y;
    uint64_t gcd = extendedGCD(a, n, x, y);
    // ax+ny=gcd(a,n)
    // x = a^-1(mod n)
    // y = n^-1(mod a)
    if (gcd != 1)
        // a and n are not coprime, so the modular multiplicative inverse does not exist.
        return 0;
    else
        // To make it a positive number
        return (x + n) % n;
}
bool RSAUtils::MillerRabin(uint64_t n)
{
    /**
     * Test whether n is a prime number using Miller-Rabin Algorithm.
     * @param n: number to be tested
     * @return true: prime number
     *         false: composite number
     */
    if (n == 2 || n == 3)
        return true;
    if (n < 2 || n % 2 == 0)
        return false;
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis(1, n - 1);
    uint64_t a = dis(gen);
    uint64_t temp = n - 1;
    uint64_t k = 0;
    while (temp % 2 == 0)
    {
        k++;
        temp /= 2;
    }
    uint64_t m = temp;
    uint64_t b = powMod(a, m, n);
    if (powMod(b, 1, n) % n == 1)
        return true;
    for (uint64_t i = 0; i < k; i++)
    {
        if (powMod(b, 1, n) == n - 1)
            return true;
        else
            b = powMod(b, 2, n);
    }
    return false;
}
bool RSAUtils::primeTest(uint64_t n, int approach, int rounds)
{
    /**
     * Test whether n is a prime number using different approaches.
     * @param n: number to be tested
     * @param approach: test approach
     *                  0: Euclidean Algorithm
     *                  1: Fermat Algorithm
     *                  2: Miller-Rabin Algorithm
     * @param rounds: test rounds
     * @return true: prime number
     *         false: composite number
     */
    if (n == 2 || n == 3)
        return true;
    if (n < 2 || n % 2 == 0)
        return false;
    switch (approach)
    {
    case MILLER_RABIN:
        /**
         * Miller-Rabin Algorithm
         * Based on the fact that Miller-Rabin Algorithm is a probabilistic algorithm,
         * A odd composite number n has at least 75% probability to be detected as a composite number
         * As a result, we repeat the test for rounds times to improve the accuracy.
         */
        while (rounds--)
        {
            if (!MillerRabin(n))
                return false;
        }
        return true;
        break;
    case EUCLIDEAN:
        // continue to implement
        return true;
        break;
    case FERMAT:
        // continue to implement
        return true;
        break;
    default:
        return false;
        break;
    }
    return false;
}
uint64_t RSAUtils::generateRamdomPrime(int bits, int rounds)
{
    /**
     * Generate a random prime number with the specified bits.
     * @param bits: bits of the prime number
     * @param rounds: test rounds
     * @return prime: random prime number
     */
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis(0, (((uint64_t)1) << (bits - 2)) - 1);
    uint64_t base;
    do
    {
        base = (uint64_t)1 << (bits - 1);
        base += dis(gen);
        base |= 1; // Make sure the number is odd

    } while (!primeTest(base, MILLER_RABIN, rounds));
    return base;
}

int RSAUtils::generatePublicKey(uint64_t n, int rounds)
{
    /**
     * Generate public key paramter (e, n) for RSA encryption.
     * @param n: number
     * @param rounds: test rounds
     * @return SUCCESS or FAILURE
     */
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis(2, phi_n - 1);
    int64_t x, y;
    do
    {
        e = dis(gen);
        rounds--;
    } while ((extendedGCD(e, phi_n, x, y) != 1) && (rounds > 0));
    if (rounds == 0)
        return FAILURE;
    // Assign the public key
    publicKey.first = e;
    publicKey.second = n;
    return SUCCESS;
}
pair<uint64_t, uint64_t> RSAUtils::getPublicKey()
{
    return publicKey;
}
void RSAUtils::setPublicKey(uint64_t e, uint64_t n)
{
    this->e = e;
    publicKey.first = e;
    this->n = n;
    publicKey.second = n;
    return;
}
int RSAUtils::generatePrivateKey(uint64_t n)
{
    /**
     * Generate private key paramter (d, n) for RSA decryption.
     * @param n: number
     * @return SUCCESS or FAILURE
     */
    d = modInverse(e, phi_n);
    if (d == 0)
    {
        return FAILURE; // If d = 0, resprsenting the case where no inverse exists
    }
    privateKey.first = d;
    privateKey.second = n;
    return SUCCESS;
}
pair<uint64_t, uint64_t> RSAUtils::getPrivateKey()
{
    return privateKey;
}
void RSAUtils::setPrivateKey(uint64_t d, uint64_t n)
{
    this->d = d;
    privateKey.first = d;
    this->n = n;
    privateKey.second = n;
    return;
}
int RSAUtils::generateKeyPair(uint64_t n, int rounds)
{
    /**
     * Generate RSA key pair both for encryption and decryption.
     * @param n: number
     */
    // Generate public key first, so the private key can be generated based on the public key.
    if (generatePublicKey(n, rounds) == FAILURE)
        return FAILURE;
    if (generatePrivateKey(n) == FAILURE)
        return FAILURE;
    return SUCCESS;
}
void RSAUtils::encrypt(uint64_t &plaintext, uint64_t &ciphertext)
{
    /**
     * Encrypt the plaintext using RSA encryption algorithm.
     * According to the RSA encryption algorithm:
     * C = M^e (mod n) using the public key (e, n)
     * @param plaintext: plaintext
     * @param ciphertext: ciphertext
     */
    ciphertext = powMod(plaintext, e, n);
    return;
}
void RSAUtils::decrypt(uint64_t &plaintext, uint64_t &ciphertext)
{
    /**
     * Decrypt the ciphertext using RSA decryption algorithm.
     * According to the RSA decryption algorithm:
     * M = C^d (mod n) using the private key (d, n)
     * @param plaintext: plaintext
     * @param ciphertext: ciphertext
     */
    cout << "p:" << p << endl;
    cout << "q:" << q << endl;
    cout << "phi_n:" << phi_n << endl;
    cout << "e: " << e << endl;
    cout << "d: " << d << endl;
    cout << "n: " << n << endl;
    plaintext = powMod(ciphertext, d, n);
    return;
}
int RSAUtils::init(int rounds, bool defaultKey, bool highSecurity)
{
    /**
     * @brief Initialize the RSAUtils
     * @details We need to follow the steps below:
     * 1. Generate two random prime numbers p and q
     * 2. Calculate n = p * q
     * 3. Calculate phi = (p - 1) * (q - 1)
     * 4. Generate public key (e, n)
     * 5. Generate private key (d, n)
     * @note If there is a need to enhance the security, we need to follow some standards:
     * 1. The length of p and q should be the nearly the same
     * 2. Large prime factors for (p-1) and (q-1)
     * 3. Small Greatest Common Divisor(GCD) for (p-1) and (q-1)
     * 4. Size of d when e<n and d>n^0.25
     * 5. Defending against Timing Attacks by
     *   - Implementing constant-time algorithms for operations such as modular exponentiation, ensuring that execution time does not depend on the values of the inputs.
     *   - Introducing random delays in the algorithm to obscure the execution time.
     *   - Randomizing operations, such as multiplying the plaintext by a random number before encryption, to make the timing analysis more challenging.
     * @param rounds: test rounds
     * @param defaultKey: whether to use the default key as e=65537(2^16+1).
     *                    It not only provides a good balance between security(low Hamming weight) and efficiency(computationlly efficient)
     *                    but also is widely used in practice.
     * @param highSecurity: whether to use the high security standard
     * @return SUCCESS or FAILURE
     */
    if (highSecurity)
    {
        // Advanced Standard for better security
        // Continuing to implement......
        if (defaultKey)
            e = 65537;
    }
    else
    {
        // Basic Standard with high efficiency
        // First, generate two random prime numbers p and q
        do
        {
            p = generateRamdomPrime(32, rounds);
            q = generateRamdomPrime(32, rounds);
            // In order to avoid overflow, we need to make sure p * q <= 2^64
            // At the same time, to make both of them as large as possible
        } while ((rounds--) && (p * q > 0xffffffffffffffff));
        n = p * q;
        phi_n = (p - 1) * (q - 1);
        // Generate RSA key pair
        if (defaultKey)
        {
            // Use the default key as e=65537(2^16+1)
            e = 65537;
            // Assign the public key
            this->e = e;
            publicKey.first = e;
            publicKey.second = n;
            // Generate private key based on the public key
            if (generatePrivateKey(n) == FAILURE)
                return FAILURE;
        }
        else
        {
            // Generate RSA key pair
            if (generateKeyPair(n) == FAILURE)
                return FAILURE;
        }
    }
    return SUCCESS;
}