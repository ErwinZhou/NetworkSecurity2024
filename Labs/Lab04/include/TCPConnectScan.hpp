// The constants, variables and declarations about the TCP Connect Scanning using TCP protocol

#ifndef TCP_CONNECT_SCAN_HPP
#define TCP_CONNECT_SCAN_HPP

#include "def.hpp"
#include "utils.hpp"
#include <string>       // std::string
#include <pthread.h>    // pthread_t, pthread_attr_t, pthread_create, pthread_join, pthread_attr_init, pthread_attr_destroy
#include <mutex>        // std::mutex
#include <unistd.h>     // sleep
#include <iostream>     // std::cout, std::endl
#include <sys/socket.h> // socket, AF_INET, SOCK_STREAM
#include <arpa/inet.h>  // sockaddr_in, inet_addr, htons

class TCPConnectScanUtil
{
private:
    /**
     *  Declaration of the variables
     */
    /* TCP Connect Scanning Thread Parameters Struct */
    static int TCPConnectThreadNum;
    static pthread_mutex_t TCPConnectThreadNumMutex; // The mutex for the thread number
    static int errorStatus;                          // The global error status for the single thread
    static pthread_mutex_t errorStatusMutex;         // The mutex for the error status
    static ThreadSafeQueue<LogMessage> logQueue;     // The log queue
public:
    TCPConnectScanUtil(){};
    ~TCPConnectScanUtil(){};
    // Mutil Thread Scanning
    static void *Thread_TCPConnectHost(void *param); // Scanning on the specific port in this thread
    static void *Thread_TCPConnectScan(void *param); // Initialize the threads for scanning, calling upon the Thread_TCPConnectHost function
};
#endif // TCP_CONNECT_SCAN_HPP