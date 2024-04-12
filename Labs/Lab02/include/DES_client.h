#include <iostream>
#include <limits>
#include <string.h>
#include <chrono>
#include <sys/socket.h> // Provides system calls and data structures for sockets
#include <netinet/in.h> // Provides data structures for Internet address family
#include <arpa/inet.h>  // Provides IP address conversion functions
#include <unistd.h>     // Provides general system call functions
#include <netdb.h>      // Provides domain name resolution functions
#include "utils.h"      // utils.h is already includes RSA.h, DES.h and agent.h
using namespace std;
#define SERVER_PORT 8007
#define BUFFERSIZE 1024

/* DES parameters */
DESUtils des;
// Hadcoded DES key for default, discard for now due to unsafety
char *defaultKey = "YCZhouNB";
char decryptedtext[64];

/* RSA parameters */
RSAUtils rsa;

/* socket parameters */
string agentName;
string agentCodeName;
string serverIP = "192.168.126.128";
int agentSocket;
struct sockaddr_in serverAddr;

int choice; // choice for the user to select the default IP address or input the IP address manually

char strSocketBuffer[BUFFERSIZE];
