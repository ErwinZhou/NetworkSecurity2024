// Declarations of global utility functions utilized in other cpp files

#ifndef UTILS_HPP
#define UTILS_HPP

#include "def.hpp" // Include the constants and macros for the global use
#include <cstdint> // uint16_t and other unsigned integer
#include <cstring> // memset
#include <regex>   // std::regex_match

uint16_t in_cksum(uint16_t *addr, int len);   // Compute the checksum of headers
bool isValidIPv4(const char *pszIPAddr);      // Check if the input is a valid IPv4 address
bool isValidPort(int beginPort, int endPort); // Check if the begin and end ports are valid for out of range

#endif // UTILS_HPP