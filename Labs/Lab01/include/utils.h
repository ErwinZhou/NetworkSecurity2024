#include <iostream>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <sys/socket.h>
#include <chrono>
#include <unistd.h>
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
string timeNow();
ssize_t TotalRecv(int s, void *buf, size_t len, int flags);
void PhantomHook(int role, int socket);
void SilentGuardian(int role, int socket);