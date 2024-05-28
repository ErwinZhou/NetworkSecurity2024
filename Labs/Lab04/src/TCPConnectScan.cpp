// Define the declared functions in the TCPConnectScan.hpp
// To implement the TCP Connect Scanning using TCP protocol

#include "TCPConnectScan.hpp"

int TCPConnectScanUtil::TCPConnectThreadNum;
pthread_mutex_t TCPConnectScanUtil::TCPConnectThreadNumMutex;
int TCPConnectScanUtil::errorStatus;
pthread_mutex_t TCPConnectScanUtil::errorStatusMutex;
ThreadSafeQueue<LogMessage> TCPConnectScanUtil::logQueue;
/* Mutil Thread Scanning functions for the TCP Connect Scanning */

// Scan on the specific port in this thread
void *TCPConnectScanUtil::Thread_TCPConnectHost(void *param)
{
    /**
     * The base function to TCP scan for the specific port
     * Using mutil-threadinng to improve the performance
     * @param param: The port number
     * @return: WHATEVER, due to the detached state of the threads
     */
    // Declare the variables
    struct TCPConnectHostThreadParam *p;
    std::string hostIP;
    int localPort;
    int port;
    int connectSocket;
    struct sockaddr_in hostAddress;
    INT ret;
    std::string logMessage;

    // Get the parameters from the struct
    p = (struct TCPConnectHostThreadParam *)param;
    hostIP = p->hostIP;
    port = p->port;

    // Create socket
    connectSocket = socket(AF_INET, SOCK_STREAM, 0);
    // Check the socket
    if (connectSocket < 0)
    {
        // Error
        std::cerr << "[ERROR] Failed to create the socket for the TCP Connect Scanning on ip address " << hostIP << " and port " << port << std::endl;
        pthread_mutex_lock(&TCPConnectThreadNumMutex);
        errorStatus = ERROR;
        pthread_mutex_unlock(&errorStatusMutex);

        // Clear the resources
        delete p;
        close(connectSocket);
        // Decrease the number of threads
        pthread_mutex_lock(&TCPConnectThreadNumMutex);
        TCPConnectThreadNum--;
        pthread_mutex_unlock(&TCPConnectThreadNumMutex);

        pthread_exit((void *)ERROR);
    }

    // Set the host address
    memset(&hostAddress, 0, sizeof(hostAddress));
    hostAddress.sin_family = AF_INET;
    hostAddress.sin_port = htons(port);
    hostAddress.sin_addr.s_addr = inet_addr(hostIP.c_str());

    // Connect to the host
    ret = connect(connectSocket, (struct sockaddr *)&hostAddress, sizeof(hostAddress));

    // Check the connection
    if (ret < 0)
    {
        // Error
        if (errno == ECONNREFUSED)
            // When the connection is refused, the port is closed or filtered, but it can't to determine which one
            // std::cout << "[INFO] Host: " << hostIP << " Port: " << port << " closed or filtered!" << std::endl;
            logMessage = "[INFO] Host: " + hostIP + " Port: " + std::to_string(port) + " closed or filtered!";
        else
        {
            pthread_mutex_lock(&TCPConnectThreadNumMutex);
            errorStatus = ERROR;
            pthread_mutex_unlock(&errorStatusMutex);
            // std::cerr << "[ERROR] Failed to connect to host: " << hostIP << " Port: " << port << ". Error: " << strerror(errno) << std::endl;
            logMessage = "[ERROR] Failed to connect to host: " + hostIP + " Port: " + std::to_string(port) + ". Error: " + strerror(errno);
        }
    }
    else
        // std::cout << "[INFO] Host: " << hostIP << " Port: " << port << " open!" << std::endl;
        logMessage = "[INFO] Host: " + hostIP + " Port: " + std::to_string(port) + " open!";

    // Clear the resources
    delete p;
    close(connectSocket);

    // Push the log message to the queue
    LogMessage log = {port, logMessage};
    logQueue.push(log);

    // Decrease the number of threads
    pthread_mutex_lock(&TCPConnectThreadNumMutex);
    TCPConnectThreadNum--;
    pthread_mutex_unlock(&TCPConnectThreadNumMutex);

    // Exit the thread
    pthread_exit(NULL);

    std::cout << "Thread_TCPConnectHost Bye Bye" << std::endl;
}

// Initialize the threads for scanning, calling upon the Thread_TCPConnectHost function
void *TCPConnectScanUtil::Thread_TCPConnectScan(void *param)
{
    /**
     * The advanced function to TCP scan for the range of ports, calling upon the Thread_TCPConnectHost function
     * Using mutil-threadinng to improve the performance
     * @param param: The port range
     * @return: SUCCESS or ERROR
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
    pthread_t loggerThreadID;
    INT ret;
    int scanFailureFlag;

    // Get the parameters from the struct
    p = (struct TCPConnectThreadParam *)param;
    hostIP = p->hostIP;
    beginPort = p->beginPort;
    endPort = p->endPort;

    // Set the number of threads to 0
    TCPConnectThreadNum = 0;
    errorStatus = SUCCESS;
    scanFailureFlag = false;

    std::cout << "[INFO] Begin TCP connect scan..." << std::endl;

    // Create the log processing thread
    ret = pthread_create(&loggerThreadID, NULL, [](void *arg) -> void *
                         {
        auto* args = static_cast<std::tuple<ThreadSafeQueue<LogMessage>*, int, int>*>(arg);
        logProcessingThread(*std::get<0>(*args), std::get<1>(*args), std::get<2>(*args));
        delete args;
        return nullptr; }, new std::tuple<ThreadSafeQueue<LogMessage> *, int, int>(&logQueue, beginPort, endPort));

    if (ret != 0)
    {
        std::cerr << "[ERROR] Failed to create logger thread" << std::endl;
        pthread_exit((void *)ERROR);
    }

    // Loop through the range of ports
    for (tempPort = beginPort; tempPort <= endPort; tempPort++)
    {

        // Set the parameters for the child thread
        TCPConnectHostThreadParam *pConHostParam = new TCPConnectHostThreadParam();
        pConHostParam->hostIP = hostIP;
        pConHostParam->port = tempPort;

        // Set the child thread separated
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        // Create the child thread
        ret = pthread_create(&childThreadID, &attr, Thread_TCPConnectHost, (void *)pConHostParam);

        if (ret != 0)
        {
            // Error
            std::cerr << "[ERROR] Failed to create the thread for the TCP Connect Scanning on ip address " << hostIP << " and port " << tempPort << std::endl;
            scanFailureFlag = true;
            break;
        }

        // Increase the number of threads
        pthread_mutex_lock(&TCPConnectThreadNumMutex);
        TCPConnectThreadNum++;
        pthread_mutex_unlock(&TCPConnectThreadNumMutex);
        // In order to avoid too many threads, we need to wait for a while
        while (TCPConnectThreadNum > 100)
            sleep(3);
    }

    // If the control flow reaches here, the scanning of the range of ports has been successful assigned to the threads
    // Wait for the threads to finish
    while (TCPConnectThreadNum != 0)
    {
        // Check the error status
        pthread_mutex_lock(&errorStatusMutex);
        if (errorStatus == ERROR)
        {
            // Error
            pthread_mutex_unlock(&errorStatusMutex);
            scanFailureFlag = true;
        }
        pthread_mutex_unlock(&errorStatusMutex);
        sleep(1);
    }
    // Wait for the logger thread to finish
    pthread_join(loggerThreadID, NULL);

    // The main thread exits
    if (scanFailureFlag)
        // Error
        pthread_exit((void *)ERROR);
    else
        // Success
        pthread_exit((void *)SUCCESS);
}