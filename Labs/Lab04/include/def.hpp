// Define the constants and macros for the global use

#ifndef DEF_HPP
#define DEF_HPP

// Include the important files for protocol struct
#include <netinet/ip.h>      // struct iphdr(Linux)
#include <netinet/tcp.h>     // struct tcphdr(Linux)
#include <netinet/udp.h>     // struct udphdr(Linux)
#include <netinet/ip_icmp.h> // struct icmphdr(Linux), ip and icmp

typedef int INT;

// Macros
#define MAX_PORT 65535
#define MIN_PORT 0
#define DEFAULT_LOCAL_HOST_IP "127.0.0.1"       // The local host IP address
#define DEFAULT_LOCAL_INET_IP "192.168.126.128" // The IPv4 address of the local host
#define DEFAULT_LOCAL_PORT 3015
#define DEFAULT_HOST_IP "127.0.0.1"
#define DEFAULT_HOST_TEST_IP "110.242.68.66" // 110.242.68.66 is the IPv4 address for www.baidu.com, which is used for testing
#define MAX_BUFFERS_SIZE 1024
#define MAX_TIMEOUT 3600
#define DEFAULT_PING_TIMES 10

// Arguments
#define ARG_HELP "-h"
#define ARG_PING "-p"
#define ARG_CONNECT_SCAN "-c"
#define ARG_SYN_SCAN "-s"
#define ARG_FIN_SCAN "-f"
#define ARG_UDP_SCAN "-u"

// Return flags
#define SUCCESS 1
#define FAILURE 0
#define ERROR -1
#define TIMEOUT -2
#endif // DEF_HPP