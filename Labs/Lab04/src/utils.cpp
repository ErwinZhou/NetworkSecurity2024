// Definitions and implementations of the declared functions in utils.hpp

#include "utils.hpp"

// Compute the checksum of the headers
uint16_t in_cksum(uint16_t *addr, int len)
{
    /**
     * The function to compute the checksum of the headers
     * @param addr The address of the headers
     * @param len The length of the headers
     * @return The checksum of the headers
     */

    int nleft = len;
    int sum = 0;
    uint16_t *w = addr;
    uint16_t answer = 0;
    // Sum up 2-byte values until none or only one byte left.
    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }
    // Add leftover byte, if any
    if (nleft == 1)
    {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }
    // Fold 32-bit sum into 16 bits
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;
}
