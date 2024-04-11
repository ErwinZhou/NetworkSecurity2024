#include <iostream>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <sys/socket.h>
#include <chrono>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "agent.h"
#include "DES.h"
#include "RSA.h"
using namespace std;
enum
{
    M16 = 0,
    AGENT = 1
};
void printBits64(uint64_t num);
void printBits32(uint32_t num);
string stringBits64(uint64_t num);
string stringBits32(uint32_t num);
string timeNow();                                                       // Get current time to display
void signalHandler(int sig);                                            // Handle signal between process to control them
ssize_t TotalRecv(int s, void *buf, size_t len, int flags);             // Receive data from socket, in case of partial data out of unstable communication
void PhantomHook(int role, Agent agent);                                // Special Operation: Communcation between Agent and Headquarter without DES encryption to lure the enemy
void SilentGuardian(int role, Agent agent, DESUtils des, RSAUtils rsa); // Special Operation: Communication between Agent and Headquarter with DES encryption to protect the secret
                                                                        //                    DES key transfered by RSA encryption