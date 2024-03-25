#include <iostream>
#include <limits>
#include <string.h>
#include <mutex>
#include <map>
#include <sys/socket.h> // Provides system calls and data structures for sockets
#include <netinet/in.h> // Provides data structures for Internet address family
#include <arpa/inet.h>  // Provides IP address conversion functions
#include <unistd.h>     // Provides general system call functions
#include <netdb.h>      // Provides domain name resolution functions
#include "utils.h"
#include "DES.h"
#include "agent.h"
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
char *key = "YCZhouNB"; // Hardcoded key
uint64_t numKey;        // Key in hexadecimal
char cipheredtext[64];

/* Socket parameters */
string commanderName;
int sListenSocket;
int sAcceptSocket;
sockaddr_in serverAddr;
sockaddr_in agentAddr;
int instruction;
char agentcodeName[64];
// vector to store the agents
vector<Agent> agents;
pid_t nPid;
mutex agentsMtx;

// Handling emergency for the commnad center
void emergencyResponse();
