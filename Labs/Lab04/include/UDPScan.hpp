// The constants, variables and declarations about the UDP Scanning using UDP protocol

#ifndef UDP_SCAN_HPP
#define UDP_SCAN_HPP

#include "def.hpp"
#include "utils.hpp"
#include <string>       // std::string
#include <iostream>     // std::cout, std::endl
#include <sys/socket.h> // socket, AF_INET, SOCK_RAM
#include <arpa/inet.h>  // sockaddr_in, inet_addr, htons

class UDPScanUtil
{
private:
public:
    void *Thread_UDPScan(void *param);
    void UDPScanHost(UDPScanHostThreadParam *param);
};
#endif // UDP_SCAN_HPP