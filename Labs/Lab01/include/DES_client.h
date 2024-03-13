#include <iostream>
#include <sys/socket.h> // Provides system calls and data structures for sockets
#include <netinet/in.h> // Provides data structures for Internet address family
#include <arpa/inet.h>  // Provides IP address conversion functions
#include <unistd.h>     // Provides general system call functions
#include <netdb.h>      // Provides domain name resolution functions
using namespace std;
#define SERVER_PORT 5000

char *agentName;
char *serverIP;
int clientSocket;
struct sockaddr_in serverAddr;