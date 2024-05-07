// Defines the MD5 class including constants and interfaces for MD5 algorithm.

#ifndef MD5_HPP
#define MD5_HPP
#include "def.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
class MD5
{
private:
    /**
     * Constants for MD5 algorithm
     */
    uint32_t IV[4] = {0x1234567, 0x89abcdef, 0xfedcba98, 0x76543210}; // Initial values for MD5 algorithm

    /**
     * Basic operations commonly used in MD5 algorithm
     */
    uint32_t F(uint32_t X, uint32_t Y, uint32_t Z);
    uint32_t G(uint32_t X, uint32_t Y, uint32_t Z);
    uint32_t H(uint32_t X, uint32_t Y, uint32_t Z);
    uint32_t I(uint32_t X, uint32_t Y, uint32_t Z);

    /**
     * Higher-level operations used in MD5 algorithm
     */
    uint32_t FF(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t M, uint32_t s, uint32_t t);
    uint32_t GG(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t M, uint32_t s, uint32_t t);
    uint32_t HH(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t M, uint32_t s, uint32_t t);
    uint32_t II(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t M, uint32_t s, uint32_t t);
    uint32_t round_handler(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t M, uint32_t s, uint32_t t, uint32_t (*func)(uint32_t, uint32_t, uint32_t));

public:
    MD5();
    ~MD5();
    /**
     * Generating Message Digest for the input string or file
     * @param advancedSecurity: advanced security level
     *                          0 for basic security
     *                          1 for Advanced Security-Divide and Merge
     *                          >1 for Advanced Security-Times for Recusion of Computing MD5
     */
    INT gene(const std::string &input_str, INT advancedSecurity = 0);
    INT gene(const std::ifstream &input_file);
    void reset(); // Reset the MD5 object especially for the inital vectors
};

#endif // MD5_HPP