// Define the constants and macros for the global use

#ifndef DEF_HPP
#define DEF_HPP

// Include the important files for protocol struct
#include <netinet/ip.h>       // struct iphdr(Linux)
#include <netinet/tcp.h>      // struct tcphdr(Linux)
#include <netinet/udp.h>      // struct udphdr(Linux)
#include <netinet/ip_icmp.h>  // struct icmphdr(Linux), ip and icmp
#include <string>             // std::string
#include <mutex>              // std::mutex
#include <queue>              // std::queue
#include <map>                // std::map
#include <iostream>           // std::cout, std::endl
#include <condition_variable> // std::condition_variable

typedef int INT;

// Macros
#define MAX_PORT 65535
#define MIN_PORT 0
#define DEFAULT_LOCAL_HOST_IP "127.0.0.1"       // The local host IP address
#define DEFAULT_LOCAL_INET_IP "192.168.126.128" // The IPv4 address of the local host
#define DEFAULT_LOCAL_PORT 80                   // The local host port
#define DEFAULT_HOST_IP "127.0.0.1"
#define DEFAULT_HOST_TEST_IP "110.242.68.66" // 110.242.68.66 is the IPv4 address for www.baidu.com, which is used for testing
#define MAX_BUFFERS_SIZE 8192
#define MAX_PING_TIMEOUT 5
#define MAX_SYN_TIMEOUT 5
#define MAX_FIN_TIMEOUT 5
#define DEFAULT_PING_TIMES 3

// Arguments
#define ARG_HELP "-h"
#define ARG_PING "-p"
#define ARG_CONNECT_SCAN "-c"
#define ARG_SYN_SCAN "-s"
#define ARG_FIN_SCAN "-f"
#define ARG_UDP_SCAN "-u"

// Return flags
#define SUCCESS 1
#define FAILURE 0
#define ERROR -1
#define TIMEOUT -2

// Manually or Automatically input
#define MANUAL 0
#define AUTO 1

/* Class Declarations */
// Mutil-Thread Communication Class - ThreadSafeQuque
template <typename T>
class ThreadSafeQueue
{
private:
    /**
     * Basic variables for the thread-safe queue
     */
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cv;

public:
    /**
     * Operations for the thread-safe queue
     */
    // Push the value into the thread-safe queue
    void push(T value)
    {
        /**
         * Push the value into the thread-safe queue
         * @param value: The value to push
         */
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(std::move(value));
        cv.notify_one();
    }
    // Pop the value from the thread-safe queue
    T pop()
    {
        /**
         * Pop the value from the thread-safe queue
         * @return: The value popped
         */
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]
                { return !queue.empty(); });
        T value = std::move(queue.front());
        queue.pop();
        return value;
    }
    bool empty()
    {
        /**
         * Check if the queue is empty
         * @return: True if the queue is empty, otherwise False
         */
        std::lock_guard<std::mutex> lock(mtx);
        return queue.empty();
    }
};

/* Struct Definitions */
// Pseudo TCP Header
struct pseudohdr
{
    /**
     * This struct is used for the pseudo header of the TCP package
     * To calculate the checksum of the TCP package
     */
    uint32_t saddr;   // Source IP address
    uint32_t daddr;   // Destination IP address
    uint8_t useless;  // To fill the 8 bits of 0
    uint8_t protocol; // Protocol type
    uint16_t length;  // The length of the TCP package
};

// Mutil-Thread Communication Struct - LogMessage
struct LogMessage
{
    /**
     * The struct for storing the log message:
     * (1) The port number to output the log message in order
     * (2) The log message
     */
    int port;
    std::string message;
};

// TCP Connect Scan Struct
struct TCPConnectHostThreadParam
{
    /**
     * The struct for the parameters of the TCP Connect Scanning for the specific port
     */
    std::string hostIP; // The IP address of the host
    int port;           // The port for scanning
};

struct TCPConnectThreadParam
{
    /**
     * The struct for the parameters of the TCP Connect Scanning for the range of ports
     */
    std::string hostIP; // The IP address of the host
    int beginPort;      // The begin port for scanning
    int endPort;        // The end port for scanning
};

// TCP Syn Scan struct
struct TCPSynHostThreadParam
{
    /**
     * The struct for the parameters of the TCP Syn Scanning for the specific port
     */
    std::string hostIP;      // The IP address of the host
    int port;                // The port for scanning
    std::string localHostIP; // The IP address of the local host
    int localPort;           // The port of the local host
};

struct TCPSynThreadParam
{
    /**
     * The struct for the parameters of the TCP Syn Scanning for the range of ports
     */
    std::string hostIP;      // The IP address of the host
    std::string localHostIP; // The IP address of the local host
    int beginPort;           // The begin port for scanning
    int endPort;             // The end port for scanning
    int localPort;           // The port of the local host
};

// TCP Fin Scan struct
struct TCPFinHostThreadParam
{
    /**
     * The struct for the parameters of the TCP Fin Scanning for the specific port
     */
    std::string hostIP;      // The IP address of the host
    int port;                // The port for scanning
    std::string localHostIP; // The IP address of the local host
    int localPort;           // The port of the local host
};

struct TCPFinThreadParam
{
    /**
     * The struct for the parameters of the TCP Fin Scanning for the range of ports
     */
    std::string hostIP;      // The IP address of the host
    std::string localHostIP; // The IP address of the local host
    int beginPort;           // The begin port for scanning
    int endPort;             // The end port for scanning
    int localPort;           // The port of the local host
};

// TCP UDP Scan struct
struct UDPScanHostThreadParam
{
    /**
     * The struct for the parameters of the UDP Scanning for the specific port
     */
    std::string hostIP;      // The IP address of the host
    int port;                // The port for scanning
    std::string localHostIP; // The IP address of the local host
    int localPort;           // The port of the local host
};

struct UDPScanThreadParam
{
    /**
     * The struct for the parameters of the UDP Scanning for the range of ports
     */
    std::string hostIP;      // The IP address of the host
    std::string localHostIP; // The IP address of the local host
    int beginPort;           // The begin port for scanning
    int endPort;             // The end port for scanning
    int localPort;           // The port of the local host
};
#endif // DEF_HPP