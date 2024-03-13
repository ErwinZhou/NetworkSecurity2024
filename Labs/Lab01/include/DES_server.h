#include <iostream>
#include <sys/socket.h> // Provides system calls and data structures for sockets
#include <netinet/in.h> // Provides data structures for Internet address family
#include <arpa/inet.h>  // Provides IP address conversion functions
#include <unistd.h>     // Provides general system call functions
#include <netdb.h>      // Provides domain name resolution functions
using namespace std;

char *commanderName;
int sListenSocket;
int sAcceptSocket;
sockaddr_in serverAddr;
sockaddr_in agentAddr;
int instruction;