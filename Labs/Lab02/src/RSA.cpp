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
     *                  0: Euclidian Algorithm
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
    case EUCLIDIAN:
        // continue to implement
        break;
    case FERMAT:
        // continue to implement
        break;
    default:
        break;
    }
}
void RSAUtils::generateRamdomPrime(uint64_t &p, uint64_t &q)
{
    /**
     * Generate two random prime numbers p and q.
     * @param p: prime number p
     * @param q: prime number q
     */
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis(1, UINT64_MAX);
    while (1)
    {
        p = dis(gen);
        if (primeTest(p, MILLER_RABIN))
            break;
    }
    while (1)
    {
        q = dis(gen);
        if (primeTest(q, MILLER_RABIN))
            break;
    }
}