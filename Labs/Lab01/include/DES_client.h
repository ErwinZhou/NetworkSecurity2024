#include <iostream>
#include <limits>
#include <sys/socket.h> // Provides system calls and data structures for sockets
#include <netinet/in.h> // Provides data structures for Internet address family
#include <arpa/inet.h>  // Provides IP address conversion functions
#include <unistd.h>     // Provides general system call functions
#include <netdb.h>      // Provides domain name resolution functions
using namespace std;
#define SERVER_PORT 8007

string agentName;
string serverIP = "192.168.126.128";
int agentSocket;
struct sockaddr_in serverAddr;

int choice; // choice for the user to select the default IP address or input the IP address manually