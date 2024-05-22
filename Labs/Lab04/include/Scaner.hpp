// Declarations of the functions used in Scaner.cpp

#include "TCPConnectScan.hpp"
#include "TCPFinScan.hpp"
#include "TCPSynScan.hpp"
#include "UDPScan.hpp"
#include "ping.hpp"
#include <iostream> // std::cout

inline void help(const char *program);
INT ping(std::string ip, int times);
inline INT TCPConnectScan(std::string ip, int beginPort, int endPort, INT mode);
inline INT TCPSyncan(std::string ip, int beginPort, int endPort, INT mode);
inline INT TCPFinScan(std::string ip, int beginPort, int endPort, INT mode);
inline INT UDPScan(std::string ip, int beginPort, int endPort, INT mode);