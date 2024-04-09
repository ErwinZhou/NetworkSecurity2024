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
        if (gcd(i, n) == 1)
            phi++;
    }
    return phi;
}

// int EulerTotientFunction(int n) {//计算欧拉函数，用来辅助求解最小原根
//	int CoPrimeCount = 0;
//	for (int i = 1; i < n; i++) {//判断从1-n-1中与n互素的数，其个数即为n的缩系大小
//		if (if_Coprime(i, n)) {
//			CoPrimeCount++;
//		}
//	}
//	return CoPrimeCount;
// }
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
bool RSAUtils::primeTest(uint64_t n, int approach, int rounds = 100)
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
        break;
    case FERMAT:
        // continue to implement
        break;
    default:
        break;
    }
}
uint64_t RSAUtils::generateRamdomPrime(char bits, int rounds = 100)
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
uint64_t RSAUtils::gcd(uint64_t &p, uint64_t &q)
{
    /**
     * Calculate the greatest common divisor of a and b using Euclidean Algorithm.
     * @param p: var1
     * @param q: var2
     * @return a: greatest common divisor
     */
    uint64_t a = p > q ? p : q;
    uint64_t b = p < q ? p : q;
    uint64_t t;
    if (p == q)
        // If the two numbers are equal, the greatest common divisor is the number itself.
        return p;
    while (b)
    {
        // gcd(a, b)=gcd(b, a-qb)
        t = a % b;
        a = b;
        b = t;
    }
    return a;
}
int RSAUtils::generatePublicKey(uint64_t n, int rounds = 100)
{
    /**
     * Generate public key paramter (e, n) for RSA encryption.
     * @param n: number
     * @param rounds: test rounds
     * @return SUCCESS or FAILURE
     */
    random_device rd;
    mt19937_64 gen(rd());
    uint64_t phi_n = EulerTotientFunction(n);
    uniform_int_distribution<uint64_t> dis(2, phi_n - 1);
    do
    {
        e = dis(gen);
        rounds--;
    } while ((gcd(e, phi_n) != 1) && (rounds > 0));
    if (rounds == 0)
        return FAILURE;
    // Assign the public key
    publicKey.first = e;
    publicKey.second = n;
    return SUCCESS;
}
int RSAUtils::generatePrivateKey(uint64_t n)
{
    /**
     * Generate private key paramter d for RSA decryption.
     * @param n: number
     * @return SUCCESS or FAILURE
     */
    uint64_t phi_n = EulerTotientFunction(n);
    uint64_t MAX = 0xffffffffffffffff - phi_n;
    uint64_t i = 1;
    while (true)
    {
        if (((i * phi_n) + 1) % e == 0)
            d = ((i * phi_n) + 1) % e;
        i++;
        if ((i + 1) * phi_n > MAX)
            return FAILURE;
    }
    // Assign the private key
    privateKey.first = d;
    privateKey.second = n;
    return SUCCESS;
}
void RSAUtils::generateKeyPair(uint64_t n)
{
    /**
     * Generate RSA key pair.
     * @param n: number
     */
    generatePublicKey(n);
    generatePrivateKey(n);
}