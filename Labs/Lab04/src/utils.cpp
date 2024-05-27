// Definitions and implementations of the declared functions in utils.hpp

#include "utils.hpp"

// Compute the checksum of the headers
uint16_t in_cksum(uint16_t *addr, int len)
{
    /**
     * The function to compute the checksum of the headers
     * @param addr The address of the headers
     * @param len The length of the headers
     * @return The checksum of the headers
     */

    int nleft = len;
    int sum = 0;
    uint16_t *w = addr;
    uint16_t answer = 0;
    // Sum up 2-byte values until none or only one byte left.
    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }
    // Add leftover byte, if any
    if (nleft == 1)
    {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }
    // Fold 32-bit sum into 16 bits
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;
}

// Check if the input is a valid IPv4 address
bool isValidIPv4(const char *pszIPAddr)
{
    /**
     * The function to check if the input is a valid IPv4 address
     * @param pszIPAddr The input IP address
     * @return True if the input is a valid IPv4 address, otherwise false
     */
    if (!pszIPAddr)
        return false; // If pszIPAddr is null

    char cIP[4];
    int len = strlen(pszIPAddr);
    int n = 0, num = 0;

    const char *p = pszIPAddr;

    while (*p != '\0')
    {
        if (*p == ' ' || *p < '0' || *p > '9')
            return false; // If character is space or not a digit

        cIP[n++] = *p; // Save the first character of the segment to check for leading zeros

        int sum = 0; // The numeric value of the current segment, must be between 0 and 255
        while (*p != '.' && *p != '\0')
        {
            if (*p == ' ' || *p < '0' || *p > '9')
                return false;          // If character is space or not a digit
            sum = sum * 10 + *p - '0'; // Convert the segment string to an integer
            p++;
        }
        if (*p == '.')
        {
            // Check if there are digits before and after the dot
            if ((*(p - 1) >= '0' && *(p - 1) <= '9') && (*(p + 1) >= '0' && *(p + 1) <= '9'))
                num++; // Count the number of dots, must be exactly 3
            else
                return false;
        }
        if ((sum > 255) || (sum > 0 && cIP[0] == '0') || num > 3)
            return false; // Invalid if segment value is >255, starts with 0, or more than 3 dots

        if (*p != '\0')
            p++;
        n = 0; // Reset segment character index
    }
    if (num != 3)
        return false; // Must have exactly 3 dots
    return true;
}

// Check if the begin and end ports are valid for out of range
bool isValidPort(int beginPort, int endPort)
{
    /**
     * The function to check if the begin and end ports are valid for out of range
     * @param beginPort The begin port
     * @param endPort The end port
     * @return True if the begin and end ports are valid for out of range, otherwise false
     */
    if (beginPort < MIN_PORT || beginPort > MAX_PORT || endPort < MIN_PORT || endPort > MAX_PORT || beginPort > endPort)
        return false;
    return true;
}

// The log processing thread for the log message
void logProcessingThread(ThreadSafeQueue<LogMessage> &logQueue, int beginPort, int endPort)
{
    /**
     * The log processing thread for the log message
     * This thread makes sure that the log message is orderly and intactly printed according to the port number
     * @param logQueue The log queue
     * @param beginPort The begin port
     * @param endPort The end port
     */
    // Define the variables
    std::map<int, std::string> logMap;
    int expectedPort = beginPort;

    // The main loop
    while (true)
    {
        // Get one log message from the queue and store it in the map
        LogMessage log = logQueue.pop();
        logMap[log.port] = log.message;
        // Check the map to see if there is any expected log message
        while (logMap.count(expectedPort))
        {
            // Continusly print the log message until the expected port is not in the map yet
            std::cout << logMap[expectedPort] << std::endl;
            logMap.erase(expectedPort);
            expectedPort++;
        }

        if ((expectedPort > endPort) && logQueue.empty())
            // If the expected port is greater than the end port and the queue is empty, then break the loop
            break;
    }
}