#include "DES_client.h"

int main()
{

    // Initializations
    memset(decryptedtext, 0, 64);
    // Easter Egg Part
    cout << "-----------Secret Hideout-----------" << endl;
    string str_time = timeNow();
    cout << "<SecretHideout::System @ " + str_time + " # Message>:Stop there mate! Who are you?" << endl;
    cout << "Well, I am:";
    getline(cin, agentName);
    /**
     * Check the agent's identity
     * If the agent is not one of the following, the agent is not allowed to enter
     * Because the real name of the agent is TOP confidential, only the someone with the following name is trustworthy to allowed to enter
     * small easteregg here hhh, Tom Cruise and Rowan Atkinson are the real name of the actor of the spy in the movie
     * Agent List:
     * 1. James Bond, 007
     * 2. David Webb, Jason Bourne
     * 3. Natasha Romanoff, Black Widow
     * 4. Tom Cruise, Ethan Hunt
     * 5. Rowan Atkinson, Johnny English
     */
    // From the Movie: James Bond
    if (agentName == "James Bond")
        agentCodeName = "007";
    // From the Movie: The Bourne Identity
    else if (agentName == "David Webb")
        agentCodeName = "Jason Bourne";
    // From the Movie: Avengers
    else if (agentName == "Natasha Romanoff")
        agentCodeName = "Black Widow";
    // From the Movie: Mission Impossible
    else if (agentName == "Tom Cruise")
        agentCodeName = "Ethan Hunt";
    // From the Movie: Johnny English
    else if (agentName == "Rowan Atkinson")
        agentCodeName = "Johnny English";
    else
    {
        // If the agent is not one of the following, he has to be a intruder
        str_time = timeNow();
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Sorry, you are not allowed to enter!" << endl;
        cout << "<SecretHideout::System @ " + str_time + " # Message>:This is a restricted area!" << endl;
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Run for your life, outlaw!" << endl;
        exit(0);
    }

    str_time = timeNow();
    if (agentCodeName != "Black Widow")
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Welcome back, Mr." + agentCodeName + "!" << endl;
    else
        // Cause Natasha Natasha Romanoff is a lady
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Welcome back, Ms." + agentCodeName + "!" << endl;
    cout << "-----Agent:" + agentCodeName + "-----" << endl;
    cout << "<SecretHideout::System @ " + str_time + " # Message>:The mission is almost ready to launch !" << endl;
    if (agentCodeName == "Black Widow")
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Please wait for a moment, Ms." + agentCodeName + "!" << endl;
    else
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Please wait for a moment, Mr." + agentCodeName + "!" << endl;
    cout << "-----Creating Socket-----" << endl;
    // Create a socket
    agentSocket = socket(AF_INET, SOCK_STREAM, 0);
    // Test if the socket is created successfully
    if (agentSocket < 0)
    {
        str_time = timeNow();
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Oops!There is a fatal error in creating socket!" << endl;
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Report this issue to the headquarter immediately!" << endl;
        exit(0);
    }
    str_time = timeNow();
    cout << "<SecretHideout::System @ " + str_time + " # Message>:Successfully creating socket!" << endl;
    cout << "<SecretHideout::System @ " + str_time + " # Message>:Agent, please input the IP adress of the Headquarter in M16:" << endl;
    while (true)
    {
        cout << "<SecretHideout::System @ " + str_time + " # Message>:1:Use the Default IP Address" << endl;
        cout << "<SecretHideout::System @ " + str_time + " # Message>:2:Input the IP Address Manually" << endl;
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();                                         // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the input
            str_time = timeNow();
            cout << "<SecretHideout::System @ " + str_time + " # Message>:Invalid input. Please enter a number." << endl;
            continue;
        }
        switch (choice)
        {
        case 1:
            break;
        case 2:
            cout << "<SecretHideout::System @ " + str_time + " # Message>:Please input the IP address of the Headquarter in M16:" << endl;
            cin >> serverIP;
            break;
        default:
            str_time = timeNow();
            cout << "<SecretHideout::System @ " + str_time + " # Message>:Sorry, sir! I am not following you." << endl;
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
    cout << "-----Connecting to the Headquarter-----" << endl;
    if (connect(agentSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        str_time = timeNow();
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Oops!There is a fatal error in connecting to the Headquarter!" << endl;
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Report this issue to the headquarter immediately!" << endl;
        exit(0);
    }
    str_time = timeNow();
    cout << "<SecretHideout::System @ " + str_time + " # Message>:Successfully connected to the Headquarter!" << endl;

    // Send the codename to the headquarter
    send(agentSocket, agentCodeName.c_str(), strlen(agentCodeName.c_str()), 0);

    // Fake a emergency
    sleep(3);
    // Earshot warning
    str_time = timeNow();
    cout << "<SecretHideout::System @ " + str_time + " # Warning>:The enemy is potentially in earshot. We are under surveillance, be careful!" << endl;
    while (true)
    {
        recv(agentSocket, strSocketBuffer, BUFFERSIZE, 0);
        if (strSocketBuffer != nullptr && strlen(strSocketBuffer) > 0)
        {
            if (strcmp(strSocketBuffer, "Phantom Hook") == 0)
            {
                // Activate the Phantom Hook
                Agent agent(agentName, agentCodeName, agentSocket);
                PhantomHook(AGENT, agent);
                close(agentSocket);
                exit(0);
            }
            else if (strcmp(strSocketBuffer, "Silent Guardian") == 0)
            {
                memset(strSocketBuffer, 0, BUFFERSIZE);
                // Activate the Silent Guardian
                Agent agent(agentName, agentCodeName, agentSocket);
                cout << "<SecretHideout::System @ " + timeNow() + " # Message>:All channels have been compromised!" << endl;
                cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Silent Guardian is activated.." << endl;
                cout << "-----Waiting for RSA Key-----" << endl;

                if (recv(agentSocket, strSocketBuffer, BUFFERSIZE, 0) < 0)
                {
                    cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Error in receiving RSA Public Key!" << endl;
                    cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Please report this issue to the headquarter immediately!" << endl;
                    close(agentSocket);
                    exit(0);
                }
                else
                    cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Successfully received the RSA Public Key!" << endl;
                // The RSA Public Key should be in the format of "(e, n)"
                // Extract the RSA Public Key
                uint64_t e, n;
                sscanf(strSocketBuffer, "(%lu, %lu)", &e, &n);
                // Print the RSA Public Key
                // cout << "<SecretHideout::System @ " + timeNow() + " # Message>:RSA Public Key: (" << e << ", " << n << ")" << endl;
                rsa.setPublicKey(e, n);
                // Activate the Silent Guardian
                SilentGuardian(AGENT, agent, des, rsa);
                // SilentGuardian_AIO(AGENT, agent, des, rsa);
                close(agentSocket);
                exit(0);
            }
            else
            {
                // Receive the message from the M16
                des.decrypt(strSocketBuffer, decryptedtext);
                str_time = timeNow();

                if (strcmp(decryptedtext, "SYSTEM DOWN") == 0)
                {
                    cout << "<SecretHideout::System @ " + str_time + " # WARNING> " +
                                "Run away, Agent!The Headquarter is down..."
                         << endl;
                    cout << "[*%$$K#$%%SA]You are on your own now......" << endl;
                    close(agentSocket);
                    exit(0);
                }
                else if (strcmp(decryptedtext, "FAKE IDENTITY") == 0)
                {
                    /**
                     * IF the Headquarter is telling back the current person using the secret hideout is ACTUALLY FAKE
                     * We do not distrub the intruder, we need to let him assume that he is still safe here
                     * But we need to close the connection
                     * The M16 will take care of the rest, sending someone to catch the intruder near the hideout
                     */
                    cout << "<SecretHideout::System @ " + str_time + " # Message> " +
                                "I'm sorry, the Headquarter is not available right now..."
                         << endl;
                    cout << "-----Connection Closed-----" << endl;
                    close(agentSocket);
                    exit(0);
                }
                cout << "<SecretHideout::Headquarter @ " + str_time + " # Message> " << decryptedtext << endl;
                memset(strSocketBuffer, 0, BUFFERSIZE);
                memset(decryptedtext, 0, 64);
            }
        }
    }
    close(agentSocket);
    return 0;
}