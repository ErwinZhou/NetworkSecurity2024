#include "DES_server.h"

void emergencyResponse()
{
    /**
     * This is a function to handle emergency for the commnad center
     */
    cout << "We must have been compromised!" << endl;
    cout << "Commander, please give us further instructions!" << endl;
    cout << "1:Abort the system and destory all files, NOW!" << endl;
    cout << "2:Reboot the system." << endl;
    while (1)
    {
        cin >> instruction;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        switch (instruction)
        {
        case 1:
            cout << "System aborted and files all in ash!" << endl;
            exit(0);
            break;
        case 2:
            cout << "Yes, Commander!" << endl;
            return;
            break;
        default:
            cout << "Sorry, sir! I am not following you." << endl;
            break;
        }
    }
}
void phantomHook(int role)
{
    /*
     * Secret Code Name:Phantom Hook
     * This is a bait to lure the enemy in and then we can take them down
     * We will use this as a trap to catch the enemy by communicating without any encryption
     * role: AGENT for agent, M16 for headquarter
     */
    if (role == AGENT)
    {
        // This part is in the agent's side
        cout << "007, we’re reading you loud and clear. Noting happened. Don't worry.Proceed with your update. Over." << endl;
    }
    else
    {
        // This part is in the headquarter's side
        cout << "Phantom Hook is activated." << endl;
        // Notify the agent to also activate the Phantom Hook
        send(sAcceptSocket, "Phantom Hook", 100, 0);
        pid_t nPid;
        nPid = fork();
    }
}
void silentGuardain(int role)
{
    /*
     * Secret Code Name:Silent Guardian
     * This is a secret communicating channel for the headquarter and the agent
     * In case there is enemy's interception, this will be activated to ensure the safety of the communication
     * The communication is based on the DES algorithm
     * role: AGENT for agent, M16 for headquarter
     */
    if (role == AGENT)
    {
        // This part is in the agent's side
        cout << "007, we’re reading you loud and clear on Silent Guardian. All other channels are compromised. Proceed with your update. Over." << endl;
    }
    else
    {
        // This part is in the headquarter's side
        cout << "Silent Guardian is activated.." << endl;
        // Notify the agent to also activate the Silent Guardian
        send(sAcceptSocket, "Silent Guardian", 100, 0);
        pid_t nPid;
        nPid = fork();
    }
}
int main()
{
    // Easter Egg Part
    cout << "-----------M16 Headquarter-----------" << endl;
    cout << "[SYSTEM]May I have your name, sir?" << endl;
    cout << "Commander: ";
    getline(cin, commanderName);
    if (commanderName != "M")
    {
        cout << "Sorry, you are not authorized to access this system!" << endl;
        exit(0);
    }
REBOOT:
    cout << "[SYSTEM]Welcome back , Commander M!" << endl;
    cout << "-----Commander M-----" << endl;
    cout << "[SYSTEM]System rebooting..." << endl;
    cout << "-----Creating Socket-----" << endl;
    // Create a socket
    sListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (sListenSocket < 0)
    {
        cout << "[SYSTEM]Oops!There is a fatal error in creating socket!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }
    cout << "[SYSTEM]Successfully creating socket!" << endl;
    // Bind the socket to the address
    cout << "-----Binding Socket-----" << endl;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sListenSocket, (sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        cout << "[SYSTEM]Oops!There is a fatal error in binding socket!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }
    cout << "[SYSTEM]Successfully binding socket!" << endl;
    // Listen for agents to connect
    cout << "-----Listening for Agents-----" << endl;
    if (listen(sListenSocket, MAX_CONNECTION) < 0)
    {
        cout << "[SYSTEM]Oops!There is a fatal error in listening for agents!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }
    cout << "[SYSTEM]Waiting for agents to come in..." << endl;
    // Accept the connection from the agent
    if ((sAcceptSocket = accept(sListenSocket, (sockaddr *)&agentAddr, (socklen_t *)&agentAddr)) < 0)
    {
        cout << "[SYSTEM]Oops!There is a fatal error in accepting the connection!" << endl;
        emergencyResponse();
        // If the preceding function returns, the system will be rebooted
        goto REBOOT;
    }
    close(sListenSocket);
    printf("[SYSTEM]We got a agent from %s, port %d, socket %d\n", inet_ntoa(agentAddr.sin_addr), ntohs(agentAddr.sin_port), sAcceptSocket);
    cout << "[WARNING]The enemy is potentially in earshot. Be careful!" << endl;
    while (1)
    {
        cout << "[SYSTEM]Please give us further instructions!" << endl;
        cout << "1:Let's GO DARK! Slient Guardian, ACTIVATE!" << endl;
        cout << "2:Forget about their existence." << endl;
        cout << "3:Terminate the connection! ASAP!" << endl;
        cin >> instruction;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        switch (instruction)
        {
        case 1:
            cout << "Yes, sir!" << endl;
            /**
             * Secret Code Name:Silent Guardian
             * This is a secret communicating channel for the headquarter and the agent
             * In case there is enemy's interception, this will be activated to ensure the safety of the communication
             */
            silentGuardain(M16);
            return 0;
            break;
        case 2:
            cout << "Yes, sir!" << endl;
            /** Normally communicating without any encryption
             * This could be a potential threat to the system
             * But we can also use this as a trap to catch the enemy
             * Like a bait to lure the enemy in
             * And then we can take them down
             * This is a risky move, but it could be a potential strategy
             */
            phantomHook(M16);

            return 0;
            break;
        case 3:
            cout << "System aborted and files all in ash!" << endl;
            /**
             * In case the system is compromised
             * We have to abort the system and destory all files
             * This is the last resort
             */
            send(sAcceptSocket, "System aborted and files all in ash!", 100, 0);
            close(sAcceptSocket);
            exit(0);
            break;
        default:
            cout << "Sorry, sir! I am not following you." << endl;
            break;
        }
    }
    return 0;
}
