// The main function of the program: Parse the arguments and Execute the scaner

#include <iostream>
#include "connectScan.hpp"
#include "finScan.hpp"
#include "synScan.hpp"
#include "UDPScan.hpp"
#include "ping.hpp"

bool ping(const char *ip)
{
    /**
     * @brief Ping the target IP address to check if it is alive
     * @param ip the target IP address
     * @return bool true if the target IP address is alive, false otherwise
     */

    std::cout << "Ping the target IP address" << std::endl;
    // Ping the target IP address

    std::cout
        << "---------------------------------------------------------------------------" << std::endl;
}
inline void help(const char *program)
{
    /**
     * @brief Print the help message to instruct the user how to use the program
     * @param program the name of the program
     * @return void
     */
    // Help Info
    std::cerr << "------------------------------ Scanner Help Info ------------------------------" << std::endl;

    std::string programPath(program);
    size_t pos = programPath.find_last_of("/\\");
    std::string programName = programPath.substr(pos + 1);

    std::cerr << "Usage: ./" << programName << " [-Option] [file path of the file validated] [file path of the .md5 file] [times]" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  [-h] --help" << std::endl;
    std::cerr << "  [-c] --TCP connect scan" << std::endl;
    std::cerr << "  [-s] --TCP syn scan" << std::endl;
    std::cerr << "  [-f] --TCP fin scan" << std::endl;
    std::cerr << "  [-u] --UDP scan" << std::endl;

    std::cerr
        << "---------------------------------------------------------------------------" << std::endl;
    return;
}

inline int connectScan()
{
}

int main(int argc, char *argv[])
{
    return 0;
}