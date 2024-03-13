#include "DES_server.h"
int main()
{
    // Easter Egg Part
    cout << "-----------M16 Headquarter-----------" << endl;
    cout << "May I have your name, sir?" << endl;
    cout << "Commander: ";
    cin >> commanderName;
    if (commanderName != "M")
    {
        cout << "Sorry, you are not authorized to access this system!" << endl;
        exit(0);
    }
REBOOT:
    cout << "Welcome back , Commander M!" << endl;
    cout << "-----Commander M-----" << endl;
    cout << "System rebooting..." << endl;
    cout << "-----Creating Socket-----" << endl;
    // Create a socket
    sListenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (sListenSocket > 0)
    {
        cout << "Oops!There is a fatal error in creating socket!" << endl;
        cout << "We must have been compromised!" << endl;
        cout << "Commander, please give us further instructions!" << endl;
        cout << "1:Abort the system and destory all files, NOW!" << endl;
        cout << "2:Reboot the system." << endl;
        while (1)
        {
            cin >> instruction;
            switch (instruction)
            {
            case 1:
                cout << "System aborted and files all in ash!" << endl;
                exit(0);
                break;
            case 2:
                cout << "Yes, Commander!" << endl;
                goto REBOOT;
                break;
            default:
                cout << "Sorry, sir! I am not following you." << endl;
                break;
            }
        }
    }
    cout << "Successfully creating socket!" << endl;
    // Bind the socket to the address
    cout << "-----Binding Socket-----" << endl;
    if (bind(sListenSocket, (sockaddr *)&serverAddr, sizeof(struct sockaddr)) < 0)
    {
        cout << "Oops!There is a fatal error in binding socket!" << endl;
        cout << "We must have been compromised!" << endl;
        cout << "Commander, please give us further instructions!" << endl;
        cout << "1:Abort the system and destory all files, NOW!" << endl;
        cout << "2:Reboot the system." << endl;
        while (1)
        {
            cin >> instruction;
            switch (instruction)
            {
            case 1:
                cout << "System aborted and files all in ash!" << endl;
                exit(0);
                break;
            case 2:
                cout << "Yes, Commander!" << endl;
                goto REBOOT;
                break;
            default:
                cout << "Sorry, sir! I am not following you." << endl;
                break;
            }
        }
    }
    cout << "Successfully binding socket!" << endl;
    // Listen for agents to connect
    cout << "-----Listening for Agents-----" << endl;
}