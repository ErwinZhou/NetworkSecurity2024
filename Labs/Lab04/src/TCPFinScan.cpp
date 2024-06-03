// Define the declared functions in the TCPSFinScan.hpp
// To implement the TCP Fin Scanning using TCP protocol

#include "TCPFinScan.hpp"

int TCPFinScanUtil::TCPFinThreadNum;
pthread_mutex_t TCPFinScanUtil::TCPFinThreadNumMutex;
int TCPFinScanUtil::errorStatus;
pthread_mutex_t TCPFinScanUtil::errorStatusMutex;
ThreadSafeQueue<LogMessage> TCPFinScanUtil::logQueue;

// Scanning on the specific port in this thread
void *TCPFinScanUtil::Thread_TCPFinHost(void *param)
{
    /**
     * The function for scanning on the specific port in this thread
     * @param param The parameters for the scanning
     * @return whatever
     */
    // Declare the variables
    TCPFinHostThreadParam *p;
    std::string hostIP;
    int port;
    std::string localHostIP;
    int localPort;
    struct sockaddr_in FINScanHostAddress; // The host address
    int finSocket;                         // The socket for the FIN scanning
    INT ret;
    int len;
    std::string logMessage;
    char *sendBuffer;              // The buffer for sending the FIN package
    char *recvBuffer;              // The buffer for receiving the reply package
    struct pseudohdr *ptcph;       // The pseudo header for the TCP package
    struct tcphdr *tcph;           // The TCP header for the TCP package
    struct iphdr *recvIPHeader;    // The IP header for the received package
    struct tcphdr *recvTCPHeader;  // The TCP header for the received package
    std::string sourceHostIP;      // The IP address of the source host
    std::string destHostIP;        // The IP address of the destination host
    int sourcePort;                // The port of the source host
    int destPort;                  // The port of the destination host
    struct timeval waitingStartTP; // The start time point for waiting
    struct timeval waitingEndTP;   // The end time point for waiting

    // Get the parameters
    p = (TCPFinHostThreadParam *)param;
    hostIP = p->hostIP;
    port = p->port;
    localHostIP = p->localHostIP;
    localPort = p->localPort;

    // Create the fin scanning socket
    finSocket = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    // Check the socket
    if (finSocket < 0)
    {
        // Error
        std::cerr << "[ERROR] Failed to create the socket for the TCP Fin Scanning on ip address " << hostIP << " and port " << port << std::endl;
        pthread_mutex_lock(&errorStatusMutex);
        errorStatus = ERROR;
        pthread_mutex_unlock(&errorStatusMutex);

        // Clear the resources
        delete p;
        close(finSocket);
        memset(sendBuffer, 0, MAX_BUFFERS_SIZE);
        free(sendBuffer);
        memset(recvBuffer, 0, MAX_BUFFERS_SIZE);
        free(recvBuffer);
        // Decrease the thread number
        pthread_mutex_lock(&TCPFinThreadNumMutex);
        TCPFinThreadNum--;
        pthread_mutex_unlock(&TCPFinThreadNumMutex);

        pthread_exit((void *)ERROR);
    }

    // Set the host address
    memset(&FINScanHostAddress, 0, sizeof(FINScanHostAddress));
    FINScanHostAddress.sin_family = AF_INET;
    FINScanHostAddress.sin_port = htons(port);
    FINScanHostAddress.sin_addr.s_addr = inet_addr(hostIP.c_str());

    // Malloc the send and receive buffer
    sendBuffer = (char *)malloc(MAX_BUFFERS_SIZE);
    memset(sendBuffer, 0, MAX_BUFFERS_SIZE);
    recvBuffer = (char *)malloc(MAX_BUFFERS_SIZE);
    memset(recvBuffer, 0, MAX_BUFFERS_SIZE);

    // Memset the TCP FIN Package
    ptcph = (struct pseudohdr *)sendBuffer;
    tcph = (struct tcphdr *)(sendBuffer + sizeof(struct pseudohdr));

    // Memset the psedo TCP Header
    // To calculate the checksum of the TCP package
    ptcph->saddr = inet_addr(localHostIP.c_str());
    ptcph->daddr = inet_addr(hostIP.c_str());
    ptcph->useless = 0; // To fill the 8 bits of 0
    ptcph->protocol = IPPROTO_TCP;
    ptcph->length = htons(sizeof(struct tcphdr));

    // Memset the TCP Header
    tcph->th_sport = htons(localPort); // Source port
    tcph->th_dport = htons(port);      // Destination port
    tcph->th_seq = htons(123456);      // Sequence number
    tcph->th_ack = 0;                  // Acknowledgement number
    tcph->th_x2 = 0;                   // Unused
    tcph->th_off = 5;                  // Data offset
    tcph->th_flags = TH_FIN;           // Flags to set it as FIN
    tcph->th_win = htons(65535);       // Window size
    tcph->th_sum = 0;                  // Checksum initialized to 0
    tcph->th_urp = 0;                  // Urgent pointer

    // Compute the cksum using in_cksum
    // 12(Pseudo TCP Header) + 20(TCP Header)
    tcph->th_sum = in_cksum((unsigned short *)ptcph, 20 + 12);

    // Send the FIN package without the pseudo header
    len = sendto(finSocket, tcph, 20, 0, (struct sockaddr *)&FINScanHostAddress, sizeof(FINScanHostAddress));

    // Check the sendto
    if (len < 0)
    {
        // Error
        std::cerr << "[ERROR] Failed to send the FIN package for the TCP Fin Scanning on ip address " << hostIP << " and port " << port << std::endl;
        pthread_mutex_lock(&errorStatusMutex);
        errorStatus = ERROR;
        pthread_mutex_unlock(&errorStatusMutex);

        // Clear the resources
        delete p;
        close(finSocket);
        memset(sendBuffer, 0, MAX_BUFFERS_SIZE);
        free(sendBuffer);
        memset(recvBuffer, 0, MAX_BUFFERS_SIZE);
        free(recvBuffer);

        // Decrease the thread number
        pthread_mutex_lock(&TCPFinThreadNumMutex);
        TCPFinThreadNum--;
        pthread_mutex_unlock(&TCPFinThreadNumMutex);

        pthread_exit((void *)ERROR);
    }

    // Set the socket to non-block mode
    ret = fcntl(finSocket, F_SETFL, O_NONBLOCK);

    if (ret < 0)
    {
        // Error setting the socket to non-block mode
        std::cerr << "[ERROR] Failed to set the socket to non-block mode for the TCP Fin Scanning on ip address " << hostIP << " and port " << port << std::endl;
        pthread_mutex_lock(&errorStatusMutex);
        errorStatus = ERROR;
        pthread_mutex_unlock(&errorStatusMutex);

        // Clear the resources
        delete p;
        close(finSocket);
        memset(sendBuffer, 0, MAX_BUFFERS_SIZE);
        free(sendBuffer);
        memset(recvBuffer, 0, MAX_BUFFERS_SIZE);
        free(recvBuffer);

        // Decrease the thread number
        pthread_mutex_lock(&TCPFinThreadNumMutex);
        TCPFinThreadNum--;
        pthread_mutex_unlock(&TCPFinThreadNumMutex);

        pthread_exit((void *)ERROR);
    }

    // Busy waiting for the TCP Reply
    gettimeofday(&waitingStartTP, NULL);

    while (true)
    {
        // Take the recevied Package as a file descriptor, using ssize_t read (int fd, void *buf, size_t count)
        len = read(finSocket, recvBuffer, MAX_BUFFERS_SIZE);

        // Check the read results
        if (len > 0)
        {
            // If the control flow reaches here, the package is received
            /* Parse the received Package */
            // Get the IP Header and TCP Header
            recvIPHeader = (struct iphdr *)recvBuffer;
            recvTCPHeader = (struct tcphdr *)(recvBuffer + (recvIPHeader->ihl << 2));

            // Get the source and destination port
            sourcePort = ntohs(recvTCPHeader->source);
            destPort = ntohs(recvTCPHeader->dest);
            /**
             * Check if the requirements are met:
             * (1) Check if the source/dest IP and Port matches
             * (2) Check if the package type if RST
             */
            // Check if the source/dest IP and Port matches
            if (sourceHostIP == hostIP && destHostIP == localHostIP && sourcePort == port && destPort == localPort)
            {
                // // Debug
                // std::cout << "[INFO] th_flags: 0x" << std::hex << static_cast<int>(recvTCPHeader->th_flags) << std::dec << std::endl;
                if ((recvTCPHeader->th_flags & TH_RST) == TH_RST)
                {
                    // If the received package type includes TH_RST, the port is closed
                    logMessage = "[INFO] Host: " + hostIP + " Port: " + std::to_string(port) + " closed!";
                    break;
                }
            }
        }

        // If the control flow reaches here, due to the non-block mode of the socket
        // Check the time to avoid the infinite loop
        gettimeofday(&waitingEndTP, NULL);
        if ((1000000 *
                 (waitingEndTP.tv_sec - waitingStartTP.tv_sec) +
             (waitingEndTP.tv_usec - waitingStartTP.tv_usec)) /
                1000000.0 >
            MAX_FIN_TIMEOUT)
        {
            // Timeout without receiving the RST package
            // But due to the previous success ping results, the port is OPEN without any reply
            logMessage = "[INFO] Host: " + hostIP + " Port: " + std::to_string(port) + " open!";
            break;
        }
    }

    // Finish sending and reading socket SYN Package, free the resources
    delete p;
    close(finSocket);
    memset(sendBuffer, 0, MAX_BUFFERS_SIZE);
    free(sendBuffer);
    memset(recvBuffer, 0, MAX_BUFFERS_SIZE);
    free(recvBuffer);

    // Push the log message to the queue
    LogMessage log = {port, logMessage};
    logQueue.push(log);

    // Decrease the thread number
    pthread_mutex_lock(&TCPFinThreadNumMutex);
    TCPFinThreadNum--;
    pthread_mutex_unlock(&TCPFinThreadNumMutex);

    // The thread exits
    pthread_exit(NULL);
}

// Initialize the threads for scanning, calling upon the Thread_TCPFinHost function
void *TCPFinScanUtil::Thread_TCPFinScan(void *param)
{
    /**
     * The function for initializing the threads for scanning, calling upon the Thread_TCPFinHost function
     * @param param The parameters for the scanning
     * @return SUCCESS or ERROR
     */

    // Declare the variables
    std::string hostIP;
    int beginPort;
    int endPort;
    std::string localHostIP;
    int localPort;
    TCPFinThreadParam *p; // The parameters for the scanning
    int tempPort;         // The temporary port for the scanning
    INT ret;
    pthread_t loggerThreadID; // The thread ID for the logger thread
    pthread_attr_t attr;
    pthread_t childThreadID; // The thread ID for the child thread
    int scanFailureFlag;

    // Get the parameters
    p = (TCPFinThreadParam *)param;
    hostIP = p->hostIP;
    beginPort = p->beginPort;
    endPort = p->endPort;
    localHostIP = p->localHostIP;
    localPort = p->localPort;

    // Initialize the variables
    TCPFinThreadNum = 0;
    errorStatus = SUCCESS;
    scanFailureFlag = false;

    std::cout << "[INFO] Begin TCP fin scan..." << std::endl;

    // Create the log processing thread
    ret = pthread_create(&loggerThreadID, NULL, [](void *arg) -> void *
                         {
        auto* args = static_cast<std::tuple<ThreadSafeQueue<LogMessage>*, int, int>*>(arg);
        logProcessingThread(*std::get<0>(*args), std::get<1>(*args), std::get<2>(*args));
        delete args;
        return nullptr; }, new std::tuple<ThreadSafeQueue<LogMessage> *, int, int>(&logQueue, beginPort, endPort));

    if (ret != 0)
    {
        std::cerr << "[ERROR] Failed to create logger thread for the TCP Fin Scanning on ip address " << hostIP << std::endl;
        delete p;
        pthread_exit((void *)ERROR);
    }

    // Loop through the range of ports
    for (tempPort = beginPort; tempPort <= endPort; tempPort++)
    {
        // Get and store in the new parameter struct
        TCPFinHostThreadParam *pFinHostParam = new TCPFinHostThreadParam;
        pFinHostParam->hostIP = hostIP;
        pFinHostParam->port = tempPort;
        pFinHostParam->localHostIP = localHostIP;
        pFinHostParam->localPort = localPort;

        // Set the thread attributes to detached
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        // Create the child thread
        ret = pthread_create(&childThreadID, &attr, Thread_TCPFinHost, (void *)pFinHostParam);

        if (ret != 0)
        {
            std::cerr << "[ERROR] Failed to create the thread for the TCP Fin Scanning on ip address " << hostIP << " and port " << tempPort << std::endl;
            scanFailureFlag = true;
            break;
        }

        // Increment the thread number
        pthread_mutex_lock(&TCPFinThreadNumMutex);
        TCPFinThreadNum++;
        pthread_mutex_unlock(&TCPFinThreadNumMutex);

        // Make sure there are not too many threads
        while (TCPFinThreadNum >= 100)
            sleep(3);
    }

    // If the control reaches here, the scanning is done
    while (TCPFinThreadNum != 0)
    {
        // Check the error status
        pthread_mutex_lock(&errorStatusMutex);
        if (errorStatus == ERROR)
        {
            // Error
            std::cerr << "[ERROR] Failed to scan the TCP Fin on the ip address " << hostIP << " and port " << tempPort << std::endl;
            scanFailureFlag = true;
        }
        pthread_mutex_unlock(&errorStatusMutex);
        sleep(1);
    }

    // Wait for the log thread to finish
    pthread_join(loggerThreadID, NULL);

    // The main thread exits
    if (scanFailureFlag)
        pthread_exit((void *)ERROR);
    else
        pthread_exit((void *)SUCCESS);
}