#include <iostream>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <sys/socket.h>
#include <chrono>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <deque>
#include <mutex>
#include "agent.h"
#include "DES.h"
using namespace std;
enum
{
    M16 = 0,
    AGENT = 1
};
// deque<string> log_queue; // log queue for multi-thread
// mutex log_queue_mutex;   // lock for log queue
void printBits64(uint64_t num);
void printBits32(uint32_t num);
string stringBits64(uint64_t num);
string stringBits32(uint32_t num);
string timeNow();
void signalHandler(int sig);
void *logThreadMain(void *arg);
ssize_t TotalRecv(int s, void *buf, size_t len, int flags);
void PhantomHook(int role, Agent agent);
void SilentGuardian(int role, Agent agent, DESUtils des);