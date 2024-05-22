// Declarations of global utility functions utilized in other cpp files

#ifndef UTILS_HPP
#define UTILS_HPP
#include <cstdint> // uint16_t and other unsigned integer
#include <cstring> // memset

uint16_t in_cksum(uint16_t *addr, int len); // Compute the checksum of headers
#endif                                      // UTILS_HPP