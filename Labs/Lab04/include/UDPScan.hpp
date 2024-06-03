// The constants, variables and declarations about the UDP Scanning using UDP protocol

#ifndef UDP_SCAN_HPP
#define UDP_SCAN_HPP

#include "def.hpp"
#include "utils.hpp"
#include <string>       // std::string
#include <iostream>     // std::cout, std::endl
#include <sys/socket.h> // socket, AF_INET, SOCK_RAM
#include <arpa/inet.h>  // sockaddr_in, inet_addr, htons
#include <fcntl.h>      // fcntl, F_SETFL, O_NONBLOCK
#include <sys/time.h>   // gettimeofday
#include <unistd.h>     // read

class UDPScanUtil
{
private:
    /**
     *  Declaration of the variables
     */
    // Socket Variables
    static int udpSocket; // socket for UDP
    static int on;        // the flag for the socket

public:
    UDPScanUtil(){};
    ~UDPScanUtil(){};
    static void *Thread_UDPScan(void *param);
    static INT UDPScanHost(UDPScanHostThreadParam *param);
};
#endif // UDP_SCAN_HPP