#include "utils.h"

void printBits64(uint64_t num)
{
    for (int i = 63; i >= 0; --i)
    {
        cout << ((num >> i) & 1);
    }
    cout << endl;
}
void printBits32(uint32_t num)
{
    for (int i = 31; i >= 0; --i)
    {
        cout << ((num >> i) & 1);
    }
    cout << endl;
}

string hexToBinary(const string &hex)
{
    bitset<16> set(stoull(hex, nullptr, 16));
    return set.to_string();
}

string stringBits64(uint64_t num)
{
    string binary = "";
    for (int i = 63; i >= 0; --i)
    {
        binary += to_string((num >> i) & 1);
    }
    return binary;
}
string stringBits32(uint32_t num)
{
    string binary = "";
    for (int i = 31; i >= 0; --i)
    {
        binary += to_string((num >> i) & 1);
    }
    return binary;
}
string binaryToHex(const string &binary)
{
    bitset<64> set(binary);
    stringstream ss;
    ss << hex << set.to_ullong();
    return ss.str();
}

string timeNow()
{
    /**
     * Get the current time
     * @return string
     */
    auto now = chrono::system_clock::now();
    time_t current_time = chrono::system_clock::to_time_t(now);

    string timeString = ctime(&current_time);
    timeString.pop_back();
    return timeString;
}

ssize_t TotalRecv(int s, void *buf, size_t len, int flags)
{
    /**
     * Receive data from the socket in case sometimes not all data is received out of change of network
     * @param s: socket
     * @param buf: buffer to store the data
     * @param len: length of the data
     * @param flags: flags
     * @return ssize_t
     */
    size_t nCurSize = 0;
    while (nCurSize < len)
    {
        ssize_t nRes = recv(s, ((char *)buf) + nCurSize, len - nCurSize, flags);
        if (nRes < 0 || nRes + nCurSize > len)
        {
            return -1;
        }
        nCurSize += nRes;
    }
    return nCurSize;
}

void PhantomHook(int role, int socket)
{
    /**
     * Secret Code Name:Phantom Hook
     * This is a bait to lure the enemy in and then we can take them down
     * We will use this as a trap to catch the enemy by communicating without any encryption
     * role: AGENT for agent, M16 for headquarter
     * @param role: AGENT for agent, M16 for headquarter
     * @param socket: socket
     * @return void
     */
    if (role == AGENT)
    {
        // This part is in the agent's side
        string str_time = timeNow();
        cout << "<Agent::System @ " + str_time + " # Message>:007, we’re reading you loud and clear. Noting happened. Don't worry.Proceed with your update. Over." << endl;
    }
    else
    {
        // This part is in the headquarter's side
        string str_time = timeNow();
        cout << "<Headquarter::System @ " + str_time + " # Message>:Phantom Hook is activated." << endl;
        // Notify the agent to also activate the Phantom Hook
        send(socket, "Phantom Hook", 100, 0);
        pid_t nPid;
        nPid = fork();
    }
}

void SilentGuardian(int role, int socket)
{
    /**
     * Secret Code Name:Silent Guardian
     * This is a secret communicating channel for the headquarter and the agent
     * In case there is enemy's interception, this will be activated to ensure the safety of the communication
     * The communication is based on the DES algorithm
     * @param role: AGENT for agent, M16 for headquarter
     * @param socket: socket
     * @return void
     */
    if (role == AGENT)
    {
        // This part is in the agent's side
        string str_time = timeNow();
        cout << "<Agent::System @ " + str_time + " # Message>:007, we’re reading you loud and clear on Silent Guardian. All other channels are compromised. Proceed with your update. Over." << endl;
    }
    else
    {
        // This part is in the headquarter's side
        string str_time = timeNow();
        cout << "<Headquarter::System @ " + str_time + " # Message>:Silent Guardian is activated.." << endl;
        // Notify the agent to also activate the Silent Guardian
        send(socket, "Silent Guardian", 100, 0);
        pid_t nPid;
        nPid = fork();
    }
}