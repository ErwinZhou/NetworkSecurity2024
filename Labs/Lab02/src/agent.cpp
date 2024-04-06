#include "agent.h"

Agent::Agent()
{
    agentName = "";
    agentCodeName = "";
    agentSocket = 0;
}
Agent::Agent(string name, string codeName, int socket)
{
    agentName = name;
    agentCodeName = codeName;
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
string Agent::getAgentCodeName()
{
    return agentCodeName;
}
void Agent::setAgentCodeName(string codeName)
{
    agentCodeName = codeName;
}
int Agent::getAgentSocket()
{
    return agentSocket;
}
void Agent::setAgentSocket(int socket)
{
    agentSocket = socket;
}