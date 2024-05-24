// Define the declared functions in the TCPConnectScan.hpp
// To implement the TCP Connect Scanning using TCP protocol

#include "TCPConnectScan.hpp"

/* Mutil Thread Scanning functions for the TCP Connect Scanning */

// Scan on the specific port in this thread
void *TCPConnectScanUtil::Thread_TCPConnectHost(void *param)
{
    /**
     * The base function to TCP scan for the specific port
     * Using mutil-threadinng to improve the performance
     * @param param: The port number
     * @return: SUCCESS or FAILURE
     */
}
// Initialize the threads for scanning, calling upon the Thread_TCPConnectHost function
void *TCPConnectScanUtil::Thread_TCPConnectScan(void *param)
{
    /**
     * The advanced function to TCP scan for the range of ports, calling upon the Thread_TCPConnectHost function
     * Using mutil-threadinng to improve the performance
     * @param param: The port range
     * @return: SUCCESS or FAILURE
     */

    // Declare the variables
    struct TCPConnectThreadParam *p;
    std::string hostIP;
    int localPort;
    int beginPort;
    int tempPort; // The temporary port for scanning
    int endPort;
    pthread_attr_t attr;
    pthread_t childThreadID;
    int ret;

    // Get the parameters from the struct
    p = (struct TCPConnectThreadParam *)param;
    hostIP = p->hostIP;
    localPort = p->localPort;
    beginPort = p->beginPort;
    endPort = p->endPort;

    // Set the number of threads to 0
    TCPConnectThreadNum = 0;

    // Loop through the range of ports
    for (tempPort = beginPort; tempPort <= endPort; tempPort++)
    {
        // Set the parameters for the child thread
        TCPConnectHostThreadParam *pConHostParam = new TCPConnectHostThreadParam();
        pConHostParam->hostIP = hostIP;
        pConHostParam->localPort = localPort;
        pConHostParam->port = tempPort;

        // Set the child thread separated
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        // Create the child thread
        ret = pthread_create(&childThreadID, &attr, Thread_TCPConnectHost, (void *)pConHostParam);

        if (ret != 0)
            return (void *)ERROR;

        // Increase the number of threads
        pthread_mutex_lock(&TCPConnectThreadNumMutex);
        TCPConnectThreadNum++;
        pthread_mutex_unlock(&TCPConnectThreadNumMutex);
    }
}