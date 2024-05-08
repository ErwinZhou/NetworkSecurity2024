// Defines the MD5 class including constants and interfaces for MD5 algorithm.

#ifndef MD5_HPP
#define MD5_HPP
#include "def.hpp"
#include <cstdint>
#include <vector>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

class MD5
{
private:
    /**
     * Constants for MD5 algorithm
     */
    uint32_t IV[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476}; // Initial vectors for MD5 algorithm

    const uint32_t s_table[4][4] = {{7, 12, 17, 22}, {5, 9, 14, 20}, {4, 11, 16, 23}, {6, 10, 15, 21}}; // Shift values for each round

    const uint8_t padding[64] = {0x80};       // Padding values for each round
    uint8_t *messageDigest = new uint8_t[16]; // The final message digest in Bytes form
    uint8_t *bufferBlock = new uint8_t[64];   // The buffer block for the input message
    uint32_t *bitsCounter = new uint32_t[2];  // The counter for the number of bits in the input message
    bool isFinished;                          // The flag to indicate whether the MD5 algorithm is finished

    /**
     * Preprocessing for MD5: padding the input message & decoding the input message
     */
    // void padding(const void *input, size_t length, std::vector<uint32_t> &blocks);
    void decode(const uint8_t *input, uint32_t *output, size_t length); // Decode the MD5 message digest from BYTE to DWORD
    /**
     * Basic operations commonly used in MD5 algorithm
     */
    INT mod(INT a, INT b);
    uint32_t F(uint32_t x, uint32_t y, uint32_t z);
    uint32_t G(uint32_t x, uint32_t y, uint32_t z);
    uint32_t H(uint32_t x, uint32_t y, uint32_t z);
    uint32_t I(uint32_t x, uint32_t y, uint32_t z);
    uint32_t leftRotate(uint32_t x, uint32_t n);

    /**
     * Higher-level nonlinear operations used in MD5 algorithm to update initial vectors
     */
    void FF(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t M_k, uint32_t s, uint32_t T_i);
    void GG(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t M_k, uint32_t s, uint32_t T_i);
    void HH(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t M_k, uint32_t s, uint32_t T_i);
    void II(uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t M_k, uint32_t s, uint32_t T_i);

    INT roundHandler(const uint8_t block[64]); // Handler for each round of MD5 algorithm

    INT update(const void *input, size_t length, INT advancedSecurity = 0);    // Bytes flow
    INT update(const std::string &input_str, INT advancedSecurity = 0);        // String flow
    INT update(std::ifstream &input_file, INT advancedSecurity = 0);           // File flow
    INT update(const uint8_t *input, size_t length, INT advancedSecurity = 0); // Update the status of the MD5 object

    /**
     * Postprocessing for MD5
     */
    void encode(const uint32_t *input, uint8_t *output, size_t length); // Encode the MD5 message digest from DWORD to BYTE
    std::string bytesToHexString(const uint8_t *input, size_t length);  // Convert the BYTE array to HEX string
    void finalize();                                                    // Finalization for MD5 algorithm, including padding and process the last block

public:
    MD5();
    ~MD5();

    /**
     * Computing Message Digest for the input string or file
     * @param advancedSecurity: advanced security level
     *                          0 for basic security
     *                          1 for Advanced Security-Divide and Merge
     *                          >1 for Advanced Security-Times for Recusion of Computing MD5
     */
    INT compute(const void *input, size_t length, INT advancedSecurity = 0); // Bytes flow
    INT compute(const std::string &input_str, INT advancedSecurity = 0);     // String flow
    INT compute(std::ifstream &input_file, INT advancedSecurity = 0);        // File flow

    const uint8_t *getMessageDigest(); // Return the final message digest in Bytes form
    std::string toString();            // Return the final message digest in HEX string form
    void reset();                      // Reset the MD5 object especially for the inital vectors
};

#endif // MD5_HPP