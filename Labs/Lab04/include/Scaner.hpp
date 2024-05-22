// Declarations of the functions used in Scaner.cpp

#include "TCPConnectScan.hpp"
#include "TCPFinScan.hpp"
#include "TCPSynScan.hpp"
#include "UDPScan.hpp"
#include "ping.hpp"
#include <iostream> // std::cout

inline void help(const char *program);
INT ping(std::string ip, int times);