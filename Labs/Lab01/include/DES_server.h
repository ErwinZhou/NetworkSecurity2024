#include <iostream>
#include <limits>
#include <string.h>
#include <sys/socket.h> // Provides system calls and data structures for sockets
#include <netinet/in.h> // Provides data structures for Internet address family
#include <arpa/inet.h>  // Provides IP address conversion functions
#include <unistd.h>     // Provides general system call functions
#include <netdb.h>      // Provides domain name resolution functions
using namespace std;
// In case the M16 headquarter is under a DDoS attack
// There will be a maximum of 5 connection requests
#define MAX_CONNECTION 5
#define SERVER_PORT 8007

string commanderName;
int sListenSocket;
int sAcceptSocket;
sockaddr_in serverAddr;
sockaddr_in agentAddr;
int instruction;

// Handling emergency for the commnad center
void emergencyResponse();