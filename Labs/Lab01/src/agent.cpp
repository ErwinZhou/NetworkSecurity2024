#include "agent.h"

Agent::Agent()
{
    agentName = "";
    agentcodeName = "";
    agentSocket = 0;
}
Agent::Agent(string name, string codeName, int socket)
{
    agentName = name;
    agentcodeName = codeName;
    agentSocket = socket;
}
string Agent::getAgentName()
{
    return agentName;
}
void Agent::setAgentName(string name)
{
    agentName = name;
}
string Agent::getAgentcodeName()
{
    return agentcodeName;
}
void Agent::setAgentcodeName(string codeName)
{
    agentcodeName = codeName;
}
int Agent::getAgentSocket()
{
    return agentSocket;
}
void Agent::setAgentSocket(int socket)
{
    agentSocket = socket;
}