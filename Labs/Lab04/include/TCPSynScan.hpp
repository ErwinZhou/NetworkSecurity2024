// The constants, variables and declarations about the TCP Syn Scan using the TCP Protocol

#ifndef TCP_SYN_SCAN_HPP
#define TCP_SYN_SCAN_HPP

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
class TCPSynScanUtil
{
private:
    /**
     *  Declaration of the variables
     */
    /* TCP Syn Scanning Thread Parameters Struct */
    static int TCPSynThreadNum;
    static pthread_mutex_t TCPSynThreadNumMutex; // The mutex for the thread number
    static int errorStatus;                      // The global error status for the single thread
    static pthread_mutex_t errorStatusMutex;     // The mutex for the error status
    static ThreadSafeQueue<LogMessage> logQueue;

public:
    TCPSynScanUtil(){};
    ~TCPSynScanUtil(){};
    static void *Thread_TCPSynHost(void *param); // Scanning on the specific port in this thread
    static void *Thread_TCPSynScan(void *param); // Initialize the threads for scanning, calling upon the Thread_TCPSynHost function
};

#endif // TCP_SYN_SCAN_HPP