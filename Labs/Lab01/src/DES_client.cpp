#include "DES_client.h"
int main()
{

    // Easter Egg Part
    cout << "-----------Secret Hideout-----------" << endl;
    string str_time = timeNow();
    cout << "<Headquarter::System @ " + str_time + " # Message>:Stop there mate! Who are you?" << endl;
    cout << "Well, I am:";
    getline(cin, agentName);
    if (agentName != "James Bond")
    {
        str_time = timeNow();
        cout << "<Headquarter::System @ " + str_time + " # Message>:Sorry, you are not allowed to enter!" << endl;
        cout << "<Headquarter::System @ " + str_time + " # Message>:This is a restricted area!" << endl;
        cout << "<Headquarter::System @ " + str_time + " # Message>:Run for your life, outlaw!" << endl;
        exit(0);
    }
    str_time = timeNow();
    cout << "<Headquarter::System @ " + str_time + " # Message>:Welcome back, Mr.James Bond!" << endl;
    cout << "-----Agent:007-----" << endl;
    cout << "<Headquarter::System @ " + str_time + " # Message>:The mission is almost ready to launch !" << endl;
    cout << "<Headquarter::System @ " + str_time + " # Message>:Please wait for a moment, Mr.Bond!" << endl;
    cout << "-----Creating Socket-----" << endl;
    // Create a socket
    agentSocket = socket(AF_INET, SOCK_STREAM, 0);
    // Test if the socket is created successfully
    if (agentSocket < 0)
    {
        str_time = timeNow();
        cout << "<Headquarter::System @ " + str_time + " # Message>:Oops!There is a fatal error in creating socket!" << endl;
        cout << "<Headquarter::System @ " + str_time + " # Message>:Report this issue to the headquarter immediately!" << endl;
        exit(0);
    }
    str_time = timeNow();
    cout << "<Headquarter::System @ " + str_time + " # Message>:Successfully creating socket!" << endl;
    cout << "<Headquarter::System @ " + str_time + " # Message>:Agent, please input the IP adress of the Headquarter in M16:" << endl;
    while (1)
    {
        cout << "<Headquarter::System @ " + str_time + " # Message>:1:Use the Default IP Address" << endl;
        cout << "<Headquarter::System @ " + str_time + " # Message>:2:Input the IP Address Manually" << endl;
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();                                         // 清除错误状态
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 忽略错误的输入
            str_time = timeNow();
            cout << "<Headquarter::System @ " + str_time + " # Message>:Invalid input. Please enter a number." << endl;
            continue;
        }
        switch (choice)
        {
        case 1:
            break;
        case 2:
            cout << "<Headquarter::System @ " + str_time + " # Message>:Please input the IP address of the Headquarter in M16:" << endl;
            cin >> serverIP;
            break;
        default:
            str_time = timeNow();
            cout << "<Headquarter::System @ " + str_time + " # Message>:Sorry, sir! I am not following you." << endl;
            continue;
        }
        break;
    }
    // Set the address of the server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());

    // Connect to the server
    str_time = timeNow();
    cout << "<Headquarter::System @ " + str_time + " # Message>:Connecting to the Headquarter-----" << endl;
    if (connect(agentSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        str_time = timeNow();
        cout << "<Headquarter::System @ " + str_time + " # Message>:Oops!There is a fatal error in connecting to the Headquarter!" << endl;
        cout << "<Headquarter::System @ " + str_time + " # Message>:Report this issue to the headquarter immediately!" << endl;
        exit(0);
    }
    str_time = timeNow();
    cout << "<Headquarter::System @ " + str_time + " # Message>:Successfully connected to the Headquarter!" << endl;

    str_time = timeNow();
    cout << "<Headquarter::System @ " + str_time + " # Warning>:The enemy is potentially in earshot. We are under surveillance, be careful!" << endl;
    while (1)
    {
        recv(agentSocket, strSocketBuffer, BUFFERSIZE, 0);
        if (strSocketBuffer != nullptr && strlen(strSocketBuffer) > 0)
        {
            if (strcmp(strSocketBuffer, "Phantom Hook") == 0)
            {
                // Activate the Phantom Hook
                PhantomHook(AGENT, agentSocket);
                close(agentSocket);
                return 0;
            }
            else if (strcmp(strSocketBuffer, "Silent Guardian") == 0)
            {
                // Activate the Silent Guardian
                SilentGuardian(AGENT, agentSocket);
                close(agentSocket);
                return 0;
            }
            else
            {

                DESUtils des;
                char *key = "YCZhouNB";
                uint64_t numKey = 0;
                for (int i = 0; i < 8; ++i)
                {
                    numKey <<= 8;
                    numKey |= static_cast<unsigned char>(key[i]);
                }
                des.genKey(&numKey);
                char decryptedtext[64]; // 存储解密后的文本
                memset(decryptedtext, 0, 64);
                // Receive the message from the M16
                des.decrypt(strSocketBuffer, decryptedtext);
                str_time = timeNow();
                cout << "<Headquarter::M16 @ " + str_time + " # Message> " << decryptedtext << endl;
                if (strcmp(strSocketBuffer, "System aborted and files all in ash!") == 0)
                {
                    cout << "<Headquarter::SYSTEM @ " + str_time + " # Message> " +
                                "Run away, Agent!The Headquarter is down..."
                         << endl;
                    cout << "[*%$$K#$%%SA]You are on your own now......" << endl;
                    break;
                }
                memset(strSocketBuffer, 0, BUFFERSIZE);
            }
        }
    }
    close(agentSocket);
    return 0;
}