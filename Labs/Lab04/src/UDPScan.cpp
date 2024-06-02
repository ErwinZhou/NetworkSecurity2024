// Define the declared functions in the UDPScan.hpp
// To implement the UDP Scanning using UDP protocol

#include "UDPScan.hpp"

// The base function to UDP scan for the specific port
void UDPScanUtil::UDPScanHost(UDPScanHostThreadParam *param)
{
    /**
     * A non-thread function to scan for specific UDP port
     * Because there is no port specificed in ICMP reply
     * So it can't be implemented in mutil-thread, or it will confuse the port
     * @param param The parameter to scan for the specific port
     * @return void
     */
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

    // Get the parameters from the struct
    p = (struct UDPScanThreadParam *)param;
    hostIP = p->hostIP;
    beginPort = p->beginPort;
    endPort = p->endPort;
    localPort = p->localPort;
}
