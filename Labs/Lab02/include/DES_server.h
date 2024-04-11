#include <iostream>
#include <limits>
#include <string.h>
#include <mutex>
#include <map>
#include <signal.h>
#include <sys/socket.h> // Provides system calls and data structures for sockets
#include <netinet/in.h> // Provides data structures for Internet address family
#include <arpa/inet.h>  // Provides IP address conversion functions
#include <unistd.h>     // Provides general system call functions
#include <netdb.h>      // Provides domain name resolution functions
#include <sys/wait.h>   // Provides waitpid() function
#include "utils.h"      // utils.h is already includes RSA.h, DES.h and agent.h
using namespace std;
/**
 * In case the M16 headquarter is under a DDoS attack
 * There will be a maximum of 5 connection requests
 */
#define MAX_CONNECTION 5
#define SERVER_PORT 8007

map<string, string> agentConfidential;
string agentNames[] = {"James Bond", "David Webb", "Natasha Romanoff", "Tom Cruise", "Rowan Atkinson"};
string agentPasswords[] = {"007", "Jason Bourne", "Black Widow", "Ethan Hunt", "Johnny English"};

/* DES parameters */
DESUtils des;
// Hadcoded DES key for default, discard for now due to unsafety
char *defaultKey = "YCZhouNB";
uint64_t numKey; // Key in hexadecimal
char cipheredtext[64];

/* RSA parameters */
RSAUtils rsa;

/* Socket parameters */
string commanderName;
int sListenSocket;
int sAcceptSocket;
sockaddr_in serverAddr;
sockaddr_in agentAddr;
int instruction;
char agentCodeName[64];
// vector to store the agents
vector<Agent> agents;
pid_t nPid;
// Mutex for the agents
mutex agentsMtx;
// Timeout for the socket
struct timeval timeout;

// Handling emergency for the commnad center
void emergencyResponse();
// Handling the signal of Ctrl+C to resolve the Parent Process holding socket WHILE dying issue
void handle_sigint(int sig);
