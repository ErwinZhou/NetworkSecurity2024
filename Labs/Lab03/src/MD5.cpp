// Completion of MD5 algorithm for the specific class-MD5

#include "MD5.hpp"

MD5::MD5() : isFinished(false)
{
    memset(bufferBlock, 0, sizeof(bufferBlock));
    memset(bitsCounter, 0, sizeof(bitsCounter));
    memset(messageDigest, 0, sizeof(messageDigest));
}

MD5::~MD5()
{
}

// Preprocessing for MD5
void MD5::decode(const uint8_t *input, uint32_t *output, size_t length)
{
    /**
     * Decode the MD5 message digest from BYTE to DWORD
     * @param input: the input message digest in BYTE form
     * @param output: the output message digest in DWORD form
     * @param length: the length of the input message digest
     */
    for (size_t i = 0, j = 0; j < length; i++, j += 4)
        output[i] = ((uint32_t)input[j]) | (((uint32_t)input[j + 1]) << 8) | (((uint32_t)input[j + 2]) << 16) | (((uint32_t)input[j + 3]) << 24);
    return;
}

std::string MD5::bytesToHexString(const uint8_t *input, size_t length)
{
    /**
     * Convert the BYTE array to HEX string
     * @param input: the input message digest in BYTE form
     * @param length: the length of the input message digest
     */
    // If the computation is not finished, return an empty string
    if (!isFinished)
        return "";
    std::string hexString;
    for (size_t i = 0; i < length; i++)
    {
        char hex[3];
        sprintf(hex, "%02x", input[i]);
        hexString += hex;
    }
    return hexString;
}
// Basic operations commonly used in MD5 algorithm

uint32_t MD5::F(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) | (~x & z);
}

uint32_t MD5::G(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & z) | (y & ~z);
}

uint32_t MD5::H(uint32_t x, uint32_t y, uint32_t z)
{
    return x ^ y ^ z;
}

uint32_t MD5::I(uint32_t x, uint32_t y, uint32_t z)
{
    return y ^ (x | ~z);
}

uint32_t MD5::leftRotate(uint32_t x, uint32_t n)
{
    /**
     * Perforom left rotation on x by n bits
     * @param x: the input value
     * @param n: the number of bits to rotate
     */
    return (x << n) | (x >> (32 - n));
}

// Higher-level operations used in MD5 algorithm
void MD5::FF(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t M_k, uint32_t s, uint32_t T_i)
{
    /**
     * Nonlinear operations on inital vectors (a, b, c, d) using F
     * Based on hard-coded solution: a = b + ((a + I(b, c, d) + X[k] + T[i]) <<< s)
     * @param a, b, c, d: initial vector
     * @param M_k: the k-th block of message
     * @param s: shift value
     * @param i: the i-th round
     */
    a = b + (leftRotate(a + F(b, c, d) + M_k + T_i, s));
    return;
}

void MD5::GG(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t M_k, uint32_t s, uint32_t T_i)
{
    a = b + (leftRotate(a + G(b, c, d) + M_k + T_i, s));
    return;
}

void MD5::HH(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t M_k, uint32_t s, uint32_t T_i)
{
    a = b + (leftRotate(a + H(b, c, d) + M_k + T_i, s));
    return;
}

void MD5::II(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t M_k, uint32_t s, uint32_t T_i)
{
    a = b + (leftRotate(a + I(b, c, d) + M_k + T_i, s));
    return;
}

INT MD5::roundHandler(const uint8_t block[64])
{
    /**
     * Round Handler for each round of MD5 algorithm
     * @param block: the input block of 64 bytes, 512 bits in total
     */

    // Store the temporary values of a, b, c, d
    uint32_t a = IV[0], b = IV[1], c = IV[2], d = IV[3];
    // Define the smaller block x which has 32Bytes for 16 ones
    uint32_t x[16];
    // Decode the uint8_t block for 64 -> uint32_t for 16
    decode(block, x, 64);

    /* Round 1 */
    FF(a, b, c, d, x[0], s_table[0][0], 0xd76aa478);  /* 1 */
    FF(d, a, b, c, x[1], s_table[0][1], 0xe8c7b756);  /* 2 */
    FF(c, d, a, b, x[2], s_table[0][2], 0x242070db);  /* 3 */
    FF(b, c, d, a, x[3], s_table[0][3], 0xc1bdceee);  /* 4 */
    FF(a, b, c, d, x[4], s_table[0][0], 0xf57c0faf);  /* 5 */
    FF(d, a, b, c, x[5], s_table[0][1], 0x4787c62a);  /* 6 */
    FF(c, d, a, b, x[6], s_table[0][2], 0xa8304613);  /* 7 */
    FF(b, c, d, a, x[7], s_table[0][3], 0xfd469501);  /* 8 */
    FF(a, b, c, d, x[8], s_table[0][0], 0x698098d8);  /* 9 */
    FF(d, a, b, c, x[9], s_table[0][1], 0x8b44f7af);  /* 10 */
    FF(c, d, a, b, x[10], s_table[0][2], 0xffff5bb1); /* 11 */
    FF(b, c, d, a, x[11], s_table[0][3], 0x895cd7be); /* 12 */
    FF(a, b, c, d, x[12], s_table[0][0], 0x6b901122); /* 13 */
    FF(d, a, b, c, x[13], s_table[0][1], 0xfd987193); /* 14 */
    FF(c, d, a, b, x[14], s_table[0][2], 0xa679438e); /* 15 */
    FF(b, c, d, a, x[15], s_table[0][3], 0x49b40821); /* 16 */

    /* Round 2 */
    GG(a, b, c, d, x[1], s_table[1][0], 0xf61e2562);  /* 17 */
    GG(d, a, b, c, x[6], s_table[1][1], 0xc040b340);  /* 18 */
    GG(c, d, a, b, x[11], s_table[1][2], 0x265e5a51); /* 19 */
    GG(b, c, d, a, x[0], s_table[1][3], 0xe9b6c7aa);  /* 20 */
    GG(a, b, c, d, x[5], s_table[1][0], 0xd62f105d);  /* 21 */
    GG(d, a, b, c, x[10], s_table[1][1], 0x02441453); /* 22 */
    GG(c, d, a, b, x[15], s_table[1][2], 0xd8a1e681); /* 23 */
    GG(b, c, d, a, x[4], s_table[1][3], 0xe7d3fbc8);  /* 24 */
    GG(a, b, c, d, x[9], s_table[1][0], 0x21e1cde6);  /* 25 */
    GG(d, a, b, c, x[14], s_table[1][1], 0xc33707d6); /* 26 */
    GG(c, d, a, b, x[3], s_table[1][2], 0xf4d50d87);  /* 27 */
    GG(b, c, d, a, x[8], s_table[1][3], 0x455a14ed);  /* 28 */
    GG(a, b, c, d, x[13], s_table[1][0], 0xa9e3e905); /* 29 */
    GG(d, a, b, c, x[2], s_table[1][1], 0xfcefa3f8);  /* 30 */
    GG(c, d, a, b, x[7], s_table[1][2], 0x676f02d9);  /* 31 */
    GG(b, c, d, a, x[12], s_table[1][3], 0x8d2a4c8a); /* 32 */

    /* Round 3 */
    HH(a, b, c, d, x[5], s_table[2][0], 0xfffa3942);  /* 33 */
    HH(d, a, b, c, x[8], s_table[2][1], 0x8771f681);  /* 34 */
    HH(c, d, a, b, x[11], s_table[2][2], 0x6d9d6122); /* 35 */
    HH(b, c, d, a, x[14], s_table[2][3], 0xfde5380c); /* 36 */
    HH(a, b, c, d, x[1], s_table[2][0], 0xa4beea44);  /* 37 */
    HH(d, a, b, c, x[4], s_table[2][1], 0x4bdecfa9);  /* 38 */
    HH(c, d, a, b, x[7], s_table[2][2], 0xf6bb4b60);  /* 39 */
    HH(b, c, d, a, x[10], s_table[2][3], 0xbebfbc70); /* 40 */
    HH(a, b, c, d, x[13], s_table[2][0], 0x289b7ec6); /* 41 */
    HH(d, a, b, c, x[0], s_table[2][1], 0xeaa127fa);  /* 42 */
    HH(c, d, a, b, x[3], s_table[2][2], 0xd4ef3085);  /* 43 */
    HH(b, c, d, a, x[6], s_table[2][3], 0x04881d05);  /* 44 */
    HH(a, b, c, d, x[9], s_table[2][0], 0xd9d4d039);  /* 45 */
    HH(d, a, b, c, x[12], s_table[2][1], 0xe6db99e5); /* 46 */
    HH(c, d, a, b, x[15], s_table[2][2], 0x1fa27cf8); /* 47 */
    HH(b, c, d, a, x[2], s_table[2][3], 0xc4ac5665);  /* 48 */

    /* Round 4 */
    II(a, b, c, d, x[0], s_table[3][0], 0xf4292244);  /* 49 */
    II(d, a, b, c, x[7], s_table[3][1], 0x432aff97);  /* 50 */
    II(c, d, a, b, x[14], s_table[3][2], 0xab9423a7); /* 51 */
    II(b, c, d, a, x[5], s_table[3][3], 0xfc93a039);  /* 52 */
    II(a, b, c, d, x[12], s_table[3][0], 0x655b59c3); /* 53 */
    II(d, a, b, c, x[3], s_table[3][1], 0x8f0ccc92);  /* 54 */
    II(c, d, a, b, x[10], s_table[3][2], 0xffeff47d); /* 55 */
    II(b, c, d, a, x[1], s_table[3][3], 0x85845dd1);  /* 56 */
    II(a, b, c, d, x[8], s_table[3][0], 0x6fa87e4f);  /* 57 */
    II(d, a, b, c, x[15], s_table[3][1], 0xfe2ce6e0); /* 58 */
    II(c, d, a, b, x[6], s_table[3][2], 0xa3014314);  /* 59 */
    II(b, c, d, a, x[13], s_table[3][3], 0x4e0811a1); /* 60 */
    II(a, b, c, d, x[4], s_table[3][0], 0xf7537e82);  /* 61 */
    II(d, a, b, c, x[11], s_table[3][1], 0xbd3af235); /* 62 */
    II(c, d, a, b, x[2], s_table[3][2], 0x2ad7d2bb);  /* 63 */
    II(b, c, d, a, x[9], s_table[3][3], 0xeb86d391);  /* 64 */

    // Update the initial vectors
    IV[0] += a;
    IV[1] += b;
    IV[2] += c;
    IV[3] += d;

    // Clear the temporary values
    memset(x, 0, sizeof(x));
    return SUCCESS;
}

INT MD5::update(const uint8_t *input, size_t length)
{
    /**
     * Update the status of the MD5 message digest for the input Bytes flow of given length
     * @param input: bytes flow of the input
     * @param length: the given bytes length of the input bytes flow
     */
    uint32_t i, remainingIndex, remainingLength;
    // To get the index of the bytes that are not processed yet and was stored in the buffer
    remainingIndex = (uint8_t)((bitsCounter[0] >> 3) & 0x3F);

    // To keep track of the bits of the message that has already been processed
    if ((bitsCounter[0] += length << 3) < (length << 3))
        bitsCounter[1]++;
    bitsCounter[1] += uint32_t(length >> 29);

    // To get the number of bytes that are not processed yet
    remainingLength = 64 - remainingIndex;

    /**
     * Basic Security: Perform the basic MD5 algorithm
     */
    if (length >= remainingLength)
    {
        memcpy(&bufferBlock[remainingIndex], input, remainingLength);
        roundHandler(bufferBlock);
        for (i = remainingLength; i + 63 < length; i += 64)
            roundHandler(&input[i]);
        remainingIndex = 0;
    }
    else
        i = 0;
    memcpy(&bufferBlock[remainingIndex], &input[i], length - i);
    return SUCCESS;
}

// Postprocessing for MD5
void MD5::encode(const uint32_t *input, uint8_t *output, size_t length)
{
    /**
     * Encode the MD5 message digest from DWORD to BYTE
     * @param input: the input message digest in DWORD form
     * @param output: the output message digest in BYTE formj
     * @param length: the length of the input messjage digest
     */
    for (size_t i = 0, j = 0; j < length; i++, j += 4)
    {
        output[j] = (uint8_t)(input[i] & 0xFF);
        output[j + 1] = (uint8_t)((input[i] >> 8) & 0xFF);
        output[j + 2] = (uint8_t)((input[i] >> 16) & 0xFF);
        output[j + 3] = (uint8_t)((input[i] >> 24) & 0xFF);
    }
}

// Different forms of interfaces for MD5
INT MD5::update(const void *input, size_t length)
{
    /**
     * Update the status of the MD5 object in bytes flow
     * @param input: bytes flow of the input
     * @param length: the given length of the input bytes flow
     */
    // Call upon on the base interface
    update((const uint8_t *)input, length);
    return SUCCESS;
}

INT MD5::update(const std::string &input_str)
{
    /**
     * Update the status of the MD5 object in string flow
     * @param input_str: the input string
     */
    // Call upon on the base interface
    update(input_str.c_str(), input_str.length());
    return SUCCESS;
}

INT MD5::update(std::ifstream &input_file)
{
    /**
     * Update the status of the MD5 object in file flow
     * @param input_file: the input file stream
     */
    // Read the file content
    input_file.seekg(0, std::ios::end);
    std::streamsize size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (input_file.read(buffer.data(), size))
        // Call upon on the base interface
        update((const uint8_t *)buffer.data(), size);
    else
        return FAILURE;
    return SUCCESS;
}

// Public Interfaces for MD5
void MD5::finalize()
{
    /**
     * Finalization for MD5 algorithm, including padding and process the last block
     */
    uint8_t bits[8] = {0};
    uint32_t remainingIndex, paddingLength;
    // If the computation is finished, that means there is no need to padding and finalization
    if (isFinished)
        return;
    // Get the index of the bytes that are not processed yet and was stored in the buffer
    remainingIndex = (uint8_t)((bitsCounter[0] >> 3) & 0x3F);
    /**
     * Padding: 0x80 + 0x00 * (448 mod 512) + bitsLength
     *          First to compute the padding length
     *          If the remainingIndex < 56, paddingLength = 56 - remainingIndex
     *          Else simply add another block of padding, paddingLength = 120 - remainingIndex
     */
    paddingLength = (remainingIndex < 56) ? (56 - remainingIndex) : (120 - remainingIndex);
    // Store the length of the bits, for later filling
    encode(bitsCounter, bits, 8);
    // Perform the padding
    update(padding, paddingLength);
    // Perform the bits length
    update(bits, 8);

    // Store the final message digest
    encode(IV, messageDigest, 16);
    // Set the computation to be finished
    isFinished = true;
    // Clear the status
    memset(bufferBlock, 0, sizeof(bufferBlock));
    memset(bitsCounter, 0, sizeof(bitsCounter));
    return;
}

INT MD5::compute(const void *input, size_t length)
{
    /**
     * Computing the MD5 message digest for the input bytes flow
     * @param input: bytes flow of the input
     * @param length: the given length of the input bytes flow
     */
    // Reset all the parameters in the MD5 to clear out the status
    reset();
    if (update(input, length) == FAILURE)
        return FAILURE;
    finalize();
    return SUCCESS;
}

INT MD5::compute(const std::string &input_string)
{
    /**
     * Computing the MD5 message digest for the input string
     * @param input_string: the input string
     */
    // Reset all the parameters in the MD5 to clear out the status
    reset();
    if (update(input_string) == FAILURE)
        return FAILURE;
    finalize();
    return SUCCESS;
}

INT MD5::compute(std::ifstream &input_file)
{
    /**
     * Computing the MD5 message digest for the input file
     * @param input_file: the input file stream
     */
    // Reset all the parameters in the MD5 to clear out the status
    reset();
    if (update(input_file) == FAILURE)
        return FAILURE;
    finalize();
    return SUCCESS;
}

const uint8_t *MD5::getMessageDigest()
{
    /**
     * Return the final message digest in Bytes form
     */
    return messageDigest;
}

std::string MD5::toString()
{
    /**
     * Convert the message digest to HEX string
     * Follow the usage of getMessgeDigest()
     */
    return bytesToHexString(messageDigest, 16);
}

void MD5::reset()
{
    /**
     * Reset initial vectors for later usage
     */
    isFinished = false;
    IV[0] = 0x67452301;
    IV[1] = 0xefcdab89;
    IV[2] = 0x98badcfe;
    IV[3] = 0x10325476;
    memset(bufferBlock, 0, sizeof(bufferBlock));
    memset(bitsCounter, 0, sizeof(bitsCounter));
    memset(messageDigest, 0, sizeof(messageDigest));
    return;
}