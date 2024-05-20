// The constants, variables and declarations
// about the Ping Scanning  using ICMP protocol

#ifndef PING_HPP
#define PING_HPP

#include "def.hpp"
#include <string>       // std::string
#include <sys/socket.h> // socket
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>  // inet_pton
#include <unistd.h>     // close

class ICMPUtil
{
private:
public:
    ICMPUtil();
    ~ICMPUtil();
    bool ping(std::string hostIP, std::string localHostIP);
};
#endif // PING_HPP