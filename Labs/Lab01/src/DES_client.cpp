#include "DES_client.h"
using namespace std;
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
    while (1)
        ;
    return 0;
}