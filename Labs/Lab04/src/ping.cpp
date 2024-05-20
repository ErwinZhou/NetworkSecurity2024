// Define the declared functions in the ping.hpp
// To implement the ping scanning using ICMP protocol

#include "ping.hpp"

ICMPUtil::ICMPUtil()
{
}
ICMPUtil::~ICMPUtil()
{
}
bool ICMPUtil::ping(std::string hostIP, std::string localHostIP)
{
    /**
     * The function to ping the host using ICMP protocol
     * @param hostIP The IP address of the host
     * @param localHostIP The IP address of the local host
     * @return true if the host is reachable, false otherwise
     */

    /* Definiton of the variables */
    int pingSocket;     // socket for ICMP
    int on;             // the flag for the socket
    int ret;            // return results
    int sendBufferSize; // the size for sending buffer

    // Create the raw socket
    pingSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    /* Set the socket relevant Settings, for the need to include and operate the IP header */
    on = 1;
    ret = setsockopt(pingSocket, 0, IP_HDRINCL, &on, sizeof(on));

    /**
     * Create the ICMP Package
     * The ICMP Package should include three parts:
     * 1. IP Header
     * 2. ICMP Header
     * 3. Timeval struct
     */
    sendBufferSize = sizeof(struct iphdr) + sizeof(struct icmphdr) + sizeof(timeval);
}