// The constants, variables and declarations about the Ping Scanning  using ICMP protocol

#ifndef PING_HPP
#define PING_HPP

#include "def.hpp"
#include "utils.hpp"
#include <string>       // std::string
#include <cstring>      // memset
#include <iostream>     // std::cout
#include <sys/time.h>   // gettimeofday
#include <sys/socket.h> // socket
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>  // inet_pton
#include <unistd.h>     // close
#include <fcntl.h>      // fcntl

class ICMPUtil
{
private:
    /**
     *  Declaration of the variables
     */
    /* IP address and Ports */
    // Sending ICMP Package
    std::string localHostIP; // The IP address of the local host
    int localPort;           // The port of the local host
    std::string hostIP;      // The IP address of the host

    // Receving ICMP Package
    std::string sourceHostIP; // The IP address of the source host
    std::string destHostIP;   // The IP address of the destination host

    // Socket Variables
    int pingSocket;                  // socket for ICMP
    int on;                          // the flag for the socket
    int sendBufferSize;              // the size for sending buffer
    char *sendBuffer;                // the buffer for sending
    char *recvBuffer;                // the buffer for receiving
    struct timeval *tp;              // the timeval struct as timepstamp
    struct timeval waitingStartTP;   // the timeval struct as the start time for waiting
    struct timeval waitingEndTP;     // the timeval struct as the end time for waiting
    INT ret;                         // return results to determine the success of the operation
    struct sockaddr_in pingHostAddr; // the destination address to send ICMP packet
    struct sockaddr_in fromHostAddr; // the source address of the received packet
    int addrLen;                     // the length of the address
    int pingtimes;                   // the times for sending the ICMP packet
    bool flag;                       // the flag to determine whether it is the inital sending

    // Struct Headers
    struct ip *sendIPHeader;     // IP Header for sending
    struct icmp *sendICMPHeader; // ICMP Header for sending
    struct ip *recvIPHeader;     // IP Header for receiving
    struct icmp *recvICMPHeader; // ICMP Header for receiving

public:
    ICMPUtil(){};
    ICMPUtil(std::string hostIP, int localPort, std::string localHostIP);
    ~ICMPUtil(){};
    INT ping(int times);
};
#endif // PING_HPP