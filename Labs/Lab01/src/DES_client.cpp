#include "DES_client.h"

void phantomHook(int role)
{
    /*
     * Secret Code Name:Phantom Hook
     * This is a bait to lure the enemy in and then we can take them down
     * We will use this as a trap to catch the enemy by communicating without any encryption
     * role: 0 for agent, 1 for headquarter
     */
    if (role == M16)
    {
        cout << "007, we’re reading you loud and clear. Noting happened. Don't worry.Proceed with your update. Over." << endl;
    }
    else
    {
        cout << "Phantom Hook is activated. Please proceed with your update." << endl;
    }
}
void silentGuardain(int role)
{
    /*
     * Secret Code Name:Silent Guardian
     * This is a secret communicating channel for the headquarter and the agent
     * In case there is enemy's interception, this will be activated to ensure the safety of the communication
     * The communication is based on the DES algorithm
     * role: 0 for agent, 1 for headquarter
     */
    if (role == 0)
    {
        cout << "007, we’re reading you loud and clear on Silent Guardian. All other channels are compromised. Proceed with your update. Over." << endl;
    }
    else
    {
        cout << "Silent Guardian is activated. Please proceed with your update." << endl;
    }
}
int main()
{

    // Easter Egg Part
    cout << "-----------Secret Hideout-----------" << endl;
    cout << "[SYSTEM]Stop there mate! Who are you?" << endl;
    cout << "Well, I am:";
    getline(cin, agentName);
    if (agentName != "James Bond")
    {
        cout << "[SYSTEM]Sorry, you are not allowed to enter!" << endl;
        cout << "[SYSTEM]This is a restricted area!" << endl;
        cout << "[SYSTEM]Run for your life, outlaw!" << endl;
        exit(0);
    }
    cout << "[SYSTEM]Wecome back, Mr.James Bond!" << endl;
    cout << "-----Agent:007-----" << endl;
    cout << "[SYSTEM]The mission is almost ready to launch!" << endl;
    cout << "[SYSTEM]Please wait for a moment, Mr.Bond!" << endl;
    cout << "-----Creating Socket-----" << endl;
    // Create a socket
    agentSocket = socket(AF_INET, SOCK_STREAM, 0);
    // Test if the socket is created successfully
    if (agentSocket < 0)
    {
        cout << "[SYSTEM]Oops!There is a fatal error in creating socket!" << endl;
        cout << "[SYSTEM]Report this issue to the headquarter immediately!" << endl;
        exit(0);
    }
    cout << "[SYSTEM]Successfully creating socket!" << endl;
    cout << "[SYSTEM]Agent, please input the IP adress of the Headquarter in M16:" << endl;
    while (1)
    {
        cout << "[SYSTEM]1:Use the Default IP Address" << endl;
        cout << "[SYSTEM]2:Input the IP Address Manually" << endl;
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();                                         // 清除错误状态
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略错误的输入
            cout << "[SYSTEM]Invalid input. Please enter a number." << endl;
            continue;
        }
        switch (choice)
        {
        case 1:
            break;
        case 2:
            cout << "[SYSTEM]Please input the IP address of the Headquarter in M16:" << endl;
            cin >> serverIP;
            break;
        default:
            cout << "[SYSTEM]Sorry, sir! I am not following you." << endl;
            continue;
        }
        break;
    }
    // Set the address of the server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());

    // Connect to the server
    cout << "-----Connecting to the Headquarter-----" << endl;
    if (connect(agentSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cout << "[SYSTEM]Oops!There is a fatal error in connecting to the Headquarter!" << endl;
        cout << "[SYSTEM]Report this issue to the headquarter immediately!" << endl;
        exit(0);
    }
    cout << "[SYSTEM]Successfully connected to the Headquarter!" << endl;

    cout << "[WARNING]The enemy is potentially in earshot. We are under surveillance, be careful!" << endl;
    while (1)
    {
        recv(agentSocket, strSocketBuffer, BUFFERSIZE, 0);
        if (strSocketBuffer != nullptr && strlen(strSocketBuffer) > 0)
        {
            if (strcmp(strSocketBuffer, "Phantom Hook") == 0)
            {
                // Activate the Phantom Hook
                phantomHook(AGENT);
                close(agentSocket);
                return 0;
            }
            else if (strcmp(strSocketBuffer, "Silent Guardian") == 0)
            {
                // Activate the Silent Guardian
                silentGuardain(AGENT);
                close(agentSocket);
                return 0;
            }
            else
            {
                if (strcmp(strSocketBuffer, "quit") == 0)
                {
                    cout << "Quit!" << endl;
                    break;
                }
                // Receive the message from the M16
                cout << "Receive message from M16: " << strSocketBuffer << endl;
            }
        }
    }
    close(agentSocket);
    return 0;
}