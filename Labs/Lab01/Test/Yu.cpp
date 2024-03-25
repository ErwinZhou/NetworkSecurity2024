#include <iostream>
#include <limits>
#include <string.h>
#include <sys/socket.h> // Provides system calls and data structures for sockets
#include <netinet/in.h> // Provides data structures for Internet address family
#include <arpa/inet.h>  // Provides IP address conversion functions
#include <unistd.h>     // Provides general system call functions
#include <netdb.h>      // Provides domain name resolution functions
#include "utils.h"
using namespace std;
#define SERVER_PORT 8007

void SecretChat(int nSock, char *pRemoteName, char *pKey)
{
    CDesOperate cDes;
    if (strlen(pKey) != 8)
    {
        printf("Key length error");
        return;
    }
    pid_t nPid;
    nPid = fork();
    if (nPid != 0)
    {
        // Parent process
        while (1)
        {
            bzero(&strSocketBuffer, BUFFERSIZE);
            int nLength = 0;
            nLength = TotalRecv(nSock, strSocketBuffer, BUFFERSIZE, 0);
            if (nLength != BUFFERSIZE)
            {
                break;
            }
            else
            {
                int nLen = BUFFERSIZE;
                cDes.Decry(strSocketBuffer, BUFFERSIZE, strDecryBuffer, nLen, pKey, 8);
                strDecryBuffer[BUFFERSIZE - 1] = 0;
                if (strDecryBuffer[0] != 0 && strDecryBuffer[0] != '\n')
                {
                    printf("Receive message form <%s>: %s\n",
                           pRemoteName, strDecryBuffer);
                    if (0 == memcmp("quit", strDecryBuffer, 4))
                    {
                        printf("Quit!\n");
                        break;
                    }
                }
            }
        }
    }
    else
    {
        while (1)
        {
            bzero(&strStdinBuffer, BUFFERSIZE);
            while (strStdinBuffer[0] == 0)
            {
                if (fgets(strStdinBuffer, BUFFERSIZE, stdin) == NULL)
                {
                    continue;
                }
            }
            int nLen = BUFFERSIZE;
            cDes.Encry(strStdinBuffer, BUFFERSIZE, strEncryBuffer, nLen, pKey, 8);
            if (send(nSock, strEncryBuffer, BUFFERSIZE, 0) != BUFFERSIZE)
            {
                perror("send");
            }
            else
            {
                if (0 == memcmp("quit", strStdinBuffer, 4))
                {
                    printf("Quit!\n");
                    break;
                }
            }
        }
    }
}