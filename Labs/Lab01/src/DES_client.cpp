#include "DES_client.h"
using namespace std;
int main()
{
    // Easter Egg Part
    cout << "-----------Secret Hideout-----------" << endl;
    cout << "Stop there mate! Who are you?" << endl;
    cout << "Well, I am: ";
    cin >> agentName;
    if (agentName != "James Bond")
    {
        cout << "Sorry, you are not allowed to enter!" << endl;
        cout << "This is a restricted area!" << endl;
        cout << "Run for your life, outlaw!";
        exit(0);
    }
    cout << "Wecome back, Mr.James Bond!" << endl;
    cout << "-----Agent:007-----" << endl;
    cout << "The mission is almost ready to launch!" << endl;
    cout << "Please wait for a moment, Mr.Bond!" << endl;
    cout << "-----Creating Socket-----" << endl;
    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    // Test if the socket is created successfully
    if (clientSocket < 0)
    {
        cout << "Oops!There is a fatal error in creating socket!" << endl;
        cout << "Report this issue to the headquarter immediately!" << endl;
        exit(0);
    }
    cout << "Successfully creating socket!" << endl;
    cout << "Agent, please input the IP adress of the Headquarter in M16:";
    cin >> serverIP;
    // Set the address of the server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);

    // Connect to the server
    cout << "-----Connecting to the Headquarter-----" << endl;
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        cout << "Oops!There is a fatal error in connecting to the Headquarter!" << endl;
        cout << "Report this issue to the headquarter immediately!" << endl;
        exit(0);
    }
    cout << "Successfully connected to the Headquarter!" << endl;

    return 0;
}