#include "DES_server.h"

void handle_sigint(int sig)
{
    /**
     * This is a function to handle the signal of Ctrl+C
     * To resolve the Parent Process holding socket WHILE dying issue
     * @param sig: the signal of Ctrl+C
     * @return void
     */
    close(sListenSocket);
    exit(0);
}
void emergencyResponse()
{
    /**
     * This is a function to handle emergency for the commnad center
     * @return void
     */
    string str_time = timeNow();
    cout << "<Headquarter::System @ " + str_time + " # Message>:We must have been compromised!" << endl;
    cout << "<Headquarter::System @ " + str_time + " # Message>:Commander, please give us further instructions!" << endl;
    cout << "1:Abort the system and destory all files, NOW!" << endl;
    cout << "2:Reboot the system." << endl;
    while (true)
    {
        cin >> instruction;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "<Headquarter::System @ " + str_time + " # Message>:Invalid input. Please enter a number." << endl;
            continue;
        }
        switch (instruction)
        {
        case 1:
            cout << "<Headquarter::System @ " + str_time + " # Message>:System aborted and files all in ash!" << endl;
            exit(0);
            break;
        case 2:
            cout << "<Headquarter::System @ " + str_time + " # Message>:Yes, Commander!" << endl;
            return;
            break;
        default:
            cout << "<Headquarter::System @ " + str_time + " # Message>:Sorry, sir! I am not following you." << endl;
            break;
        }
    }
}

int main()
{
    // Initializations
    signal(SIGINT, handle_sigint);
    timeout.tv_sec = 300; // After 300 seconds, the socket will be closed automatically
    timeout.tv_usec = 0;
    numKey = 0;
    for (int i = 0; i < 8; ++i)
    {
        numKey <<= 8;
        numKey |= static_cast<unsigned char>(key[i]);
    }
    des.genKey(&numKey);
    memset(cipheredtext, 0, 64);
    memset(agentCodeName, 0, 64);
    // Agent's confidential information
    for (int i = 0; i < sizeof(agentNames) / sizeof(agentNames[0]); i++)
        agentConfidential[agentPasswords[i]] = agentNames[i];
    // Easter Egg Part
    string str_time = timeNow();
    cout << "-----------M16 Headquarter-----------" << endl;
    cout << "<Headquarter::System @ " + str_time + " # Message>:May I have your name, sir?" << endl;
    cout << "Commander: ";
    getline(cin, commanderName);
    if (commanderName != "M")
    {
        cout << "<Headquarter::System @ " + str_time + " # Message>:Sorry, you are not authorized to access this system!" << endl;
        exit(0);
    }
REBOOT:
    string str_time1 = timeNow();
    cout << "<Headquarter::System @ " + str_time1 + " # Message>:Welcome back , Commander M!" << endl;
    cout << "-----Commander M-----" << endl;
    cout << "<Headquarter::System @ " + str_time1 + " # Message>:System rebooting..." << endl;
    cout << "-----Creating Socket-----" << endl;
    // Create a socket
    sListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (sListenSocket < 0)
    {
        cout << "<Headquarter::System @ " + str_time1 + " # Message>:Oops!There is a fatal error in creating socket!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }
    cout << "<Headquarter::System @ " + str_time1 + " # Message>:Successfully creating socket!" << endl;
    if (setsockopt(sListenSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
    {
        cout << "<Headquarter::System @ " + str_time1 + " # Message>:Oops!There is a fatal error in setting socket!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }

    if (setsockopt(sListenSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
    {
        cout << "<Headquarter::System @ " + str_time1 + " # Message>:Oops!There is a fatal error in setting socket!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }
    // Bind the socket to the address
    cout << "-----Binding Socket-----" << endl;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT + 5);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sListenSocket, (sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        cout << "<Headquarter::System @ " + str_time1 + " # Message>:Oops!There is a fatal error in binding socket!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }
    string str_time2 = timeNow();
    cout << "<Headquarter::System @ " + str_time2 + " # Message>:Successfully binding socket!" << endl;
    // Listen for agents to connect
    cout << "-----Listening for Agents-----" << endl;
    if (listen(sListenSocket, MAX_CONNECTION) < 0)
    {
        cout << "<Headquarter::System @ " + str_time2 + " # Message>:Oops!There is a fatal error in listening for agents!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }

    while (true)
    {
        // Accept the connection from the agent
        string str_time3 = timeNow();
        cout << "<Headquarter::System @ " + str_time3 + " # Message>:Waiting for agents to come in..." << endl;
        if ((sAcceptSocket = accept(sListenSocket, (sockaddr *)&agentAddr, (socklen_t *)&agentAddr)) < 0)
        {
            if (errno == EWOULDBLOCK)
            {
                /**
                 * TIMEOUT
                 * If the interval between connections is too long
                 * The SYSTEM will wait for the last agent to leave
                 * And then the SYSTEM will go down to save energy
                 */
                cout << "<Headquarter::System @ " + str_time3 + " # WARNING>:Too long intervals between connections." << endl;
                cout << "<Headquarter::System @ " + str_time3 + " # WARNING>:The main system will go down." << endl;
                int status;
                waitpid(nPid, &status, 0);
                cout << "----------SYSTEM SHUTDOWN----------" << endl;
                close(sListenSocket);
                exit(0);
            }
            else
            {
                // Other fatal errors except for the interval between connections
                cout << "<Headquarter::System @ " + str_time3 + " # Message>:Oops!There is a fatal error in accepting the connection!" << endl;
                emergencyResponse();
                // If the preceding function returns, the system will be rebooted
                goto REBOOT;
            }
        }
        nPid = fork();
        if (nPid != 0)
        {
            // Parent process, busy waiting for the agent to leave
            int status;
            waitpid(nPid, &status, 0);
            continue;
        }
        else
        {
            // Child process, handle the connection
            // For every agent, we will create a new process to handle the connection
            close(sListenSocket);
            string str_time4 = timeNow();
            printf("<Headquarter::System @ %s # Message>:We got a agent from %s, port %d, socket %d\n", str_time4.c_str(), inet_ntoa(agentAddr.sin_addr), ntohs(agentAddr.sin_port), sAcceptSocket);
            mutex mtx;
            {
                lock_guard<mutex> lock(mtx);
                recv(sAcceptSocket, agentCodeName, 64, 0);
                cout << "<Headquarter::System @ " + str_time4 + " # Message>:Agent code name:" + agentCodeName + "." << endl;
            }
            // Receive the code name of the agent
            {
                lock_guard<mutex> lock(agentsMtx);
                for (auto &agent : agents)
                {
                    if (agent.getAgentCodeName() == agentCodeName)
                    {
                        /**
                         * If the agent is already in the system
                         * That means the agent is a FAKE one
                         * TETMINATE THE CONNECTION
                         * Tell the secret hideout to disconnect the person too
                         */
                        string str_time5 = timeNow();
                        cout << "<Headquarter::System @ " + str_time5 + " # Message>:Agent " + agentCodeName + " is already in the system." << endl;
                        // ENCRYPT THE MESSAGE so that the enemy cannot be aware of the situation
                        des.encrypt("FAKE IDENTITY", cipheredtext);
                        send(sAcceptSocket, cipheredtext, 100, 0);
                        close(sAcceptSocket);
                        exit(0);
                    }
                }
            }

            // Push the agent into the vector
            Agent agent(agentConfidential[agentCodeName], agentCodeName, sAcceptSocket);
            {
                lock_guard<mutex> lock(agentsMtx);
                agents.push_back(agent);
            }
            // Fake a emergency
            sleep(3);
            // Earshot warning
            string str_time6 = timeNow();
            cout << "<Headquarter::System @ " + str_time6 + " # WARNING>:The enemy is potentially in earshot. Be careful!" << endl;
            while (true)
            {
                string str_time7 = timeNow();
                cout << "<Headquarter::System @ " + str_time7 + " # Message>:Please give us further instructions!" << endl;
                cout << "1:Let's GO DARK! Slient Guardian, ACTIVATE!" << endl;
                cout << "2:Forget about their existence." << endl;
                cout << "3:Terminate the connection! ASAP!" << endl;
                cin >> instruction;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    string str_time8 = timeNow();
                    cout << "<Headquarter::System @ " + str_time8 + " # Message>:Invalid input. Please enter a number." << endl;
                    continue;
                }
                string str_time9 = timeNow();
                switch (instruction)
                {
                case 1:
                    cout << "<Headquarter::System @ " + str_time9 + " # Message>:Yes, sir!" << endl;
                    /**
                     * Secret Code Name:Silent Guardian
                     * This is a secret communicating channel for the headquarter and the agent
                     * In case there is enemy's interception, this will be activated to ensure the safety of the communication
                     */

                    SilentGuardian(M16, agent, des);
                    return 0;
                    break;
                case 2:
                    cout << "<Headquarter::System @ " + str_time9 + " # Message>:Yes, sir!" << endl;
                    /** Normally communicating without any encryption
                     * This could be a potential threat to the system
                     * But we can also use this as a trap to catch the enemy
                     * Like a bait to lure the enemy in
                     * And then we can take them down
                     * This is a risky move, but it could be a potential strategy
                     */
                    PhantomHook(M16, agent);

                    return 0;
                    break;
                case 3:
                    cout << "<Headquarter::System @ " + str_time9 + " # Message>:System aborted and files all in ash!" << endl;
                    /**
                     * In case the system is compromised
                     * We have to abort the system and destory all files
                     * This is the last resort
                     */

                    des.encrypt("SYSTEM DOWN", cipheredtext);
                    send(sAcceptSocket, cipheredtext, 255, 0);
                    close(sAcceptSocket);
                    exit(0);
                    break;
                default:
                    cout << "<Headquarter::System @ " + str_time9 + " # Message>:Sorry, sir! I am not following you." << endl;
                    break;
                }
            }
        }
    }

    return 0;
}
