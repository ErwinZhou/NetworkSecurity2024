// The main function of the program: Parse the arguments and Execute the scaner

#include "Scaner.hpp"

INT ping(std::string ip, int times)
{
    /**
     * @brief Ping the target IP address to check if it is alive
     * @param ip the target IP address
     * @param times the times to ping the target IP address
     * @return The ping results from the host:
     *         1 for success
     *         0 for failure
     *         -1 for error
     */

    // Ping the target IP address
    ICMPUtil icmpUtil(ip, DEFAULT_LOCAL_PORT, DEFAULT_LOCAL_HOST_IP);
    INT ret = icmpUtil.ping(times);
    if (ret == SUCCESS)
    {
        std::cout << "The target IP address is alive!" << std::endl;
        return SUCCESS;
    }
    else if (ret == FAILURE)
    {
        std::cout << "The target IP address is not alive!" << std::endl;
        return FAILURE;
    }
    else if (ret == ERROR)
    {
        std::cerr << "Error occurs when pinging the target IP address!" << std::endl;
        return ERROR;
    }
    else if (ret == TIMEOUT)
    {
        std::cerr << "Timeout when pinging the target IP address!" << std::endl;
        return TIMEOUT;
    }
    std::cout
        << "---------------------------------------------------------------------------" << std::endl;
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

    std::cerr << "Usage: sudo ./" << programName << " [-Option] [target address] [times]" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  [-h] --help" << std::endl;
    std::cerr << "  [-p] [target address] [times] \n"
              << "       --ping the [target address] for [times] times\n"
              << "       --if [times] equals 0, it stands for ping until timeout\n"
              << "       --if [times] is empty, it stands for ping for once\n"
              << "       --if [target address] is also empty, it stands for using default target address 127.0.0.1 for test" << std::endl;
    std::cerr << "  [-c] --TCP connect scan" << std::endl;
    std::cerr << "  [-s] --TCP syn scan" << std::endl;
    std::cerr << "  [-f] --TCP fin scan" << std::endl;
    std::cerr << "  [-u] --UDP scan" << std::endl;

    std::cerr
        << "---------------------------------------------------------------------------" << std::endl;
    return;
}

inline INT TCPConnectScan()
{
    return SUCCESS;
}

inline INT TCPSyncan()
{
    return SUCCESS;
}

inline INT TCPFinScan()
{
    return SUCCESS;
}

inline INT UDPScan()
{
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
         * (2) -p : ping the [target address] for [times] times
         * (3) -c : TCP connect scan
         * (4) -s : TCP syn scan
         * (5) -f : TCP fin scan
         * (6) -u : UDP scan
         */
        std::string optionArg = argv[1];
        if (optionArg == ARG_HELP)
        {
            // If the option is help, print the help information
            help(argv[0]);
        }
        else if (optionArg == ARG_PING)
        {
            /**
             * If the option is ping, there are three cases for the arguments:
             * (1) -p [target address] [times]
             * (2) -p [target address]
             * (3) -p
             */
            if (argc == 4)
            {
                // (1) -p [target address] [times]
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
                // (2) -p [target address]
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
            else if (argc == 2)
            {
                // (3) -p
                // ping the default target IP address for once
                std::cout << "[INFO] Using the default ping times 1 for default target IP address: " << DEFAULT_HOST_IP << std::endl;
                ret = ping(DEFAULT_HOST_IP, 1);
                if (ret == ERROR || ret == TIMEOUT)
                {
                    help(argv[0]);
                    return 1;
                }
                return 0;
            }
            else
            {
                help(argv[0]);
                return 1;
            }
        }
        else if (optionArg == ARG_CONNECT_SCAN)
        {
            TCPConnectScan();
        }
        else if (optionArg == ARG_SYN_SCAN)
        {
            TCPSyncan();
        }
        else if (optionArg == ARG_FIN_SCAN)
        {
            TCPFinScan();
        }
        else if (optionArg == ARG_UDP_SCAN)
        {
            UDPScan();
        }
        else
        {
            help(argv[0]);
            return 1;
        }
    }
    else
    {
        help(argv[0]);
        return 1;
    }
    return 0;
}