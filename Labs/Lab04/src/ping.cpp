// Define the declared functions in the ping.hpp
// To implement the ping scanning using ICMP protocol

#include "ping.hpp"

ICMPUtil::ICMPUtil(std::string hostIP, int localPort, std::string localHostIP) : hostIP(hostIP), localPort(localPort), localHostIP(localHostIP)
{
    /**
     * The constructor of the ICMPUtil class
     * @param hostIP The IP address of the host
     * @param localPort The port of the local host
     * @param localHostIP The IP address of the local host
     */
    recvBuffer = (char *)malloc(MAX_BUFFERS_SIZE);
    memset(recvBuffer, 0, MAX_BUFFERS_SIZE);
    pingtimes = 0;
    flag = false;
}

INT ICMPUtil::ping(int times)
{
    /**
     * The function to ping the host using ICMP protocol
     * @param times The times to ping the host
     *         0 for ping until timeout
     *         x(x>0) for ping x times
     * @return The ping results from the host:
     *         1 for success
     *         0 for failure
     *         -1 for error
     */

    // Create the raw socket
    pingSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    /* Set the socket relevant Settings, for the need to include and operate the IP header */
    on = 1;
    ret = setsockopt(pingSocket, 0, IP_HDRINCL, &on, sizeof(on));

    /**
     * Create the ICMP Echo Package
     * The ICMP Echo Package should include four parts to meet the standard ping package on Linux Platform:
     * 1. IP Header(20 bytes)
     * 2. ICMP Header(8 bytes)
     * 3. Timeval struct(16 bytes)
     * 4. Padding(40 bytes)
     * So size of the package is 56 bytes(84 bytes in total with the Header)
     */
    sendBufferSize = sizeof(struct ip) + sizeof(struct icmp) + sizeof(timeval) + 40;
    sendBuffer = (char *)malloc(sendBufferSize);
    memset(sendBuffer, 0, sendBufferSize);
    memset(recvBuffer, 0, MAX_BUFFERS_SIZE);
    // Print the PING message
    std::cout << "PING " << hostIP << " (" << hostIP << ") 56(84) bytes of data." << std::endl;

    // Set the IP Header
    sendIPHeader = (struct ip *)sendBuffer;
    sendIPHeader->ip_hl = 5;                                      // Header Length
    sendIPHeader->ip_v = 4;                                       // Version, 4 for IPv4
    sendIPHeader->ip_tos = 0;                                     // Type of Service, 0 for normal
    sendIPHeader->ip_len = htons(sendBufferSize);                 // Total Length
    sendIPHeader->ip_id = rand();                                 // Identification
    sendIPHeader->ip_ttl = 64;                                    // Time to Live
    sendIPHeader->ip_off = 0x40;                                  // Fragment Offset, 0x40 stands for no fragmentation
    sendIPHeader->ip_p = IPPROTO_ICMP;                            // Protocol ICMP
    sendIPHeader->ip_sum = 0;                                     // Checksum initially as 0
    sendIPHeader->ip_src.s_addr = inet_addr(localHostIP.c_str()); // Source IP
    sendIPHeader->ip_dst.s_addr = inet_addr(hostIP.c_str());      // Destination IP

    // Set the ICMP Header
    sendICMPHeader = (struct icmp *)(sendIPHeader + 1); // ICMP Header follows the IP Header
    sendICMPHeader->icmp_type = ICMP_ECHO;              // ICMP Echo Request
    sendICMPHeader->icmp_code = 0;                      // Code 0
    sendICMPHeader->icmp_cksum = 0;                     // Checksum initially as 0
    sendICMPHeader->icmp_id = htons(localPort);         // local port as the ID
    sendICMPHeader->icmp_seq = htons(1);                // Sequence Number usually starts from 1 like the standard ping command

    // Fill the 40 bytes of empty data with 0
    memset(sendBuffer + sizeof(struct ip) + sizeof(struct icmp), 0, 40);

    // Compute the checksum of the ICMP Header
    tp = (struct timeval *)(sendBuffer + sizeof(struct ip) + sizeof(struct icmp)); // Get the timeval struct
    gettimeofday(tp, NULL);                                                        // Get the current time
    sendICMPHeader->icmp_cksum = in_cksum((unsigned short *)sendICMPHeader, sizeof(struct icmp) + sizeof(struct timeval) + 40);

    // Set the sending address of the socket
    pingHostAddr.sin_family = AF_INET;                        // IPv4
    pingHostAddr.sin_addr.s_addr = inet_addr(hostIP.c_str()); // Convert the IP address to the network byte order
    addrLen = sizeof(pingHostAddr);

    // Send the ICMP Request
    if (sendto(pingSocket, sendBuffer, sendBufferSize, 0, (struct sockaddr *)&pingHostAddr, addrLen) == -1)
        return ERROR;

    /* Waiting for ICMP Reply */
    // Set the socket to non-block mode
    if (fcntl(pingSocket, F_SETFL, O_NONBLOCK) < 0)
        return ERROR;
    // Busy waiting for the ICMP Reply
    gettimeofday(&waitingStartTP, NULL);
    while (pingtimes < times || times == 0)
    {

        sleep(2); // Sleep for 2 seconds

        // Update the ICMP header sequence number and timestamp
        sendICMPHeader->icmp_seq = htons(pingtimes + 2);
        tp = (struct timeval *)(sendBuffer + sizeof(struct ip) + sizeof(struct icmp));
        gettimeofday(tp, NULL);

        // Recompute the checksum of the ICMP Header
        sendICMPHeader->icmp_cksum = 0;
        sendICMPHeader->icmp_cksum = in_cksum((unsigned short *)sendICMPHeader, sizeof(struct icmp) + sizeof(struct timeval) + 40);

        // Send the ICMP Request
        if (sendto(pingSocket, sendBuffer, sendBufferSize, 0, (struct sockaddr *)&pingHostAddr, addrLen) == -1)
            return ERROR;

        gettimeofday(&waitingStartTP, NULL);
        // Set the flag to false for avoiding sending another ICMP Package immediately without receving the last one
        flag = false;

        while (true)
        {
            ret = recvfrom(pingSocket, recvBuffer, MAX_BUFFERS_SIZE, 0, (struct sockaddr *)&fromHostAddr, (socklen_t *)&addrLen);
            if (ret > 0)
            {
                /**
                 *  If the recvfrom returns a positive value, one reply is received with its actual size
                 *  Then check the corresponding requirements:
                 *      (1) If the source IP is the same as the host IP
                 *      (2) If the destination IP is the same as the local host IP
                 *      (3) If the ICMP header with type code: ICMP_ECHOREPLY
                 */
                // Get its IP Header
                recvIPHeader = (struct ip *)recvBuffer;
                recvICMPHeader = (struct icmp *)(recvBuffer + (recvIPHeader->ip_hl << 2));
                // Get the source and destination IP
                sourceHostIP = inet_ntoa(recvIPHeader->ip_src);
                destHostIP = inet_ntoa(recvIPHeader->ip_dst);

                // Check the requirements
                if (sourceHostIP == hostIP &&
                    destHostIP == localHostIP &&
                    recvICMPHeader->icmp_type == ICMP_ECHOREPLY)
                {
                    gettimeofday(&waitingEndTP, NULL);
                    double elapsedTime = (waitingEndTP.tv_sec - waitingStartTP.tv_sec) * 1000.0;
                    elapsedTime += (waitingEndTP.tv_usec - waitingStartTP.tv_usec) / 1000.0;

                    std::cout << ret << " bytes from " << hostIP << ": icmp_seq=" << ntohs(recvICMPHeader->icmp_seq) << " ttl=" << (int)recvIPHeader->ip_ttl << " time=" << elapsedTime << " ms" << std::endl;
                    pingtimes++;
                    flag = true;
                    break;
                }
            }
            // If the control flow reaches here, due to the non-block mode of the socket
            // Check the time to avoid the infinite loop

            gettimeofday(&waitingEndTP, NULL);
            if ((1000000 *
                     (waitingEndTP.tv_sec - waitingStartTP.tv_sec) +
                 (waitingEndTP.tv_usec - waitingStartTP.tv_usec)) /
                    1000000.0 >
                MAX_TIMEOUT)
                return TIMEOUT;
        }
        if (flag == false)
        {
            sleep(1); // Sleep for a short while before sending next ICMP request
        }
    }

    // Clear the resources
    memset(sendBuffer, 0, sendBufferSize);
    free(sendBuffer);
    memset(recvBuffer, 0, MAX_BUFFERS_SIZE);
    free(recvBuffer);
    close(pingSocket);
    return SUCCESS;
}