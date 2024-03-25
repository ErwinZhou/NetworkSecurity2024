#include <iostream>
#include <limits>
#include <string.h>
#include <netinet/in.h>
using namespace std;
/**
 * Agent class
 * This class is used to store the information of the agent
 * including the agent's name, code name, and socket
 */
class Agent
{
private:
    string agentName;
    string agentcodeName;
    int agentSocket;

public:
    Agent();
    Agent(string name, string codeName, int socket);
    ~Agent(){};
    string getAgentName();
    void setAgentName(string name);
    string getAgentcodeName();
    void setAgentcodeName(string codeName);
    int getAgentSocket();
    void setAgentSocket(int socket);
};