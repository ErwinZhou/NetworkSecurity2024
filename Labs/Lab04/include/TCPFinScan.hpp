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
private:
    /**
     *  Declaration of the variables
     */
    /* TCP Fin Scanning Thread Parameters Struct */
    static int TCPFinThreadNum;
    static pthread_mutex_t TCPFinThreadNumMutex; // The mutex for the thread number
    static int errorStatus;                      // The global error status for the single thread
    static pthread_mutex_t errorStatusMutex;     // The mutex for the error status
    static ThreadSafeQueue<LogMessage> logQueue; // The log queue
    static pthread_mutex_t logQueueMutex;        // The mutex for the log queue

public:
    TCPFinScanUtil(){};
    ~TCPFinScanUtil(){};
    static void *Thread_TCPFinHost(void *param); // Scanning on the specific port in this thread
    static void *Thread_TCPFinScan(void *param); // Initialize the threads for scanning, calling upon the Thread_TCPFinHost function
};

#endif // TCP_FIN_SCAN_HPP