// The constants, variables and declarations about the TCP Fin Scan using the TCP Protocol

#ifndef TCP_FIN_SCAN_HPP
#define TCP_FIN_SCAN_HPP

#include "def.hpp"
#include "utils.hpp"
#include <string>       // std::string
#include <iostream>     // std::cerr
#include <pthread.h>    // pthread_t, pthread_mutex_t, pthread_create, pthread_exit, pthread_mutex_lock, pthread_mutex_unlock
#include <sys/socket.h> // socket, AF_INET, SOCK_STREAM
#include <arpa/inet.h>  // sockaddr_in, inet_addr, htons
#include <unistd.h>     // sleep
#include <sys/time.h>   // struct timeval
#include <fcntl.h>      //  fcntl, F_SETFL, O_NONBLOCK

class TCPFinScanUtil
{
};

#endif // TCP_FIN_SCAN_HPP