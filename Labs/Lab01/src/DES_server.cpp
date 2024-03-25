#include "DES_server.h"

void emergencyResponse()
{
    /**
     * This is a function to handle emergency for the commnad center
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
    numKey = 0;
    for (int i = 0; i < 8; ++i)
    {
        numKey <<= 8;
        numKey |= static_cast<unsigned char>(key[i]);
    }
    des.genKey(&numKey);
    memset(cipheredtext, 0, 64);
    memset(agentcodeName, 0, 64);
    // Agent's confidential information
    for (int i = 0; i < sizeof(agentNames) / sizeof(agentNames[0]); i++)
        agentConfidential[agentPasswords[i]] = agentNames[i];
    // Easter Egg Part
    string str_time = timeNow();
    cout << "<-----------M16 Headquarter-----------" << endl;
    cout << "<Headquarter::System @ " + str_time + " # Message>:May I have your name, sir?" << endl;
    cout << "Commander: ";
    getline(cin, commanderName);
    if (commanderName != "M")
    {
        cout << "<Headquarter::System @ " + str_time + " # Message>:Sorry, you are not authorized to access this system!" << endl;
        exit(0);
    }
REBOOT:
    cout << "<Headquarter::System @ " + str_time + " # Message>:Welcome back , Commander M!" << endl;
    cout << "-----Commander M-----" << endl;
    cout << "<Headquarter::System @ " + str_time + " # Message>:System rebooting..." << endl;
    cout << "-----Creating Socket-----" << endl;
    // Create a socket
    sListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (sListenSocket < 0)
    {
        cout << "<Headquarter::System @ " + str_time + " # Message>:Oops!There is a fatal error in creating socket!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }
    cout << "<Headquarter::System @ " + str_time + " # Message>:Successfully creating socket!" << endl;
    // Bind the socket to the address
    cout << "-----Binding Socket-----" << endl;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sListenSocket, (sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        cout << "<Headquarter::System @ " + str_time + " # Message>:Oops!There is a fatal error in binding socket!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }
    cout << "<Headquarter::System @ " + str_time + " # Message>:Successfully binding socket!" << endl;
    // Listen for agents to connect
    cout << "-----Listening for Agents-----" << endl;
    if (listen(sListenSocket, MAX_CONNECTION) < 0)
    {
        cout << "<Headquarter::System @ " + str_time + " # Message>:Oops!There is a fatal error in listening for agents!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }
    cout << "<Headquarter::System @ " + str_time + " # Message>:Waiting for agents to come in..." << endl;

    while (true)
    {
        // Accept the connection from the agent
        if ((sAcceptSocket = accept(sListenSocket, (sockaddr *)&agentAddr, (socklen_t *)&agentAddr)) < 0)
        {
            cout << "<Headquarter::System @ " + str_time + " # Message>:Oops!There is a fatal error in accepting the connection!" << endl;
            emergencyResponse();
            // If the preceding function returns, the system will be rebooted
            goto REBOOT;
        }
        nPid = fork();
        if (nPid != 0)
            // Parent process, continue to accept the connection
            continue;
        else
        {
            // Child process, handle the connection
            // For every agent, we will create a new process to handle the connection
            close(sListenSocket);
            printf("<Headquarter::System @ %s # Message>:We got a agent from %s, port %d, socket %d\n", str_time.c_str(), inet_ntoa(agentAddr.sin_addr), ntohs(agentAddr.sin_port), sAcceptSocket);

            // Receive the code name of the agent
            recv(sAcceptSocket, agentcodeName, 64, 0);
            cout << "<Headquarter::System @ " + str_time + " # Message>:Agent code name:" + agentcodeName + "." << endl;
            // Push the agent into the vector
            {
                lock_guard<mutex> lock(agentsMtx);
                agents.push_back(Agent(agentConfidential[agentcodeName], agentcodeName, sAcceptSocket));
            }
            // Fake a emergency
            sleep(3);
            // Earshot warning
            cout << "<Headquarter::System @ " + str_time + " # WARNING>:The enemy is potentially in earshot. Be careful!" << endl;
            while (true)
            {
                cout << "<Headquarter::System @ " + str_time + " # Message>:Please give us further instructions!" << endl;
                cout << "1:Let's GO DARK! Slient Guardian, ACTIVATE!" << endl;
                cout << "2:Forget about their existence." << endl;
                cout << "3:Terminate the connection! ASAP!" << endl;
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
                    cout << "<Headquarter::System @ " + str_time + " # Message>:Yes, sir!" << endl;
                    /**
                     * Secret Code Name:Silent Guardian
                     * This is a secret communicating channel for the headquarter and the agent
                     * In case there is enemy's interception, this will be activated to ensure the safety of the communication
                     */
                    SilentGuardian(M16, sAcceptSocket);
                    return 0;
                    break;
                case 2:
                    cout << "<Headquarter::System @ " + str_time + " # Message>:Yes, sir!" << endl;
                    /** Normally communicating without any encryption
                     * This could be a potential threat to the system
                     * But we can also use this as a trap to catch the enemy
                     * Like a bait to lure the enemy in
                     * And then we can take them down
                     * This is a risky move, but it could be a potential strategy
                     */
                    PhantomHook(M16, sAcceptSocket);

                    return 0;
                    break;
                case 3:
                    cout << "<Headquarter::System @ " + str_time + " # Message>:System aborted and files all in ash!" << endl;
                    /**
                     * In case the system is compromised
                     * We have to abort the system and destory all files
                     * This is the last resort
                     */
                    // send(sAcceptSocket, "System aborted and files all in ash!", 100, 0);

                    des.encrypt("System aborted and files all in ash!", cipheredtext);
                    send(sAcceptSocket, cipheredtext, 255, 0);
                    close(sAcceptSocket);
                    exit(0);
                    break;
                default:
                    cout << "<Headquarter::System @ " + str_time + " # Message>:Sorry, sir! I am not following you." << endl;
                    break;
                }
            }
        }
    }

    return 0;
}
