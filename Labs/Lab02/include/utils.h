#include <iostream>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <sys/socket.h>
#include <chrono>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <semaphore.h>
#include <aio.h> // For async I/O
#include "agent.h"
#include "DES.h"
#include "RSA.h"
#if !defined(BUFFERSIZE)
#define BUFFERSIZE 1024

#endif

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
                                                                        // DES key transfered by RSA encryption

void PhantomHook_AIO(int role, Agent agent);                                // Special Operation: Communcation between Agent and Headquarter without DES encryption to lure the enemy
void SilentGuardian_AIO(int role, Agent agent, DESUtils des, RSAUtils rsa); // Special Operation: Communication between Agent and Headquarter with DES encryption to protect the secret

extern char strStdinBuffer_AIO[BUFFERSIZE];
extern char strEncryptedBuffer_AIO[BUFFERSIZE];
extern char strDecryptedBuffer_AIO[BUFFERSIZE];

/* Class Implementation for asynchronous I/O
 * storing all the context and other essential information for the asynchronous I/O
 * Working pair:
 * StdinSingle  <---> SockoutSingle
 * SockinSingle <---> StdoutSingle
 */
class StdinSingle
{
    /**
     * Specifically for monitoring the input from stdin
     */
public:
    int m_role;
    int m_nSocket;
    aiocb *m_pReq;
    sem_t &m_bStop;
    string m_agentCodeName;
    DESUtils m_des;
    StdinSingle(int role, Agent agent, DESUtils des, sem_t &bStop);
    ~StdinSingle();
    static void StdinReadCompletionHandler(sigval_t sigval);
};
class StdoutSingle
{
    /**
     * Specifically for monitoring the output to stdout
     */
public:
    int m_role;
    int m_nSocket;
    aiocb *m_pReq;
    sem_t &m_bStop;
    string m_agentCodeName;
    DESUtils m_des;
    StdoutSingle(int role, Agent agent, DESUtils des, sem_t &bStop);
    ~StdoutSingle();
    static void StdoutWriteCompletionHandler(sigval_t sigval);
};
class SockinSingle
{
    /**
     * Specifically for monitoring the input from socket
     */
public:
    int m_role;
    int m_nSocket;
    aiocb *m_pReq;
    sem_t &m_bStop;
    string m_agentCodeName;
    DESUtils m_des;
    SockinSingle(int role, Agent agent, DESUtils des, sem_t &bStop);
    ~SockinSingle();
    static void SockinReadCompletionHandler(sigval_t sigval);
};
class SockoutSingle
{
    /**
     * Specifically for monitoring the output to socket
     */
public:
    int m_role;
    int m_nSocket;
    aiocb *m_pReq;
    sem_t &m_bStop;
    string m_agentCodeName;
    DESUtils m_des;
    SockoutSingle(int role, Agent agent, DESUtils des, sem_t &bStop);
    ~SockoutSingle();
    static void SockoutWriteCompletionHandler(sigval_t sigval);
};