// Define the constants and macros for the global use

#ifndef DEF_HPP
#define DEF_HPP

// Include the important files for protocol struct
#include <netinet/ip.h>      // struct iphdr
#include <netinet/tcp.h>     // struct tcphdr
#include <netinet/udp.h>     // struct udphdr
#include <netinet/ip_icmp.h> // struct icmphdr

// Macros
#define MAX_PORT 65535
#define MIN_PORT 0

// Arguments
#define ARG_HELP "-h"
#define ARG_CONNECT_SCAN "-c"
#define ARG_SYN_SCAN "-s"
#define ARG_FIN_SCAN "-f"
#define ARG_UDP_SCAN "-u"

#define SUCCESS 0
#define FAILURE -1

#endif // DEF_HPP