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

void signalHandler(int sig)
{
    /**
     * Signal handler: Used for the one process to exit after receiving the signal from another
     * @param sig: signal
     * @return void
     */
    exit(0);
}
void *logThreadMain(void *arg)
{
    /**
     * Log thread main function
     * @param arg: argument
     * @return void
     */
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

void PhantomHook(int role, Agent agent)
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
    int socket = agent.getAgentSocket();
    string codeName = agent.getAgentCodeName();
    if (role == AGENT)
    {
        // This part is in the agent's side
        string str_time = timeNow();
        cout << "<SecretHideout::System @ " + str_time + " # Message>:We’re reading you loud and clear." << endl;
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Phantom Hook is activated.." << endl;
        cout << "<SecretHideout::System @ " + str_time + " # Message>:You know what to do......." << endl;
        pid_t nPid;
        nPid = fork();
        if (nPid != 0)
        {
            /**
             * Parent process
             * In charge of the following:
             * 1. Receive message from the headquarter
             * 2. Output the message
             */
            char strSocketBuffer[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strSocketBuffer, 0, sizeof(strSocketBuffer));
                int nLength = 0;
                // Using the TotalRecv function to ensure all data is received
                nLength = TotalRecv(socket, strSocketBuffer, 255, 0);
                if (nLength == 255)
                {
                    // Normal Case
                    if (strSocketBuffer[0] != 0 && strSocketBuffer[0] != '\n')
                    {
                        // Fix the problem of the sudden message at the same line of "Please Enter:"
                        cout << endl;
                        string str_time = timeNow();
                        cout << "<SecretHideout::Headquarter @ " + str_time + " # Message>: " + strSocketBuffer;
                        if (memcmp("OVER", strSocketBuffer, 4) == 0)
                        {
                            // If the message the agent received is "OVER"
                            // It means the Headquarter is done communicating
                            string str_time = timeNow();
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:The Headquarter is off the line." << endl;
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                            cout << "-----Communication OVER-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                        else if (memcmp("COMMUNICATION ERROR", strSocketBuffer, 19) == 0)
                        {
                            // If the message the agent received is "COMMUNICATION ERROR"
                            // It means there is a problem with the communication
                            string str_time = timeNow();
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:There is a problem with the communication..." << endl;
                            cout << "-----Shutdown Communication-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                    }
                }
                else
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<SecretHideout::System @ " + str_time + " # Message>: There is a problem with the communication..." << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    send(socket, hint, 100, 0);
                    kill(nPid, SIGTERM);
                    return;
                }
            }
        }
        else
        {
            /**
             * Child process
             * In charge of the following:
             * 1. Get the message from the agent
             * 2. Send the message to the headquarter
             */
            char strStdinBuffer[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strStdinBuffer, 0, sizeof(strStdinBuffer));
                cout << "Please Enter:";
                // Get the message from the agent
                // if (fgets(strStdinBuffer, 255, stdin) == NULL)
                // {
                //     continue;
                // }
                cout << "Please Enter:";
                cin.getline(strStdinBuffer, 255);
                int nLen = 255;
                // Send the message to the agent
                if (send(socket, strStdinBuffer, sizeof(strStdinBuffer), 0) != 255)
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<SecretHideout::System @ " + str_time + " # Message>: There is a problem with the communication..." << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    send(socket, hint, 100, 0);
                    kill(getppid(), SIGTERM);
                    return;
                }
                else
                {
                    if (memcmp("OVER", strStdinBuffer, 4) == 0)
                    {
                        // If the message the agent sent is "OVER"
                        // It means the agent is done communicating
                        string str_time = timeNow();
                        cout << "<SecretHideout::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                        cout << "-----Communication OVER-----" << endl;
                        // Tell the headquarter the communication is over
                        send(socket, "OVER", 100, 0);
                        // Put an end to the the parent process
                        kill(getppid(), SIGTERM);
                        return;
                    }
                }
                cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Message Sent." << endl;
            }
        }
    }
    else if (role == M16)
    {
        // This part is in the headquarter's side
        string str_time = timeNow();
        cout << "<Headquarter::System @ " + str_time + " # Message>:Phantom Hook is activated." << endl;
        // Notify the agent to also activate the Phantom Hook
        send(socket, "Phantom Hook", 100, 0);
        pid_t nPid;
        nPid = fork();
        if (nPid != 0)
        {
            /**
             * Parent process
             * In charge of the following:
             * 1. Receive message from the agent
             * 2. Output the message
             */
            char strSocketBuffer[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strSocketBuffer, 0, sizeof(strSocketBuffer));
                int nLength = 0;
                // Using the TotalRecv function to ensure all data is received
                nLength = TotalRecv(socket, strSocketBuffer, 255, 0);
                if (nLength == 255)
                {
                    // Normal Case
                    if (strSocketBuffer[0] != 0 && strSocketBuffer[0] != '\n')
                    {
                        // Fix the problem of the sudden message at the same line of "Please Enter:"
                        cout << endl;
                        string str_time = timeNow();
                        cout << "<Headquarter::" + codeName + " @ " + str_time + " # Message>: " + strSocketBuffer << endl;
                        if (memcmp("OVER", strSocketBuffer, 4) == 0)
                        {
                            // If the message the headquarter received is "OVER"
                            // It means the agent is done communicating
                            string str_time = timeNow();
                            cout << "<Headquarter::System @ " + str_time + " # Message>:Agent OUT." << endl;
                            cout << "<Headquarter::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                            cout << "-----Communication OVER-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                        else if (memcmp("COMMUNICATION ERROR", strSocketBuffer, 19) == 0)
                        {
                            // If the message the headquarter received is "COMMUNICATION ERROR"
                            // It means there is a problem with the communication
                            string str_time = timeNow();
                            cout << "<Headquarter::System @ " + str_time + " # Message>:There is a problem with the communication..." << endl;
                            cout << "-----Shutdown Communication-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                    }
                }
                else
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<Headquarter::System @ " + str_time + " # Message>: There is a problem with the communication..." << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    send(socket, hint, 100, 0);
                    kill(nPid, SIGTERM);
                    return;
                }
            }
        }
        else
        {
            /**
             * Child process
             * In charge of the following:
             * 1. Get the message from the headquarter
             * 2. Send the message to the agent
             */
            char strStdinBuffer[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strStdinBuffer, 0, sizeof(strStdinBuffer));
                cout << "Please Enter:";
                // Get the message from the headquarter
                // if (fgets(strStdinBuffer, 255, stdin) == NULL)
                // {
                //     continue;
                // }
                cout << "Please Enter:";
                cin.getline(strStdinBuffer, 255);
                int nLen = 255;
                // Send the message to the agent
                if (send(socket, strStdinBuffer, sizeof(strStdinBuffer), 0) != 255)
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<Headquarter::System @ " + str_time + " # Message>: There is a problem with the communication..." << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    send(socket, hint, 100, 0);
                    // Put an end to the the parent process
                    kill(getppid(), SIGTERM);
                    return;
                }
                else
                {
                    if (memcmp("OVER", strStdinBuffer, 4) == 0)
                    {
                        // If the message the headquarter sent is "OVER"
                        // It means the headquarter is done communicating
                        string str_time = timeNow();
                        cout << "<Headquarter::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                        cout << "-----Communication OVER-----" << endl;
                        // Tell the agent the communication is over
                        send(socket, "OVER", 100, 0);
                        // Put an end to the the parent process
                        kill(getppid(), SIGTERM);
                        return;
                    }
                }
                cout << "<Headquarter::System @ " + timeNow() + " # Message>:Message Sent." << endl;
            }
        }
    }
}
void SilentGuardian(int role, Agent agent, DESUtils des)
{
    /**
     * Secret Code Name:Silent Guardian
     * This is a secret communicating channel for the headquarter and the agent
     * In case there is enemy's interception, this will be activated to ensure the safety of the communication
     * The communication is based on the DES algorithm
     * @param role: AGENT for agent, M16 for headquarter
     * @param agent: Agent class object storing the essential information
     * @param des: DESUtils class object used for encryption and decryption
     * @return void
     */
    int socket = agent.getAgentSocket();
    string codeName = agent.getAgentCodeName();
    if (role == AGENT)
    {
        // This part is in the agent's side
        string str_time = timeNow();
        cout << "<SecretHideout::System @ " + str_time + " # Message>:All channels have been compromised!" << endl;
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Silent Guardian is activated.." << endl;
        pid_t nPid;
        nPid = fork();
        if (nPid != 0)
        {

            /**
             * Parent process
             * In charge of the following:
             * 1. Receive message from the headquater
             * 2. Decrypt the message
             * 3. Output the message
             */
            char strSocketBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strSocketBuffer, 0, sizeof(strSocketBuffer));
                memset(encryedtext, 0, sizeof(encryedtext));
                memset(decryptedtext, 0, sizeof(decryptedtext));
                int nLength = 0;
                // Using the TotalRecv function to ensure all data is received
                nLength = TotalRecv(socket, strSocketBuffer, 255, 0);
                if (nLength == 255)
                {
                    // Normal Case
                    // // If the message is "Please Enter:"
                    // // Output the message right away
                    // if (memcmp("Please Enter:", strSocketBuffer, 13) == 0)
                    // {
                    //     cout << strSocketBuffer;
                    //     continue;
                    // }
                    des.decrypt(strSocketBuffer, decryptedtext);
                    decryptedtext[254] = 0;
                    if (decryptedtext[0] != 0 && decryptedtext[0] != '\n')
                    {
                        // Fix the problem of the sudden message at the same line of "Please Enter:"
                        cout << endl;
                        string str_time = timeNow();
                        cout << "<SecretHideout::Headquarter @ " + str_time + " # Message>: " + decryptedtext;
                        if (memcmp("OVER", decryptedtext, 4) == 0)
                        {
                            // If the message the agent received is "OVER"
                            // It means the Headquarter is done communicating
                            string str_time = timeNow();
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:The Headquarter is off the line." << endl;
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                            cout << "-----Communication OVER-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                        else if (memcmp("COMMUNICATION ERROR", decryptedtext, 19) == 0)
                        {
                            // If the message the agent received is "COMMUNICATION ERROR"
                            // It means there is a problem with the communication
                            string str_time = timeNow();
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:There is a problem with the communication..." << endl;
                            cout << "-----Shutdown Communication-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                    }
                }
                else
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<SecretHideout::System @ " + str_time + " # Message>: There is a problem with the communication..." << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    memset(encryedtext, 0, sizeof(encryedtext));
                    des.encrypt(hint, encryedtext);
                    send(socket, encryedtext, sizeof(encryedtext), 0);
                    kill(nPid, SIGTERM);
                    return;
                }
            }
        }
        else
        {
            /**
             * Child process
             * In charge of the following:
             * 1. Get the message from the agent
             * 2. Encrypt the message
             * 3. Send the message to the headquarter
             */
            char strStdinBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strStdinBuffer, 0, sizeof(strStdinBuffer));
                memset(encryedtext, 0, sizeof(encryedtext));
                memset(decryptedtext, 0, sizeof(decryptedtext));
                // Get the message from the agent
                // if (fgets(strStdinBuffer, 255, stdin) == NULL)
                // {
                //     continue;
                // }
                cout << "Please Enter:";
                cin.getline(strStdinBuffer, 255);
                // send(socket, "Please Enter:", 100, 0);
                int nLen = 255;
                // Encrypt the message
                des.encrypt(strStdinBuffer, encryedtext);
                // Send the message to the agent
                if (send(socket, encryedtext, sizeof(encryedtext), 0) != 255)
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<SecretHideout::System @ " + str_time + " # Message>: There is a problem with the communication..." << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    memset(encryedtext, 0, sizeof(encryedtext));
                    des.encrypt(hint, encryedtext);
                    send(socket, encryedtext, sizeof(encryedtext), 0);
                    // Put an end to the the parent process
                    kill(getppid(), SIGTERM);
                    return;
                }
                else
                {
                    if (memcmp("OVER", strStdinBuffer, 4) == 0)
                    {
                        // If the message the agent sent is "OVER"
                        // It means the agent is done communicating
                        string str_time = timeNow();
                        cout << "<SecretHideout::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                        cout << "-----Communication OVER-----" << endl;
                        // Tell the headquarter the communication is over
                        char *hint = "OVER";
                        memset(encryedtext, 0, sizeof(encryedtext));
                        des.encrypt(hint, encryedtext);
                        send(socket, encryedtext, sizeof(encryedtext), 0);
                        // Put an end to the the parent process
                        kill(getppid(), SIGTERM);
                        return;
                    }
                    cout << "<Headquarter::System @ " + timeNow() + " # Message>:Message Sent." << endl;
                }
            }
        }
    }
    else if (role == M16)
    {
        // This part is in the headquarter's side
        string str_time = timeNow();
        cout << "<Headquarter::System @ " + str_time + " # Message>:Silent Guardian is activated.." << endl;
        // Notify the agent to also activate the Silent Guardian
        send(socket, "Silent Guardian", 100, 0);
        pid_t nPid;
        nPid = fork();
        if (nPid != 0)
        {

            /**
             * Parent process
             * In charge of the following:
             * 1. Receive message from the agent
             * 2. Decrypt the message
             * 3. Output the message
             */
            char strSocketBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            do
            {

            } while (true);

            while (true)
            {
                memset(strSocketBuffer, 0, sizeof(strSocketBuffer));
                memset(encryedtext, 0, sizeof(encryedtext));
                memset(decryptedtext, 0, sizeof(decryptedtext));
                int nLength = 0;
                // Using the TotalRecv function to ensure all data is received
                nLength = TotalRecv(socket, strSocketBuffer, 255, 0);
                if (nLength == 255)
                {
                    // Normal Case
                    // // If the message is "Please Enter:"
                    // // Output the message right away
                    // if (memcmp("Please Enter:", strSocketBuffer, 13) == 0)
                    // {
                    //     cout << strSocketBuffer;
                    //     continue;
                    // }
                    des.decrypt(strSocketBuffer, decryptedtext);
                    decryptedtext[254] = 0;
                    if (decryptedtext[0] != 0 && decryptedtext[0] != '\n')
                    {
                        // Fix the problem of the sudden message at the same line of "Please Enter:"
                        cout << endl;
                        string str_time = timeNow();
                        cout << "<Headquarter::" + codeName + " @ " + str_time + " # Message>: " + decryptedtext;
                        if (memcmp("OVER", decryptedtext, 4) == 0)
                        {
                            // If the message the headquarter received is "OVER"
                            // It means the agent is done communicating
                            string str_time = timeNow();
                            cout << "<Headquarter::System @ " + str_time + " # Message>:Agent OUT." << endl;
                            cout << "<Headquarter::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                            cout << "-----Communication OVER-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                        else if (memcmp("COMMUNICATION ERROR", decryptedtext, 19) == 0)
                        {
                            // If the message the headquarter received is "COMMUNICATION ERROR"
                            // It means there is a problem with the communication
                            string str_time = timeNow();
                            cout << "<Headquarter::System @ " + str_time + " # Message>:There is a problem with the communication..." << endl;
                            cout << "-----Shutdown Communication-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                    }
                }
                else
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<Headquarter::System @ " + str_time + " # Message>: There is a problem with the communication..." << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    memset(encryedtext, 0, sizeof(encryedtext));
                    des.encrypt(hint, encryedtext);
                    send(socket, encryedtext, sizeof(encryedtext), 0);
                    kill(nPid, SIGTERM);
                    return;
                }
            }
        }
        else
        {
            /**
             * Child process
             * In charge of the following:
             * 1. Get the message from the headquarter
             * 2. Encrypt the message
             * 3. Send the message to the agent
             */
            char strStdinBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strStdinBuffer, 0, sizeof(strStdinBuffer));
                memset(encryedtext, 0, sizeof(encryedtext));
                memset(decryptedtext, 0, sizeof(decryptedtext));
                // Get the message from the headquarter
                // if (fgets(strStdinBuffer, 255, stdin) == NULL)
                // {
                //     continue;
                // }
                cout << "Please Enter:";
                cin.getline(strStdinBuffer, 255);
                // send(socket, "Please Enter:", 100, 0);
                int nLen = 255;
                // Encrypt the message
                des.encrypt(strStdinBuffer, encryedtext);
                // Send the message to the agent
                if (send(socket, encryedtext, sizeof(encryedtext), 0) != 255)
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<Headquarter::System @ " + str_time + " # Message>: There is a problem with the communication..." << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    memset(encryedtext, 0, sizeof(encryedtext));
                    des.encrypt(hint, encryedtext);
                    send(socket, encryedtext, sizeof(encryedtext), 0);
                    // Put an end to the the parent process
                    kill(getppid(), SIGTERM);
                    return;
                }
                else
                {
                    if (memcmp("OVER", strStdinBuffer, 4) == 0)
                    {
                        // If the message the headquarter sent is "OVER"
                        // It means the headquarter is done communicating
                        string str_time = timeNow();
                        cout << "<Headquarter::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                        cout << "-----Communication OVER-----" << endl;
                        // Tell the agent the communication is over
                        char *hint = "OVER";
                        memset(encryedtext, 0, sizeof(encryedtext));
                        des.encrypt(hint, encryedtext);
                        send(socket, encryedtext, sizeof(encryedtext), 0);
                        // Put an end to the the parent process
                        kill(getppid(), SIGTERM);
                        return;
                    }
                }
                cout << "<Headquarter::System @ " + timeNow() + " # Message>:Message Sent." << endl;
            }
        }
    }
    return;
}