// Define the declared functions in the UDPScan.hpp
// To implement the UDP Scanning using UDP protocol

#include "UDPScan.hpp"

int UDPScanUtil::udpSocket; // socket for UDP
int UDPScanUtil::on;        // the flag for the socket

// The base function to UDP scan for the specific port
INT UDPScanUtil::UDPScanHost(UDPScanHostThreadParam *param)
{
    /**
     * A non-thread function to scan for specific UDP port
     * Because there is no port specificed in ICMP reply
     * So it can't be implemented in mutil-thread, or it will confuse the port
     * @param param The parameter to scan for the specific port
     * @return SUCCESS or ERROR
     */
    // Declare the local variables
    UDPScanHostThreadParam *p;          // The parameter to scan for the specific port
    std::string hostIP;                 // The IP address of the host
    int port;                           // The port for scanning
    std::string localHostIP;            // The IP address of the local host
    int localPort;                      // The port of the local host
    struct sockaddr_in UDPScanHostAddr; // The destination address to send UDP packet
    int addrLen;                        // The length of the address
    INT ret;                            // The return value
    int len;                            // The length of the packet

    struct iphdr *ipHeader;         // The IP header
    struct udphdr *udpHeader;       // The UDP header
    struct pseudohdr *pseudoHeader; // The pseudo header
    char *packet;                   // The packet to send
    int packetSize;                 // The size of the packet

    struct timeval waitingStartTP; // The start time to wait for the response
    struct timeval waitingEndTP;   // The end time to wait for the response

    struct ipicmphdr *recvHeader; // The header of the received ICMP packet
    struct icmphdr *icmpHeader;   // The ICMP header of the received ICMP packet

    // Get the parameters from the struct
    p = (UDPScanHostThreadParam *)param;
    hostIP = p->hostIP;
    port = p->port;
    localHostIP = p->localHostIP;
    localPort = p->localPort;

    // Set the addr of the UDP socket
    memset(&UDPScanHostAddr, 0, sizeof(UDPScanHostAddr));
    UDPScanHostAddr.sin_family = AF_INET;
    UDPScanHostAddr.sin_port = htons(port);
    UDPScanHostAddr.sin_addr.s_addr = inet_addr(hostIP.c_str());
    addrLen = sizeof(UDPScanHostAddr);

    // Memset the UDP packet
    packetSize = sizeof(struct iphdr) + sizeof(struct udphdr);
    packet = (char *)malloc(packetSize);
    memset(packet, 0, packetSize);

    /**
     * Create the UDP Packet
     * The IUDP Package should include two parts to meet the standard UDP package on Linux Platform:
     * 1. IP Header
     * 2. UDP Header
     * Because the pseudo header is only used to calculate the checksum of the udp
     * So there is no need to include the pseudo header in the packet
     */
    // Create the UDP Packet
    ipHeader = (struct iphdr *)packet;
    udpHeader = (struct udphdr *)(packet + sizeof(struct iphdr));
    pseudoHeader = (struct pseudohdr *)(packet + sizeof(struct iphdr) - sizeof(struct pseudohdr));

    // Memset the UDP Header
    udpHeader->source = htons(localPort);          // The source port is the local port
    udpHeader->dest = htons(port);                 // The destination port is the scanning port
    udpHeader->len = htons(sizeof(struct udphdr)); // The length of the UDP header
    udpHeader->check = 0;                          // The checksum of the UDP header initially set to 0

    // Memset the Pseudo UDP Header, to calculate the checksum of the UDP packet
    pseudoHeader->saddr = inet_addr(localHostIP.c_str()); // The source IP address
    pseudoHeader->daddr = inet_addr(hostIP.c_str());      // The destination IP address
    pseudoHeader->useless = 0;                            // To fill the 8 bits of 0
    pseudoHeader->protocol = IPPROTO_UDP;                 // The protocol type is UDP
    pseudoHeader->length = udpHeader->len;                // The length of the UDP package

    // Calculate the checksum of the UDP packet
    udpHeader->check = in_cksum((unsigned short *)pseudoHeader, sizeof(struct udphdr) + sizeof(struct pseudohdr));

    // Memset the IP Header
    ipHeader->ihl = 5;                                // Header Length
    ipHeader->version = 4;                            // Version, 4 for IPv4
    ipHeader->tos = 0x10;                             // Type of Service, 0x10 for IPTOS_LOWDELAY
    ipHeader->tot_len = htons(packetSize);            // Total Length is the size of the packet
    ipHeader->id = htons(54321);                      // Identification
    ipHeader->frag_off = 0;                           // Fragment Offset, 0 stands for no fragmentation
    ipHeader->ttl = 69;                               // Time to Live
    ipHeader->protocol = IPPROTO_UDP;                 // Protocol UDP
    ipHeader->check = 0;                              // Checksum initially as 0
    ipHeader->saddr = inet_addr(localHostIP.c_str()); // Source IP
    ipHeader->daddr = inet_addr(hostIP.c_str());      // Destination IP

    // Calculate the IP header checksum
    ipHeader->check = in_cksum((unsigned short *)ipHeader, sizeof(struct iphdr));

    // Send the UDP packet
    len = sendto(udpSocket, packet, packetSize, 0, (struct sockaddr *)&UDPScanHostAddr, addrLen);

    // Check the sending length
    if (len < 0)
    {
        // Error
        std::cerr << "[ERROR] Failed to send the UDP package for the UDP Scanning on ip address " << hostIP << " and port " << port << std::endl;
        // Clear the resources
        delete p;
        memset(packet, 0, packetSize);
        free(packet);
        return ERROR;
    }

    /* Waiting for ICMP Packet */
    // Set the socket to non-blocking mode
    ret = fcntl(udpSocket, F_SETFL, O_NONBLOCK);
    // Check the return value
    if (ret < 0)
    {
        // Error
        std::cerr << "[ERROR] Failed to set the socket to non-blocking mode for UDP Scanning on ip address " << hostIP << " and port " << port << std::endl;
        // Clear the resources
        delete p;
        memset(packet, 0, packetSize);
        free(packet);
        return ERROR;
    }

    // Set the waiting time
    gettimeofday(&waitingStartTP, NULL);
    // Busy waiting for the ICMP Reply
    while (true)
    {
        // Receive the ICMP Packet
        len = read(udpSocket, (struct ipicmphdr *)&recvHeader, sizeof(recvHeader));
        if (len > 0)
        {
            /**
             *  If the recvfrom returns a positive value, one reply is received with its actual size
             *  Then check the corresponding requirements:
             *      (1) If the source IP is the same as the host IP
             *      (2) If the destination IP is the same as the local host IP
             *      (3) If the ICMP header with code: ICMP_DEST_UNREACH 3
             *      (4) If the ICMP header with type: ICMP_PORT_UNREACH 3
             */
            if (
                recvHeader->ipHeader.saddr == inet_addr(hostIP.c_str()) &&
                recvHeader->ipHeader.daddr == inet_addr(localHostIP.c_str()) &&
                recvHeader->icmpHeader.code == ICMP_DEST_UNREACH &&
                recvHeader->icmpHeader.type == ICMP_PORT_UNREACH)
            {
                // Output the log message
                std::cout << "[INFO] Host: " + hostIP + " Port: " + std::to_string(port) + " closed!" << std::endl;
                break;
            }
        }

        // If the control flow reaches here, due to the non - block mode of the socket
        // Check the time to avoid the infinite loop
        gettimeofday(&waitingEndTP, NULL);
        if ((1000000 *
                 (waitingEndTP.tv_sec - waitingStartTP.tv_sec) +
             (waitingEndTP.tv_usec - waitingStartTP.tv_usec)) /
                1000000.0 >
            MAX_UDP_TIMEOUT)
        {
            // Timeout without receiving the ICMP Packet
            // But due to the previous success ping results, the port is OPEN without any reply
            std::cout << "[INFO] Host: " + hostIP + " Port: " + std::to_string(port) + " open or filerted!" << std::endl;
            break;
        }
    }

    // Clear the resources
    delete p;
    memset(packet, 0, packetSize);
    free(packet);
    return SUCCESS;
}

// The thread function used to scan for a range of ports
// by calling upon UDPScanHost on each port
void *UDPScanUtil::Thread_UDPScan(void *param)
{
    /**
     * A thread function to scan for a range of ports
     * @param param The parameter to scan for the range of ports
     * @return SUCCESS or ERROR
     */
    // Declare the variables
    struct UDPScanThreadParam *p;
    std::string hostIP;
    int port;
    int localPort;
    int beginPort;
    int tempPort; // The temporary port for scanning
    int endPort;
    INT ret;
    bool scanFailureFlag;

    // Get the parameters from the struct
    p = (struct UDPScanThreadParam *)param;
    hostIP = p->hostIP;
    beginPort = p->beginPort;
    endPort = p->endPort;
    localPort = p->localPort;

    std::cout << "[INFO] Begin UDP scan..." << std::endl;

    /**
     * Initializtions before scanning
     */
    scanFailureFlag = false;                             // Set the flag to false
    udpSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP); // Create the socket using SOCK_RAW and ICMP protocol

    // Set the socket relevant Settings, for the need to include and operate the IP header
    on = 1;
    ret = setsockopt(udpSocket, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on));

    // Check the return value
    if (ret == ERROR)
    {
        std::cout << "[ERROR] Failed to set the socket to non-blocking mode for UDP scanning on ip address " << hostIP << std::endl;
        pthread_exit((void *)ERROR);
    }

    // The loop to scan for the range of ports
    for (tempPort = beginPort; tempPort <= endPort; tempPort++)
    {
        // Get and store in the new parameter struct
        UDPScanHostThreadParam *pUDPScanHostParam = new UDPScanHostThreadParam();
        pUDPScanHostParam->hostIP = hostIP;
        pUDPScanHostParam->port = tempPort;
        pUDPScanHostParam->localHostIP = p->localHostIP;
        pUDPScanHostParam->localPort = localPort;

        // Instead of implementing in multi-thread, call the function directly
        ret = UDPScanUtil::UDPScanHost(pUDPScanHostParam);

        // Check the return value
        if (ret == ERROR)
            // Set the flag to true if there is an error without stopping to finish the rest
            scanFailureFlag = true;
    }

    // Check the flag
    if (scanFailureFlag)
        pthread_exit((void *)ERROR);
    else
        pthread_exit((void *)SUCCESS);
}
