#include "DES_server.h"

void emergencyResponse()
{
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
}