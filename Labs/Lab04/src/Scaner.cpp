// The main function of the program: Parse the arguments and Execute the scaner

#include "Scaner.hpp"

INT ping(std::string ip, int times)
{
    /**
     * @brief Ping the target IP address to check if it is alive
     * @param ip the target IP address
     * @param times the times to ping the target IP address
     * @return The ping results from the host:
     *         SUCCESS(1) for success
     *         FAILURE(0) for failure
     *         ERROR(-1) for error
     *         TIMEOUT(-2) for timeout
     */

    // Ping the target IP address
    ICMPUtil icmpUtil(ip, DEFAULT_LOCAL_PORT, DEFAULT_LOCAL_INET_IP);
    std::cout << "----------------------------- Ping Target IP Address ------------------------------" << std::endl;
    INT ret = icmpUtil.ping(times);
    if (ret == SUCCESS)
    {
        std::cout << "[INFO] Ping the target IP address " << ip << " successfully " << std::endl;
        std::cout
            << "---------------------------------------------------------------------------" << std::endl;
        return SUCCESS;
    }
    else if (ret == FAILURE)
    {
        std::cout << "[INFO] The target IP address " << ip << " is unreachable" << std::endl;
        std::cout
            << "---------------------------------------------------------------------------" << std::endl;
        return FAILURE;
    }
    else if (ret == ERROR)
    {
        std::cerr << "[ERROR] Ping the target IP address " << ip << " FAILED" << std::endl;
        std::cout
            << "---------------------------------------------------------------------------" << std::endl;
        return ERROR;
    }
    else if (ret == TIMEOUT)
    {
        std::cerr << "[ERROR] Ping the target IP address " << ip << " TIMEOUT" << std::endl;
        std::cout
            << "---------------------------------------------------------------------------" << std::endl;
        return TIMEOUT;
    }

    return SUCCESS;
}

inline void help(const char *program)
{
    /**
     * @brief Print the help message to instruct the user how to use the program
     * @param program the name of the program
     * @return void
     */
    // Help Info
    std::cerr << "------------------------------ Scaner Help Info ------------------------------" << std::endl;

    std::string programPath(program);
    size_t pos = programPath.find_last_of("/\\");
    std::string programName = programPath.substr(pos + 1);

    std::cerr << "Usage: sudo ./" << programName << " [-Option] <target address> {additional parameters}" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  [-h] --help" << std::endl;
    std::cerr << "  [-p] <target address> {times}\n"
              << "       --ping the <target address> for {times} times\n"
              << "       --if {times} equals 0, it stands for ping until timeout\n"
              << "       --if {times} is empty, it stands for ping once" << std::endl;
    std::cerr << "  [-c] <target address> {begin port} {end port}\n"
              << "       --TCP Connect scan at the <target address> from <begin port> to <end port>\n"
              << "       --{begin port} and {end port} can be empty for maually input" << std::endl;
    std::cerr << "  [-s] --TCP Syn scan\n"
              << "       --TCP Syn scan at the <target address> from <begin port> to <end port>\n"
              << "       --{begin port} and {end port} can be empty for maually input" << std::endl;
    std::cerr << "  [-f] --TCP fin scan\n"
              << "       --TCP Fin scan at the <target address> from <begin port> to <end port>\n"
              << "       --{begin port} and {end port} can be empty for maually input" << std::endl;
    std::cerr << "  [-u] --UDP scan\n"
              << "       --UDP scan at the <target address> from <begin port> to <end port>\n"
              << "       --{begin port} and {end port} can be empty for maually input" << std::endl;

    std::cerr
        << "---------------------------------------------------------------------------" << std::endl;
    return;
}

inline INT TCPConnectScan(std::string ip, int beginPort, int endPort, INT mode)
{
    /**
     * @brief TCP Connect scan at the target address from the begin port to the end port
     * @param ip the target IP address
     * @param beginPort the begin port
     * @param endPort the end port
     * @param mode the mode to input the begin port and end port
     *             MANUAL(0) for manually input
     *             AUTO(1) for automatically input
     */
    // Declare the variables
    INT ret;
    pthread_attr_t attr;
    pthread_t childThreadID;
    struct TCPConnectThreadParam param;
    int status;

    std::cout << "----------------------------- TCP Connect Scan ------------------------------" << std::endl;
    if (mode == MANUAL)
    {
        // If the mode is manual, input the begin port and end port
        std::cout << "[INFO] Please input the range of port(0-65535)..." << std::endl;
        std::cout << "Begin port: ";
        std::cin >> beginPort;
        std::cout << "End port: ";
        std::cin >> endPort;
    }
    /* mode == AUTO */
    // If the mode if AUTO, it means that the port range has already been given by the CML arguments
    // Ping the target IP address first to make sure it is alive
    ret = ping(ip, DEFAULT_PING_TIMES);
    // If the ping is failed, return the error directly and stop the scanning
    if (ret == FAILURE || ret == ERROR || ret == TIMEOUT)
        return ret;

    // Begin the TCP Connect Scan
    std::cout << "[INFO] TCP Connect Scan Host " << ip << " port " << beginPort << "~" << endPort << "..." << std::endl;

    // Assign the struct for the parameters
    param.hostIP = ip;
    param.beginPort = beginPort;
    param.endPort = endPort;

    // Initialize the threads for scanning, calling upon the Thread_TCPConnectScan function
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Create the child thread
    ret = pthread_create(&childThreadID, &attr, TCPConnectScanUtil::Thread_TCPConnectScan, (void *)&param);

    // Check if the thread is created successfully
    if (ret != 0)
    {
        // Error
        std::cerr << "[ERROR] Failed to create the thread for the TCP Connect Scanning on ip address " << ip << " and port " << beginPort << "~" << endPort << std::endl;
        return FAILURE;
    }

    // Wait for the child thread to finish
    pthread_join(childThreadID, (void **)&status);

    // Check the status of the thread
    if (status == SUCCESS)
    {
        std::cout << "[INFO] TCP Connect Scan Host " << ip << " port " << beginPort << "~" << endPort << " successfully" << std::endl;
        std::cout << "---------------------------------------------------------------------------" << std::endl;
        return SUCCESS;
    }
    else
    {
        std::cerr << "[ERROR] TCP Connect Scan Host " << ip << " port " << beginPort << "~" << endPort << " FAILED" << std::endl;
        std::cout << "---------------------------------------------------------------------------" << std::endl;
        return FAILURE;
    }
    return SUCCESS;
}

inline INT TCPSyncan(std::string ip, int beginPort, int endPort, INT mode)
{
    /**
     * @brief TCP Syn scan at the target address from the begin port to the end port
     * @param ip the target IP address
     * @param beginPort the begin port
     * @param endPort the end port
     * @param mode the mode to input the begin port and end port
     *             MANUAL(0) for manually input
     *             AUTO(1) for automatically input
     */

    // Declare the variables
    INT ret;
    pthread_attr_t attr;
    pthread_t childThreadID;
    struct TCPFinThreadParam param;
    int status;

    std::cout << "----------------------------- TCP Syn Scan ------------------------------" << std::endl;
    if (mode == MANUAL)
    {
        // If the mode is manual, input the begin port and end port
        std::cout << "[INFO] Please input the range of port(0-65535)..." << std::endl;
        std::cout << "Begin port: ";
        std::cin >> beginPort;
        std::cout << "End port: ";
        std::cin >> endPort;
    }
    /* mode == AUTO */
    // If the mode if AUTO, it means that the port range has already been given by the CML arguments
    // Ping the target IP address first to make sure it is alive
    ret = ping(ip, DEFAULT_PING_TIMES);
    // If the ping is failed, return the error directly and stop the scanning
    if (ret == FAILURE || ret == ERROR || ret == TIMEOUT)
        return ret;

    // Begin the TCP Syn Scan
    std::cout << "[INFO] TCP Syn Scan Host " << ip << " port " << beginPort << "~" << endPort << "..." << std::endl;

    // Initial the parameters for Thread_TCPSynScan
    param.hostIP = ip;
    param.beginPort = beginPort;
    param.endPort = endPort;
    param.localHostIP = DEFAULT_LOCAL_INET_IP;
    param.localPort = DEFAULT_LOCAL_PORT;

    // Initialize the threads for scanning, calling upon the Thread_TCPSynScan function
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Create the child thread
    ret = pthread_create(&childThreadID, &attr, TCPSynScanUtil::Thread_TCPSynScan, (void *)&param);

    // Check if the thread is created successfully
    if (ret != 0)
    {
        // Error
        std::cerr << "[ERROR] Failed to create the thread for the TCP Syn Scanning on ip address " << ip << " and port " << beginPort << "~" << endPort << std::endl;
        return FAILURE;
    }

    // Wait for the child thread to finish
    pthread_join(childThreadID, (void **)&status);

    // Check the status of the thread
    if (status == SUCCESS)
    {
        std::cout << "[INFO] TCP Syn Scan Host " << ip << " port " << beginPort << "~" << endPort << " successfully" << std::endl;
        std::cout << "---------------------------------------------------------------------------" << std::endl;
        return SUCCESS;
    }
    else
    {
        std::cerr << "[ERROR] TCP Syn Scan Host " << ip << " port " << beginPort << "~" << endPort << " FAILED" << std::endl;
        std::cout << "---------------------------------------------------------------------------" << std::endl;
        return FAILURE;
    }

    return SUCCESS;
}

inline INT TCPFinScan(std::string ip, int beginPort, int endPort, INT mode)
{
    /**
     * @brief TCP Fin scan at the target address from the begin port to the end port
     * @param ip the target IP address
     * @param beginPort the begin port
     * @param endPort the end port
     * @param mode the mode to input the begin port and end port
     *             MANUAL(0) for manually input
     *             AUTO(1) for automatically input
     */
    // Declare the variables
    INT ret;
    pthread_attr_t attr;
    pthread_t childThreadID;
    struct TCPFinThreadParam param;
    int status;

    std::cout << "----------------------------- TCP Fin Scan ------------------------------" << std::endl;
    if (mode == MANUAL)
    {
        // If the mode is manual, input the begin port and end port
        std::cout << "[INFO] Please input the range of port(0-65535)..." << std::endl;
        std::cout << "Begin port: ";
        std::cin >> beginPort;
        std::cout << "End port: ";
        std::cin >> endPort;
    }
    /* mode == AUTO */
    // If the mode if AUTO, it means that the port range has already been given by the CML arguments
    // Ping the target IP address first to make sure it is alive
    ret = ping(ip, DEFAULT_PING_TIMES);
    // If the ping is failed, return the error directly and stop the scanning
    if (ret == FAILURE || ret == ERROR || ret == TIMEOUT)
        return ret;

    // Begin the TCP Fin Scan
    std::cout << "[INFO] TCP Fin Scan Host " << ip << " port " << beginPort << "~" << endPort << "..." << std::endl;

    // Initial the parameters for Thread_TCPSynScan
    param.hostIP = ip;
    param.beginPort = beginPort;
    param.endPort = endPort;
    param.localHostIP = DEFAULT_LOCAL_INET_IP;
    param.localPort = DEFAULT_LOCAL_PORT;

    // Initialize the threads for scanning, calling upon the Thread_TCPSynScan function
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Create the child thread
    ret = pthread_create(&childThreadID, &attr, TCPFinScanUtil::Thread_TCPFinScan, (void *)&param);

    // Check if the thread is created successfully
    if (ret != 0)
    {
        // Error
        std::cerr << "[ERROR] Failed to create the thread for the TCP Fin Scanning on ip address " << ip << " and port " << beginPort << "~" << endPort << std::endl;
        return FAILURE;
    }

    // Wait for the child thread to finish
    pthread_join(childThreadID, (void **)&status);

    // Check the status of the thread
    if (status == SUCCESS)
    {
        std::cout << "[INFO] TCP Fin Scan Host " << ip << " port " << beginPort << "~" << endPort << " successfully" << std::endl;
        std::cout << "---------------------------------------------------------------------------" << std::endl;
        return SUCCESS;
    }
    else
    {
        std::cerr << "[ERROR] TCP Fin Scan Host " << ip << " port " << beginPort << "~" << endPort << " FAILED" << std::endl;
        std::cout << "---------------------------------------------------------------------------" << std::endl;
        return FAILURE;
    }
    return SUCCESS;
}

inline INT UDPScan(std::string ip, int beginPort, int endPort, INT mode)
{
    /**
     * @brief UDP scan at the target address from the begin port to the end port
     * @param ip the target IP address
     * @param beginPort the begin port
     * @param endPort the end port
     * @param mode the mode to input the begin port and end port
     *             MANUAL(0) for manually input
     *             AUTO(1) for automatically input
     */

    return SUCCESS;
}

int main(int argc, char *argv[])
{
    int ret;
    // Arguments Parsing
    if (argc >= 2)
    {
        /**
         * Normally parsing the arguments here
         * (1) -h : help
         * (2) -p : ping the <target address> for {times} times
         * (3) -c : TCP connect scan at the <target address> from <begin port> to <end port>
         * (4) -s : TCP syn scan
         * (5) -f : TCP fin scan
         * (6) -u : UDP scan
         */
        std::string optionArg = argv[1];
        if (optionArg == ARG_HELP)
        {
            // If the option is help, print the help information
            help(argv[0]);
            return 0;
        }
        else if (optionArg == ARG_PING)
        {
            /**
             * If the option is ping, there are two cases for the arguments:
             * (1) -p <target address> {times}
             * (2) -p <target address>
             */
            if (argc == 4)
            {
                // (1) -p <target address> {times}
                // Check if the <target address> is a valid IPv4 address
                if (!isValidIPv4(argv[2]))
                {
                    std::cerr << "[ERROR] Target IP address INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }
                // Check if the input IP address is 0 to print the info
                if (std::stoi(argv[3]) == 0)
                    std::cout << "[INFO] Ping the target IP address: " << argv[2] << " until timeout" << std::endl;

                ret = ping(argv[2], std::stoi(argv[3]));

                if (ret == ERROR || ret == TIMEOUT)
                {
                    help(argv[0]);
                    return 1;
                }
                return 0;
            }
            else if (argc == 3)
            {
                // (2) -p <target address>
                // Check if the <target address> is a valid IPv4 address
                if (!isValidIPv4(argv[2]))
                {
                    std::cerr << "[ERROR] Target IP address INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }
                // ping the target IP address for once
                std::cout << "[INFO] Using the default ping times 1 for the target IP address: " << argv[2] << std::endl;

                ret = ping(argv[2], 1);

                if (ret == ERROR || ret == TIMEOUT)
                {
                    help(argv[0]);
                    return 1;
                }
                return 0;
            }
            else
            {
                // If the option is ping,
                // But the number of arguments is not correct, print the help info to instruct the user
                std::cerr << "[ERROR] The number of arguments is INCORRECT" << std::endl;
                help(argv[0]);
                return 1;
            }
        }
        else if (optionArg == ARG_CONNECT_SCAN)
        {
            /**
             * If the option is TCP connect scan, there are two cases for the arguments:
             * (1) -c <target address> {begin port} {end port}
             * (2) -c <target address>
             */
            if (argc == 5)
            {
                // (1) -c <target address> {begin port} {end port}
                // Check if the <target address> is a valid IPv4 address
                if (!isValidIPv4(argv[2]))
                {
                    std::cerr << "[ERROR] Target IP address INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }
                // Check if the {begin port} and {end port} are valid
                if (!isValidPort(std::stoi(argv[3]), std::stoi(argv[4])))
                {
                    std::cerr << "[ERROR] The begin port and end port are INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }
                ret = TCPConnectScan(argv[2], std::stoi(argv[3]), std::stoi(argv[4]), AUTO);
                if (ret == ERROR || ret == TIMEOUT || ret == FAILURE)
                {
                    help(argv[0]);
                    return 1;
                }
                return 0;
            }
            else if (argc == 3)
            {
                // (2) -c <target address>
                // Check if the <target address> is a valid IPv4 address
                if (!isValidIPv4(argv[2]))
                {
                    std::cerr << "[ERROR] Target IP address INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }
                // Manually input the begin port and end port
                ret = TCPConnectScan(argv[2], 0, 0, MANUAL);
                if (ret == ERROR || ret == TIMEOUT || ret == FAILURE)
                {
                    help(argv[0]);
                    return 1;
                }
                return 0;
            }
            else
            {
                // If the option is TCPConnectScan,
                // But the number of arguments is not correct, print the help info to instruct the user
                std::cerr << "[ERROR] The number of arguments is INCORRECT" << std::endl;
                help(argv[0]);
                return 1;
            }
        }
        else if (optionArg == ARG_SYN_SCAN)
        {
            /**
             * If the option is TCP syn scan, there are two cases for the arguments:
             * (1) -s <target address> {begin port} {end port}
             * (2) -s <target address>
             */
            if (argc == 5)
            {
                // (1) -s <target address> {begin port} {end port}
                // Check if the <target address> is a valid IPv4 address
                if (!isValidIPv4(argv[2]))
                {
                    std::cerr << "[ERROR] Target IP address INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }
                // Check if the {begin port} and {end port} are valid
                if (!isValidPort(std::stoi(argv[3]), std::stoi(argv[4])))
                {
                    std::cerr << "[ERROR] The begin port and end port are INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }

                ret = TCPSyncan(argv[2], std::stoi(argv[3]), std::stoi(argv[4]), AUTO);
                if (ret == ERROR || ret == TIMEOUT || ret == FAILURE)
                {
                    help(argv[0]);
                    return 1;
                }
                return 0;
            }
            else if (argc == 3)
            {
                // (2) -s <target address>
                // Check if the <target address> is a valid IPv4 address
                if (!isValidIPv4(argv[2]))
                {
                    std::cerr << "[ERROR] Target IP address INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }
                // Manually input the begin port and end port
                ret = TCPSyncan(argv[2], 0, 0, MANUAL);
                if (ret == ERROR || ret == TIMEOUT || ret == FAILURE)
                {
                    help(argv[0]);
                    return 1;
                }
                return 0;
            }
            else
            {
                // If the option is TCPSynScan,
                // But the number of arguments is not correct, print the help info to instruct the user
                help(argv[0]);
                return 1;
            }
        }
        else if (optionArg == ARG_FIN_SCAN)
        {
            /**
             * If the option is TCP fin scan, there are two cases for the arguments:
             * (1) -f <target address> {begin port} {end port}
             * (2) -f <target address>
             */
            if (argc == 5)
            {
                // (1) -f <target address> {begin port} {end port}
                // Check if the <target address> is a valid IPv4 address
                if (!isValidIPv4(argv[2]))
                {
                    std::cerr << "[ERROR] Target IP address INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }
                // Check if the {begin port} and {end port} are valid
                if (!isValidPort(std::stoi(argv[3]), std::stoi(argv[4])))
                {
                    std::cerr << "[ERROR] The begin port and end port are INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }

                ret = TCPFinScan(argv[2], std::stoi(argv[3]), std::stoi(argv[4]), AUTO);
                if (ret == ERROR || ret == TIMEOUT || ret == FAILURE)
                {
                    help(argv[0]);
                    return 1;
                }
                return 0;
            }
            else if (argc == 3)
            {
                // (2) -f <target address>
                // Check if the <target address> is a valid IPv4 address
                if (!isValidIPv4(argv[2]))
                {
                    std::cerr << "[ERROR] Target IP address INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }
                // Manually input the begin port and end port
                ret = TCPFinScan(argv[2], 0, 0, MANUAL);
                if (ret == ERROR || ret == TIMEOUT || ret == FAILURE)
                {
                    help(argv[0]);
                    return 1;
                }
                return 0;
            }
            else
            {
                // If the option is TCPFinScan,
                // But the number of arguments is not correct, print the help info to instruct the user
                help(argv[0]);
                return 1;
            }
        }
        else if (optionArg == ARG_UDP_SCAN)
        {
            /**
             * If the option is UDP scan, there are two cases for the arguments:
             * (1) -u <target address> {begin port} {end port}
             * (2) -u <target address>
             */
            if (argc == 5)
            {
                // (1) -u <target address> {begin port} {end port}
                // Check if the <target address> is a valid IPv4 address
                if (!isValidIPv4(argv[2]))
                {
                    std::cerr << "[ERROR] Target IP address INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }
                // Check if the {begin port} and {end port} are valid
                if (!isValidPort(std::stoi(argv[3]), std::stoi(argv[4])))
                {
                    std::cerr << "[ERROR] The begin port and end port are INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }

                ret = UDPScan(argv[2], std::stoi(argv[3]), std::stoi(argv[4]), AUTO);
                if (ret == ERROR || ret == TIMEOUT || ret == FAILURE)
                {
                    help(argv[0]);
                    return 1;
                }
                return 0;
            }
            else if (argc == 3)
            {
                // (2) -u <target address>
                // Check if the <target address> is a valid IPv4 address
                if (!isValidIPv4(argv[2]))
                {
                    std::cerr << "[ERROR] Target IP address INVALID" << std::endl;
                    help(argv[0]);
                    return 1;
                }
                // Manually input the begin port and end port
                ret = UDPScan(argv[2], 0, 0, MANUAL);
                if (ret == ERROR || ret == TIMEOUT || ret == FAILURE)
                {
                    help(argv[0]);
                    return 1;
                }
                return 0;
            }
            else
            {
                // If the option is UDPScan,
                // But the number of arguments is not correct, print the help info to instruct the user
                std::cerr << "[ERROR] The number of arguments is INCORRECT" << std::endl;
                help(argv[0]);
                return 1;
            }
        }
        else
        {
            // If none of the options is matched, print the help info to instruct the user
            std::cerr << "[ERROR] The option is INVALID" << std::endl;
            help(argv[0]);
            return 1;
        }
    }
    else
    {
        // If the number of arguments is less than 2, print the help info to instruct the user
        std::cerr << "[ERROR] The number of arguments is LESS THAN 2" << std::endl;
        help(argv[0]);
        return 1;
    }
    return 0;
}