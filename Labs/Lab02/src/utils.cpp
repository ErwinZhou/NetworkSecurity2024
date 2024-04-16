#include "utils.h"

char strStdinBuffer_AIO[BUFFERSIZE];
char strEncryptedBuffer_AIO[BUFFERSIZE];
char strDecryptedBuffer_AIO[BUFFERSIZE];

void printBits64(uint64_t num)
{
    for (int i = 63; i >= 0; --i)
    {
        cout << ((num >> i) & 1);
    }
    cout << endl;
}
void printBits32(uint32_t num)
{
    for (int i = 31; i >= 0; --i)
    {
        cout << ((num >> i) & 1);
    }
    cout << endl;
}
string hexToBinary(const string &hex)
{
    bitset<16> set(stoull(hex, nullptr, 16));
    return set.to_string();
}
string stringBits64(uint64_t num)
{
    string binary = "";
    for (int i = 63; i >= 0; --i)
    {
        binary += to_string((num >> i) & 1);
    }
    return binary;
}
string stringBits32(uint32_t num)
{
    string binary = "";
    for (int i = 31; i >= 0; --i)
    {
        binary += to_string((num >> i) & 1);
    }
    return binary;
}
string binaryToHex(const string &binary)
{
    bitset<64> set(binary);
    stringstream ss;
    ss << hex << set.to_ullong();
    return ss.str();
}
string timeNow()
{
    /**
     * Get the current time
     * @return string
     */
    auto now = chrono::system_clock::now();
    time_t current_time = chrono::system_clock::to_time_t(now);

    string timeString = ctime(&current_time);
    timeString.pop_back();
    return timeString;
}
void signalHandler(int sig)
{
    /**
     * Signal handler: Used for the one process to exit after receiving the signal from another
     * @param sig: signal
     * @return void
     */
    exit(0);
}
ssize_t TotalRecv(int s, void *buf, size_t len, int flags)
{
    /**
     * Receive data from the socket in case sometimes not all data is received out of change of network
     * @param s: socket
     * @param buf: buffer to store the data
     * @param len: length of the data
     * @param flags: flags
     * @return ssize_t
     */
    size_t nCurSize = 0;
    while (nCurSize < len)
    {
        ssize_t nRes = recv(s, ((char *)buf) + nCurSize, len - nCurSize, flags);
        if (nRes < 0 || nRes + nCurSize > len)
        {
            return -1;
        }
        nCurSize += nRes;
    }
    return nCurSize;
}
// StdinSingle
StdinSingle::StdinSingle(int role, Agent agent, DESUtils des, sem_t &bStop) : m_bStop(bStop), m_role(role), m_des(des)
{
    /**
     * Constructor for the StdinSingle class
     * @param role: AGENT for agent, M16 for headquarter
     * @param agent: Agent class object storing the essential information
     * @param des: DESUtils class object used for encryption and decryption
     * @param bStop: semaphore to control the communication
     */
    this->m_nSocket = agent.getAgentSocket();
    this->m_agentCodeName = agent.getAgentCodeName();
    bzero(&strStdinBuffer_AIO, sizeof(strStdinBuffer_AIO));
    this->m_pReq = new aiocb;
    bzero((char *)this->m_pReq, sizeof(struct aiocb));
    /* Assign the aio control block */

    this->m_pReq->aio_fildes = STDIN_FILENO;                                                   // Set the file descriptor to STDIN_FILENO
    this->m_pReq->aio_buf = strStdinBuffer_AIO;                                                // Set the buffer to store the input from stdin
    this->m_pReq->aio_nbytes = BUFFERSIZE;                                                     // Set the number of bytes to read from stdin
    this->m_pReq->aio_offset = 0;                                                              // Set the offset to 0
    this->m_pReq->aio_sigevent.sigev_notify = SIGEV_THREAD;                                    // Set the notification method to SIGEV_THREAD
    this->m_pReq->aio_sigevent._sigev_un._sigev_thread._function = StdinReadCompletionHandler; // Set the completion handler function to StdinReadCompletionHandler
    this->m_pReq->aio_sigevent._sigev_un._sigev_thread._attribute = NULL;                      // Set the thread attribute to NULL
    this->m_pReq->aio_sigevent.sigev_value.sival_ptr = this;                                   // Set the thread attribute to NULL
}
StdinSingle::~StdinSingle()
{
    /**
     * Destructor for the StdinSingle class
     */
    delete this->m_pReq;
    bzero(&strStdinBuffer_AIO, sizeof(strStdinBuffer_AIO));
}
void StdinSingle::StdinReadCompletionHandler(sigval_t sigval)
{
    /**
     * Completion handler for the StdinSingle class
     * @param sigval: signal value, containing the result of the asynchronous I/O
     * @return void
     */
    StdinSingle *pThis = (StdinSingle *)sigval.sival_ptr;
    if (aio_error(pThis->m_pReq) == 0)
    {
        // There is no error in the asynchronous I/O
        int nLength = aio_return(pThis->m_pReq);
        pThis->m_des.encrypt(strStdinBuffer_AIO, strEncryptedBuffer_AIO);
        SockoutSingle *pSockoutSingle = new SockoutSingle(
            pThis->m_role,
            Agent("Doesn't matter", pThis->m_agentCodeName, pThis->m_nSocket),
            pThis->m_des,
            pThis->m_bStop);
        aio_write(pSockoutSingle->m_pReq);
        if (memcmp("OVER", strStdinBuffer_AIO, 4) == 0)
        {
            // If the message the other sent is "OVER"
            // It means the end is done communicating
            if (pThis->m_role == AGENT)
                cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Communication is OVER." << endl;
            else if (pThis->m_role == M16)
                cout << "<Headquarter::System @ " + timeNow() + " # Message>:Communication is OVER." << endl;
            cout << "-----Communication OVER-----" << endl;
            // Tell the other end the communication is over
            char *hint = "OVER";
            bzero(strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO));
            pThis->m_des.encrypt(hint, strEncryptedBuffer_AIO);
            send(pThis->m_nSocket, strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO), 0);
            sem_post(&pThis->m_bStop);
            delete pThis;
            exit(0);
        }
    }
    else
    {
        // Communication ERROR
        // Shut it down
        if (pThis->m_role == AGENT)
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>: There is a problem with the communication...1" << endl;
        else if (pThis->m_role == M16)
            cout << "<Headquarter::System @ " + timeNow() + " # Message>: There is a problem with the communication...2" << endl;

        cout << "-----Shutdown Communication-----" << endl;
        char *hint = "COMMUNICATION ERROR";
        bzero(strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO));
        pThis->m_des.encrypt(hint, strEncryptedBuffer_AIO);
        send(pThis->m_nSocket, strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO), 0);
        sem_post(&pThis->m_bStop);
        delete pThis;
        exit(0);
    }
    delete pThis;
    return;
}
// StdoutSingle
StdoutSingle::StdoutSingle(int role, Agent agent, DESUtils des, sem_t &bStop) : m_bStop(bStop), m_role(role), m_des(des)
{
    /**
     * Constructor for the StdoutSingle class
     * @param role: AGENT for agent, M16 for headquarter
     * @param agent: Agent class object storing the essential information
     * @param des: DESUtils class object used for encryption and decryption
     * @param bStop: semaphore to control the communication
     */
    this->m_nSocket = agent.getAgentSocket();
    this->m_agentCodeName = agent.getAgentCodeName();
    this->m_pReq = new aiocb;
    bzero((char *)this->m_pReq, sizeof(struct aiocb));
    /* Assign the aio control block */
    this->m_pReq->aio_fildes = STDOUT_FILENO;                                                    // Set the file descriptor to STDOUT_FILENO
    this->m_pReq->aio_buf = strDecryptedBuffer_AIO;                                              // Set the buffer to store the decrypted message
    this->m_pReq->aio_nbytes = BUFFERSIZE;                                                       // Set the number of bytes to read from stdin
    this->m_pReq->aio_offset = 0;                                                                // Set the offset to 0
    this->m_pReq->aio_sigevent.sigev_notify = SIGEV_THREAD;                                      // Set the notification method to SIGEV_THREAD
    this->m_pReq->aio_sigevent._sigev_un._sigev_thread._function = StdoutWriteCompletionHandler; // Set the completion handler function to StdoutWriteCompletionHandler
    this->m_pReq->aio_sigevent._sigev_un._sigev_thread._attribute = NULL;                        // Set the thread attribute to NULL
    this->m_pReq->aio_sigevent.sigev_value.sival_ptr = this;                                     // Set the thread attribute to NULL
}
StdoutSingle::~StdoutSingle()
{
    /**
     * Destructor for the StdoutSingle class
     */
    delete this->m_pReq;
    bzero(&strDecryptedBuffer_AIO, sizeof(strDecryptedBuffer_AIO));
}
void StdoutSingle::StdoutWriteCompletionHandler(sigval_t sigval)
{
    /**
     * Completion handler for the StdoutSingle class
     * @param sigval: signal value, containing the result of the asynchronous I/O
     * @return void
     */
    StdoutSingle *pThis = (StdoutSingle *)sigval.sival_ptr;
    if (aio_error(pThis->m_pReq) == 0)
    {
        // There is no error in the asynchronous I/O
        int nLength = aio_return(pThis->m_pReq);
        // Initialize a SockinSingle Object to move back in
        SockinSingle *pSockin = new SockinSingle(
            pThis->m_role,
            Agent("Doesn't matter", pThis->m_agentCodeName, pThis->m_nSocket),
            pThis->m_des,
            pThis->m_bStop);
        // Start the operation on Sockin
        // Motivate the SockinSingle to continue working
        aio_read(pSockin->m_pReq);
    }
    else
    {
        // Communication ERROR
        // Shut it down
        if (pThis->m_role == AGENT)
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>: There is a problem with the communication...3" << endl;
        else if (pThis->m_role == M16)
            cout << "<Headquarter::System @ " + timeNow() + " # Message>: There is a problem with the communication...4" << endl;

        cout << "-----Shutdown Communication-----" << endl;
        char *hint = "COMMUNICATION ERROR";
        bzero(strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO));
        pThis->m_des.encrypt(hint, strEncryptedBuffer_AIO);
        send(pThis->m_nSocket, strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO), 0);
        sem_post(&pThis->m_bStop);
        delete pThis;
        exit(0);
    }
    delete pThis;
    return;
}
// SockinSingle
SockinSingle::SockinSingle(int role, Agent agent, DESUtils des, sem_t &bStop) : m_bStop(bStop), m_role(role), m_des(des)
{
    /**
     * Constructor for the SockinSingle class
     * @param role: AGENT for agent, M16 for headquarter
     * @param agent: Agent class object storing the essential information
     * @param des: DESUtils class object used for encryption and decryption
     * @param bStop: semaphore to control the communication
     */
    this->m_nSocket = agent.getAgentSocket();
    this->m_agentCodeName = agent.getAgentCodeName();
    this->m_pReq = new aiocb;
    bzero((char *)this->m_pReq, sizeof(struct aiocb));
    // Assign the aio control block
    this->m_pReq->aio_fildes = this->m_nSocket;                                                 // Set the file descriptor to STDOUT_FILENO
    this->m_pReq->aio_buf = strEncryptedBuffer_AIO;                                             // Set the buffer to store the encrypted message
    this->m_pReq->aio_nbytes = BUFFERSIZE;                                                      // Set the number of bytes to read from stdin
    this->m_pReq->aio_offset = 0;                                                               // Set the offset to 0
    this->m_pReq->aio_sigevent.sigev_notify = SIGEV_THREAD;                                     // Set the notification method to SIGEV_THREAD
    this->m_pReq->aio_sigevent._sigev_un._sigev_thread._function = SockinReadCompletionHandler; // Set the completion handler function to SockinReadCompletionHandler
    this->m_pReq->aio_sigevent._sigev_un._sigev_thread._attribute = NULL;                       // Set the thread attribute to NULL
    this->m_pReq->aio_sigevent.sigev_value.sival_ptr = this;                                    // Set the thread attribute to NULL
}
SockinSingle::~SockinSingle()
{
    /**
     * Destructor for the SockinSingle class
     */
    delete this->m_pReq;
    bzero(&strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO));
}
void SockinSingle::SockinReadCompletionHandler(sigval_t sigval)
{
    /**
     * Completion handler for the SockinSingle class
     * @param sigval: signal value, containing the result of the asynchronous I/O
     * @return void
     */
    SockinSingle *pThis = (SockinSingle *)sigval.sival_ptr;
    if (aio_error(pThis->m_pReq) == 0)
    {
        // There is no error in the asynchronous I/O
        int nLength = aio_return(pThis->m_pReq);
        pThis->m_des.decrypt(strEncryptedBuffer_AIO, strDecryptedBuffer_AIO);
        StdoutSingle *pStdout = new StdoutSingle(
            pThis->m_role,
            Agent("Doesn't matter", pThis->m_agentCodeName, pThis->m_nSocket),
            pThis->m_des,
            pThis->m_bStop);
        aio_write(pStdout->m_pReq);
        // Check if the message is "OVER"
        if (memcmp("OVER", strDecryptedBuffer_AIO, 4) == 0)
        {
            // If the message received is "OVER"
            // It means the other end is done communicating
            if (pThis->m_role == AGENT)
            {
                cout << "<SecretHideout::System @ " + timeNow() + " # Message>:The Headquarter is off the line." << endl;
                cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Communication is OVER." << endl;
            }
            else if (pThis->m_role == M16)
            {
                cout << "<Headquarter::System @ " + timeNow() + " # Message>:Agent OUT." << endl;
                cout << "<Headquarter::System @ " + timeNow() + " # Message>:Communication is OVER." << endl;
            }

            cout << "-----Communication OVER-----" << endl;
            sem_post(&pThis->m_bStop);
            delete pThis;
            exit(0);
        }
        // Check if the message is "COMMUNICATION ERROR"
        else if (memcmp("COMMUNICATION ERROR", strDecryptedBuffer_AIO, 19) == 0)
        {
            // If the message the pther received is "COMMUNICATION ERROR"
            // It means there is a problem with the communication
            if (pThis->m_role == AGENT)
                cout << "<SecretHideout::System @ " + timeNow() + " # Message>:There is a problem with the communication...7" << endl;
            else if (pThis->m_role == M16)
                cout << "<Headquarter::System @ " + timeNow() + " # Message>:There is a problem with the communication...8" << endl;
            cout << "-----Shutdown Communication-----" << endl;
            sem_post(&pThis->m_bStop);
            delete pThis;
            exit(0);
        }
    }
    else
    {
        // Communication ERROR
        // Shut it down
        if (pThis->m_role == AGENT)
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>: There is a problem with the communication...9" << endl;
        else if (pThis->m_role == M16)
            cout << "<Headquarter::System @ " + timeNow() + " # Message>: There is a problem with the communication...10" << endl;

        cout << "-----Shutdown Communication-----" << endl;
        char *hint = "COMMUNICATION ERROR";
        bzero(strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO));
        pThis->m_des.encrypt(hint, strEncryptedBuffer_AIO);
        send(pThis->m_nSocket, strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO), 0);
        sem_post(&pThis->m_bStop);
        delete pThis;
        exit(0);
    }
    delete pThis;
    return;
}
// SockoutSingle
SockoutSingle::SockoutSingle(int role, Agent agent, DESUtils des, sem_t &bStop) : m_bStop(bStop), m_role(role), m_des(des)
{
    /**
     * Constructor for the SockoutSingle class
     * @param role: AGENT for agent, M16 for headquarter
     * @param agent: Agent class object storing the essential information
     * @param des: DESUtils class object used
     * for encryption and decryption
     * @param bStop: semaphore to control the communication
     */
    this->m_nSocket = agent.getAgentSocket();
    this->m_agentCodeName = agent.getAgentCodeName();
    this->m_pReq = new aiocb;
    bzero((char *)this->m_pReq, sizeof(struct aiocb));
    // Assign the aio control block
    this->m_pReq->aio_fildes = this->m_nSocket;                                                   // Set the file descriptor to STDOUT_FILENO
    this->m_pReq->aio_buf = strEncryptedBuffer_AIO;                                               // Set the buffer to store the encrypted message
    this->m_pReq->aio_nbytes = BUFFERSIZE;                                                        // Set the number of bytes to read from stdin
    this->m_pReq->aio_offset = 0;                                                                 // Set the offset to 0
    this->m_pReq->aio_sigevent.sigev_notify = SIGEV_THREAD;                                       // Set the notification method to SIGEV_THREAD
    this->m_pReq->aio_sigevent._sigev_un._sigev_thread._function = SockoutWriteCompletionHandler; // Set the completion handler function to SockoutWriteCompletionHandler
    this->m_pReq->aio_sigevent._sigev_un._sigev_thread._attribute = NULL;                         // Set the thread attribute to NULL
    this->m_pReq->aio_sigevent.sigev_value.sival_ptr = this;                                      // Set the thread attribute to NULL
}
SockoutSingle::~SockoutSingle()
{
    /**
     * Destructor for the SockoutSingle class
     */
    delete this->m_pReq;
    bzero(&strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO));
}
void SockoutSingle::SockoutWriteCompletionHandler(sigval_t sigval)
{
    /**
     * Completion handler for the SockoutSingle class
     * @param sigval: signal value, containing the result of the asynchronous I/O
     * @return void
     */
    SockoutSingle *pThis = (SockoutSingle *)sigval.sival_ptr;
    if (aio_error(pThis->m_pReq) == 0)
    {
        // There is no error in the asynchronous I/O
        int nLength = aio_return(pThis->m_pReq);
        if (nLength != BUFFERSIZE)
        {
            // Communication ERROR
            // Shut it down
            if (pThis->m_role == AGENT)
                cout << "<SecretHideout::System @ " + timeNow() + " # Message>: There is a problem with the communication...11" << endl;
            else if (pThis->m_role == M16)
                cout << "<Headquarter::System @ " + timeNow() + " # Message>: There is a problem with the communication...12" << endl;

            cout << "-----Shutdown Communication-----" << endl;
            char *hint = "COMMUNICATION ERROR";
            bzero(strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO));
            pThis->m_des.encrypt(hint, strEncryptedBuffer_AIO);
            send(pThis->m_nSocket, strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO), 0);
            sem_post(&pThis->m_bStop);
            delete pThis;
            exit(0);
        }
        else
        {
            // Initialize a StdinSingle Object to move back in
            StdinSingle *pStdin = new StdinSingle(
                pThis->m_role,
                Agent("Doesn't matter", pThis->m_agentCodeName, pThis->m_nSocket),
                pThis->m_des,
                pThis->m_bStop);
            // Start the operation on Stdin
            // Motivate the StdinSingle to continue working
            aio_read(pStdin->m_pReq);
        }
    }
    else
    {
        // Communication ERROR
        // Shut it down
        if (pThis->m_role == AGENT)
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>: There is a problem with the communication...13" << endl;
        else if (pThis->m_role == M16)
            cout << "<Headquarter::System @ " + timeNow() + " # Message>: There is a problem with the communication...14" << endl;

        cout << "-----Shutdown Communication-----" << endl;
        char *hint = "COMMUNICATION ERROR";
        bzero(strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO));
        pThis->m_des.encrypt(hint, strEncryptedBuffer_AIO);
        send(pThis->m_nSocket, strEncryptedBuffer_AIO, sizeof(strEncryptedBuffer_AIO), 0);
        sem_post(&pThis->m_bStop);
        delete pThis;
        exit(0);
    }
    delete pThis;
    return;
}
// Secret Operations
void PhantomHook(int role, Agent agent)
{
    /**
     * Secret Code Name:Phantom Hook
     * @brief This is a bait to lure the enemy in and then we can take them down
     *        We will use this as a trap to catch the enemy by communicating without any encryption
     *        role: AGENT for agent, M16 for headquarter
     * @param role: AGENT for agent, M16 for headquarter
     * @param socket: socket
     * @return void
     * @version 1.0
     */
    int socket = agent.getAgentSocket();
    string codeName = agent.getAgentCodeName();
    if (role == AGENT)
    {
        // This part is in the agent's side
        string str_time = timeNow();
        bool control_flag = false;
        cout << "<SecretHideout::System @ " + str_time + " # Message>:We’re reading you loud and clear." << endl;
        cout << "<SecretHideout::System @ " + str_time + " # Message>:Phantom Hook is activated.." << endl;
        cout << "<SecretHideout::System @ " + str_time + " # Message>:You know what to do......." << endl;
        pid_t nPid;
        nPid = fork();
        if (nPid != 0)
        {
            /**
             * Parent process
             * In charge of the following:
             * 1. Receive message from the headquarter
             * 2. Output the message
             */
            char strSocketBuffer[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strSocketBuffer, 0, sizeof(strSocketBuffer));
                int nLength = 0;
                // Using the TotalRecv function to ensure all data is received
                nLength = TotalRecv(socket, strSocketBuffer, 255, 0);
                if (nLength == 255)
                {
                    // Normal Case
                    if (strSocketBuffer[0] != 0 && strSocketBuffer[0] != '\n')
                    {
                        // Fix the problem of the sudden message at the same line of "Please Enter:"
                        cout << endl;
                        string str_time = timeNow();
                        cout << "<SecretHideout::Headquarter @ " + str_time + " # Message>: " + strSocketBuffer << endl;
                        if (memcmp("OVER", strSocketBuffer, 4) == 0)
                        {
                            // If the message the agent received is "OVER"
                            // It means the Headquarter is done communicating
                            string str_time = timeNow();
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:The Headquarter is off the line." << endl;
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                            cout << "-----Communication OVER-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                        else if (memcmp("COMMUNICATION ERROR", strSocketBuffer, 19) == 0)
                        {
                            // If the message the agent received is "COMMUNICATION ERROR"
                            // It means there is a problem with the communication
                            string str_time = timeNow();
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:There is a problem with the communication...15" << endl;
                            cout << "-----Shutdown Communication-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                    }
                }
                else
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<SecretHideout::System @ " + str_time + " # Message>: There is a problem with the communication...16" << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    send(socket, hint, 100, 0);
                    kill(nPid, SIGTERM);
                    return;
                }
            }
        }
        else
        {
            /**
             * Child process
             * In charge of the following:
             * 1. Get the message from the agent
             * 2. Send the message to the headquarter
             */
            char strStdinBuffer[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strStdinBuffer, 0, sizeof(strStdinBuffer));
                // Get the message from the agent
                cin.getline(strStdinBuffer, 255);
                int nLen = 255;
                // Send the message to the agent
                if (send(socket, strStdinBuffer, sizeof(strStdinBuffer), 0) != 255)
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<SecretHideout::System @ " + str_time + " # Message>: There is a problem with the communication...17" << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    send(socket, hint, 100, 0);
                    kill(getppid(), SIGTERM);
                    return;
                }
                else
                {
                    if (memcmp("OVER", strStdinBuffer, 4) == 0)
                    {
                        // If the message the agent sent is "OVER"
                        // It means the agent is done communicating
                        string str_time = timeNow();
                        cout << "<SecretHideout::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                        cout << "-----Communication OVER-----" << endl;
                        // Tell the headquarter the communication is over
                        send(socket, "OVER", 100, 0);
                        // Put an end to the the parent process
                        kill(getppid(), SIGTERM);
                        return;
                    }
                }
                if (control_flag)
                    cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Message Sent." << endl;
                control_flag = true;
            }
        }
    }
    else if (role == M16)
    {
        // This part is in the headquarter's side
        string str_time = timeNow();
        bool control_flag = false;
        cout << "<Headquarter::System @ " + str_time + " # Message>:Phantom Hook is activated." << endl;
        // Notify the agent to also activate the Phantom Hook
        send(socket, "Phantom Hook", 100, 0);
        pid_t nPid;
        nPid = fork();
        if (nPid != 0)
        {
            /**
             * Parent process
             * In charge of the following:
             * 1. Receive message from the agent
             * 2. Output the message
             */
            char strSocketBuffer[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strSocketBuffer, 0, sizeof(strSocketBuffer));
                int nLength = 0;
                // Using the TotalRecv function to ensure all data is received
                nLength = TotalRecv(socket, strSocketBuffer, 255, 0);
                if (nLength == 255)
                {
                    // Normal Case
                    if (strSocketBuffer[0] != 0 && strSocketBuffer[0] != '\n')
                    {
                        // Fix the problem of the sudden message at the same line of "Please Enter:"
                        cout << endl;
                        string str_time = timeNow();
                        cout << "<Headquarter::" + codeName + " @ " + str_time + " # Message>: " + strSocketBuffer << endl;
                        if (memcmp("OVER", strSocketBuffer, 4) == 0)
                        {
                            // If the message the headquarter received is "OVER"
                            // It means the agent is done communicating
                            string str_time = timeNow();
                            cout << "<Headquarter::System @ " + str_time + " # Message>:Agent OUT." << endl;
                            cout << "<Headquarter::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                            cout << "-----Communication OVER-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                        else if (memcmp("COMMUNICATION ERROR", strSocketBuffer, 19) == 0)
                        {
                            // If the message the headquarter received is "COMMUNICATION ERROR"
                            // It means there is a problem with the communication
                            string str_time = timeNow();
                            cout << "<Headquarter::System @ " + str_time + " # Message>:There is a problem with the communication...18" << endl;
                            cout << "-----Shutdown Communication-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                    }
                }
                else
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<Headquarter::System @ " + str_time + " # Message>: There is a problem with the communication...19" << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    send(socket, hint, 100, 0);
                    kill(nPid, SIGTERM);
                    return;
                }
            }
        }
        else
        {
            /**
             * Child process
             * In charge of the following:
             * 1. Get the message from the headquarter
             * 2. Send the message to the agent
             */
            char strStdinBuffer[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strStdinBuffer, 0, sizeof(strStdinBuffer));
                // Get the message from the headquarter
                // if (fgets(strStdinBuffer, 255, stdin) == NULL)
                // {
                //     continue;
                // }
                cin.getline(strStdinBuffer, 255);
                int nLen = 255;
                // Send the message to the agent
                if (send(socket, strStdinBuffer, sizeof(strStdinBuffer), 0) != 255)
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<Headquarter::System @ " + str_time + " # Message>: There is a problem with the communication...20" << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    send(socket, hint, 100, 0);
                    // Put an end to the the parent process
                    kill(getppid(), SIGTERM);
                    return;
                }
                else
                {
                    if (memcmp("OVER", strStdinBuffer, 4) == 0)
                    {
                        // If the message the headquarter sent is "OVER"
                        // It means the headquarter is done communicating
                        string str_time = timeNow();
                        cout << "<Headquarter::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                        cout << "-----Communication OVER-----" << endl;
                        // Tell the agent the communication is over
                        send(socket, "OVER", 100, 0);
                        // Put an end to the the parent process
                        kill(getppid(), SIGTERM);
                        return;
                    }
                }
                if (control_flag)
                    cout << "<Headquarter::System @ " + timeNow() + " # Message>:Message Sent." << endl;
                control_flag = true;
            }
        }
    }
}
void SilentGuardian(int role, Agent agent, DESUtils des, RSAUtils rsa)
{
    /**
     * Secret Code Name:Silent Guardian
     * @brief This is a secret communicating channel for the headquarter and the agent
     *        In case there is enemy's interception, this will be activated to ensure the safety of the communication
     *        The communication is based on the DES algorithm
     * @note Implementation of the RSA algorithm is now available for the transmission of the DES key
     *       There is no need to worry about the interception of the DES key
     *       So now the DES key is randomly generated instead of written hard-coded
     *       and then transmitted using the RSA algorithm.
     * @param role: AGENT for agent, M16 for headquarter
     * @param agent: Agent class object storing the essential information
     * @param des: DESUtils class object used for encryption and decryption
     * @param rsa: RSAUtils class object used for transmitting the DES key
     * @return void
     * @version 2.0
     */
    int socket = agent.getAgentSocket();
    string codeName = agent.getAgentCodeName();
    if (role == AGENT)
    {
        // This part is in the agent's side
        string str_time = timeNow();
        bool control_flag = false;

        // First, the agent will use the public RSA key to encrypt the DES key
        // Then send the encrypted DES key to the headquarter
        cout << "-----Generating DES Key-----" << endl;
        // Gererating the DES key until the DES key is smaller than the RSA public key
        while (true)
        {
            des.generateRandomRootKey();
            if (des.getRootKey() >= rsa.getPublicKey().second)
                continue;
            break;
        }

        // Due to the hard-coded written of the Default DES key, we discard the hard-coded key for now
        des.genKey(des.getRootKey());
        cout << "<SecretHideout::System @ " + str_time + " # Message>:DES Key Generated." << endl;
        cout << "-----RSA encrypting the DES Key-----" << endl;
        // Encrypt the DES key using the RSA public key
        uint64_t encryptedKey = 0;
        // In order not to be noticed by the enemy, we use this random name to hide the DES key
        uint64_t sdjsadixcxzdssf = des.getRootKey();
        // Print the DES key
        // cout << "<SecretHideout::System @ " + str_time + " # Message>:DES Key: " + to_string(sdjsadixcxzdssf) << endl;
        rsa.encrypt(sdjsadixcxzdssf, encryptedKey);
        // Print the encrypted DES key
        // cout << "<SecretHideout::System @ " + str_time + " # Message>:Encrypted DES Key: " + to_string(encryptedKey) << endl;
        if (send(socket, &encryptedKey, sizeof(encryptedKey), 0) != sizeof(encryptedKey))
        {
            cout << "<SecretHideout::System @ " + str_time + " # Message>:There is a problem with the communication...21" << endl;
            cout << "-----Shutdown Communication-----" << endl;
            return;
        }
        else
            cout << "<SecretHideout::System @ " + str_time + " # Message>:Successfully sent the DES Key!" << endl;

        cout << "-----Everything Ready-----" << endl;
        cout << "-----Communication Start-----" << endl;
        pid_t nPid;
        nPid = fork();
        if (nPid != 0)
        {

            /**
             * Parent process
             * In charge of the following:
             * 1. Receive message from the headquater
             * 2. Decrypt the message
             * 3. Output the message
             */
            char strSocketBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strSocketBuffer, 0, sizeof(strSocketBuffer));
                memset(encryedtext, 0, sizeof(encryedtext));
                memset(decryptedtext, 0, sizeof(decryptedtext));
                int nLength = 0;
                // Using the TotalRecv function to ensure all data is received
                nLength = TotalRecv(socket, strSocketBuffer, 255, 0);
                if (nLength == 255)
                {
                    // Normal Case
                    des.decrypt(strSocketBuffer, decryptedtext);
                    decryptedtext[254] = 0;
                    if (decryptedtext[0] != 0 && decryptedtext[0] != '\n')
                    {
                        // Fix the problem of the sudden message at the same line of "Please Enter:"
                        cout << endl;
                        string str_time = timeNow();
                        // if (control_flag)
                        //     cout << "<SecretHideout::Headquarter @ " + str_time + " # Message>: " + decryptedtext << endl;
                        cout << "<SecretHideout::Headquarter @ " + str_time + " # Message>: " + decryptedtext << endl;
                        if (memcmp("OVER", decryptedtext, 4) == 0)
                        {
                            // If the message the agent received is "OVER"
                            // It means the Headquarter is done communicating
                            string str_time = timeNow();
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:The Headquarter is off the line." << endl;
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                            cout << "-----Communication OVER-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                        else if (memcmp("COMMUNICATION ERROR", decryptedtext, 19) == 0)
                        {
                            // If the message the agent received is "COMMUNICATION ERROR"
                            // It means there is a problem with the communication
                            string str_time = timeNow();
                            cout << "<SecretHideout::System @ " + str_time + " # Message>:There is a problem with the communication...22" << endl;
                            cout << "-----Shutdown Communication-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                    }
                    control_flag = true;
                }
                else
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<SecretHideout::System @ " + str_time + " # Message>: There is a problem with the communication...23" << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    memset(encryedtext, 0, sizeof(encryedtext));
                    des.encrypt(hint, encryedtext);
                    send(socket, encryedtext, sizeof(encryedtext), 0);
                    kill(nPid, SIGTERM);
                    return;
                }
            }
        }
        else
        {
            /**
             * Child process
             * In charge of the following:
             * 1. Get the message from the agent
             * 2. Encrypt the message
             * 3. Send the message to the headquarter
             */
            char strStdinBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strStdinBuffer, 0, sizeof(strStdinBuffer));
                memset(encryedtext, 0, sizeof(encryedtext));
                memset(decryptedtext, 0, sizeof(decryptedtext));

                // Get the message from the agent
                cin.getline(strStdinBuffer, 255);
                int nLen = 255;
                // Encrypt the message
                des.encrypt(strStdinBuffer, encryedtext);
                // Send the message to the agent
                if (send(socket, encryedtext, sizeof(encryedtext), 0) != 255)
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<SecretHideout::System @ " + str_time + " # Message>: There is a problem with the communication...24" << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    memset(encryedtext, 0, sizeof(encryedtext));
                    des.encrypt(hint, encryedtext);
                    send(socket, encryedtext, sizeof(encryedtext), 0);
                    // Put an end to the the parent process
                    kill(getppid(), SIGTERM);
                    return;
                }
                else
                {

                    if (memcmp("OVER", strStdinBuffer, 4) == 0)
                    {
                        // If the message the agent sent is "OVER"
                        // It means the agent is done communicating
                        string str_time = timeNow();
                        cout << "<SecretHideout::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                        cout << "-----Communication OVER-----" << endl;
                        // Tell the headquarter the communication is over
                        char *hint = "OVER";
                        memset(encryedtext, 0, sizeof(encryedtext));
                        des.encrypt(hint, encryedtext);
                        send(socket, encryedtext, sizeof(encryedtext), 0);
                        // Put an end to the the parent process
                        kill(getppid(), SIGTERM);
                        return;
                    }
                    if (control_flag)
                        cout << "<Headquarter::System @ " + timeNow() + " # Message>:Message Sent." << endl;
                    control_flag = true;
                }
            }
        }
    }
    else if (role == M16)
    {
        // This part is in the headquarter's side
        string str_time = timeNow();
        bool control_flag = false;
        cout << "<Headquarter::System @ " + str_time + " # Message>:Silent Guardian is activated.." << endl;
        // Notify the agent to also activate the Silent Guardian
        send(socket, "Silent Guardian", 100, 0);
        while (true)
        {
            // Version 2.0: RSA algorithm is used to transmit the DES key
            // Gererate the RSA utils
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>:RSA initializaing......" << endl;
            int rounds;
            bool defaulyRSAKey;
            bool ifHighSecurity;
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Please enter the number of maxium rounds for RSA:" << endl;
            cin >> rounds;
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Please enter the default RSA key or not(1 for yes, 0 for no):" << endl;
            cin >> defaulyRSAKey;
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Please enter the high security mode or not(1 for yes, 0 for no):" << endl;
            cin >> ifHighSecurity;
            if (rsa.init(rounds, defaulyRSAKey, ifHighSecurity) == FAILURE)
            {
                cout << "<Headquarter::System @ " + timeNow() + " # Message>:Oops!RSA Initialization failed!" << endl;
                cout << "<Headquarter::System @ " + timeNow() + " # Message>:It is probably due to the the related settings.Please try again." << endl;
                continue;
            }
            break;
        }

        // Send the RSA public key to the agent
        pair<uint64_t, uint64_t> publicKey = rsa.getPublicKey();
        // Send the public key to the agent in the format of "(e, n)"
        string strPublicKey = "(" + to_string(publicKey.first) + ", " + to_string(publicKey.second) + ")";
        send(socket, strPublicKey.c_str(), 100, 0);
        // Print the public key
        // cout << "<Headquarter::System @ " + timeNow() + " # Message>:RSA Public Key: " + strPublicKey << endl;
        if (send(socket, strPublicKey.c_str(), 100, 0) != 100)
        {
            cout << "<Headquarter::System @ " + timeNow() + " # Message>:There is a problem with the communication...25" << endl;
            cout << "-----Shutdown Communication-----" << endl;
            return;
        }
        else
            cout << "<Headquarter::System @ " + timeNow() + " # Message>:Successfully sent the RSA Key!" << endl;

        // Wait for the agent to send the DES key back
        char strSocketBuffer[255];
        memset(strSocketBuffer, 0, sizeof(strSocketBuffer));
        cout << "-----Waiting for the DES Key-----" << endl;
        // Using the TotalRecv function to ensure all data is received
        if (recv(socket, strSocketBuffer, 255, 0) < 0)
        {
            cout << "<Headquarter::System @ " + timeNow() + " # Message>:There is a problem with the communication...26" << endl;
            cout << "-----Shutdown Communication-----" << endl;
            return;
        }
        else
        {
            cout << "<Headquarter::System @ " + timeNow() + " # Message>:Successfully received the DES Key!" << endl;
            cout << "-----Decrypting the DES Key-----" << endl;
            // Decrypt the DES key using the RSA private key
            uint64_t encryptedKey = 0;
            memcpy(&encryptedKey, strSocketBuffer, sizeof(encryptedKey));
            // Print the encrypted DES key
            // cout << "<Headquarter::System @ " + timeNow() + " # Message>:Encrypted DES Key: " + to_string(encryptedKey) << endl;
            uint64_t decryptedKey = 0;
            rsa.decrypt(decryptedKey, encryptedKey);
            // Print the decrypted DES key
            // cout << "<Headquarter::System @ " + timeNow() + " # Message>:Decrypted DES Key: " + to_string(decryptedKey) << endl;
            cout << "-----Generating DES Key-----" << endl;
            des.genKey(decryptedKey);
            cout << "<Headquarter::System @ " + timeNow() + " # Message>:DES Key Generated." << endl;
        }
        cout << "-----Everything Ready-----" << endl;
        cout << "-----Communication Start-----" << endl;

        pid_t nPid;
        nPid = fork();
        if (nPid != 0)
        {
            /**
             * Parent process
             * In charge of the following:
             * 1. Receive message from the agent
             * 2. Decrypt the message
             * 3. Output the message
             */
            char strSocketBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];

            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strSocketBuffer, 0, sizeof(strSocketBuffer));
                memset(encryedtext, 0, sizeof(encryedtext));
                memset(decryptedtext, 0, sizeof(decryptedtext));
                int nLength = 0;
                // Using the TotalRecv function to ensure all data is received
                nLength = TotalRecv(socket, strSocketBuffer, 255, 0);
                if (nLength == 255)
                {
                    // Normal Case
                    des.decrypt(strSocketBuffer, decryptedtext);
                    decryptedtext[254] = 0;
                    if (decryptedtext[0] != 0 && decryptedtext[0] != '\n')
                    {
                        // Fix the problem of the sudden message at the same line of "Please Enter:"
                        cout << endl;
                        string str_time = timeNow();
                        cout << "<Headquarter::" + codeName + " @ " + str_time + " # Message>: " + decryptedtext << endl;
                        if (memcmp("OVER", decryptedtext, 4) == 0)
                        {
                            // If the message the headquarter received is "OVER"
                            // It means the agent is done communicating
                            string str_time = timeNow();
                            cout << "<Headquarter::System @ " + str_time + " # Message>:Agent OUT." << endl;
                            cout << "<Headquarter::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                            cout << "-----Communication OVER-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                        else if (memcmp("COMMUNICATION ERROR", decryptedtext, 19) == 0)
                        {
                            // If the message the headquarter received is "COMMUNICATION ERROR"
                            // It means there is a problem with the communication
                            string str_time = timeNow();
                            cout << "<Headquarter::System @ " + str_time + " # Message>:There is a problem with the communication...27" << endl;
                            cout << "-----Shutdown Communication-----" << endl;
                            kill(nPid, SIGTERM);
                            return;
                        }
                    }
                }
                else
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<Headquarter::System @ " + str_time + " # Message>: There is a problem with the communication...28" << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    memset(encryedtext, 0, sizeof(encryedtext));
                    des.encrypt(hint, encryedtext);
                    send(socket, encryedtext, sizeof(encryedtext), 0);
                    kill(nPid, SIGTERM);
                    return;
                }
            }
        }
        else
        {
            /**
             * Child process
             * In charge of the following:
             * 1. Get the message from the headquarter
             * 2. Encrypt the message
             * 3. Send the message to the agent
             */
            char strStdinBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            while (true)
            {
                memset(strStdinBuffer, 0, sizeof(strStdinBuffer));
                memset(encryedtext, 0, sizeof(encryedtext));
                memset(decryptedtext, 0, sizeof(decryptedtext));
                // Get the message from the headquarter
                cin.getline(strStdinBuffer, 255);
                int nLen = 255;
                // Encrypt the message
                des.encrypt(strStdinBuffer, encryedtext);
                // Send the message to the agent
                if (send(socket, encryedtext, sizeof(encryedtext), 0) != 255)
                {
                    // If the message is not 255 bytes
                    // There is a problem with the communication
                    string str_time = timeNow();
                    cout << "<Headquarter::System @ " + str_time + " # Message>: There is a problem with the communication...29" << endl;
                    cout << "-----Shutdown Communication-----" << endl;
                    char *hint = "COMMUNICATION ERROR";
                    memset(encryedtext, 0, sizeof(encryedtext));
                    des.encrypt(hint, encryedtext);
                    send(socket, encryedtext, sizeof(encryedtext), 0);
                    // Put an end to the the parent process
                    kill(getppid(), SIGTERM);
                    return;
                }
                else
                {
                    if (memcmp("OVER", strStdinBuffer, 4) == 0)
                    {
                        // If the message the headquarter sent is "OVER"
                        // It means the headquarter is done communicating
                        string str_time = timeNow();
                        cout << "<Headquarter::System @ " + str_time + " # Message>:Communication is OVER." << endl;
                        cout << "-----Communication OVER-----" << endl;
                        // Tell the agent the communication is over
                        char *hint = "OVER";
                        memset(encryedtext, 0, sizeof(encryedtext));
                        des.encrypt(hint, encryedtext);
                        send(socket, encryedtext, sizeof(encryedtext), 0);
                        // Put an end to the the parent process
                        kill(getppid(), SIGTERM);
                        return;
                    }
                }
                if (control_flag)
                    cout << "<Headquarter::System @ " + timeNow() + " # Message>:Message Sent." << endl;
                control_flag = true;
            }
        }
    }
    return;
}
// Secret Operations with the asynchronous I/O
void PhantomHook_AIO(int role, Agent agent)
{
    int socket = agent.getAgentSocket();
    string codeName = agent.getAgentCodeName();
    // Test the PhantomHook function for the asynchronous I/O
    if (role == AGENT)
    {
        // To be implemented......
    }
    else if (role == M16)
    {
        // To be implemented......
    }
    return;
}
void SilentGuardian_AIO(int role, Agent agent, DESUtils des, RSAUtils rsa)
{
    /***
     * Secret Code Name:Silent Guardian
     * @brief This is a upgraded version of the Silent Guardian
     * @note  NOW The asynchronous I/O is used for the communication to increase the TCP efficiency
     *        The AIO workflow is following:
     *        (1)Child Process:StdinSingle ---> SockoutSingle
     *        (2)Parent Process:SockinSingle ---> StdoutSingle
     * @param role: AGENT for agent, M16 for headquarter
     * @param agent: Agent class object storing the essential information
     * @param des: DESUtils class object used
     * @param rsa: RSAUtils class object used
     * @return void
     */
    int socket = agent.getAgentSocket();
    string codeName = agent.getAgentCodeName();
    if (role == AGENT)
    {
        // This part is in the agent's side
        string str_time = timeNow();
        bool control_flag = false;

        // First, the agent will use the public RSA key to encrypt the DES key
        // Then send the encrypted DES key to the headquarter
        cout << "-----Generating DES Key-----" << endl;
        // Gererating the DES key until the DES key is smaller than the RSA public key
        while (true)
        {
            des.generateRandomRootKey();
            if (des.getRootKey() >= rsa.getPublicKey().second)
                continue;
            break;
        }
        // Due to the hard-coded written of the Default DES key, we discard the hard-coded key for now
        des.genKey(des.getRootKey());
        cout << "<SecretHideout::System @ " + str_time + " # Message>:DES Key Generated." << endl;
        cout << "-----RSA encrypting the DES Key-----" << endl;
        // Encrypt the DES key using the RSA public key
        uint64_t encryptedKey = 0;
        // In order not to be noticed by the enemy, we use this random name to hide the DES key
        uint64_t sdjsadixcxzdssf = des.getRootKey();
        // Print the DES key
        // cout << "<SecretHideout::System @ " + str_time + " # Message>:DES Key: " + to_string(sdjsadixcxzdssf) << endl;
        rsa.encrypt(sdjsadixcxzdssf, encryptedKey);
        // Print the encrypted DES key
        // cout << "<SecretHideout::System @ " + str_time + " # Message>:Encrypted DES Key: " + to_string(encryptedKey) << endl;
        if (send(socket, &encryptedKey, sizeof(encryptedKey), 0) != sizeof(encryptedKey))
        {
            cout << "<SecretHideout::System @ " + str_time + " # Message>:There is a problem with the communication...30" << endl;
            cout << "-----Shutdown Communication-----" << endl;
            return;
        }
        else
            cout << "<SecretHideout::System @ " + str_time + " # Message>:Successfully sent the DES Key!" << endl;

        cout << "-----Everything Ready-----" << endl;
        cout << "-----Communication Start-----" << endl;
        pid_t nPid;
        nPid = fork();
        sem_t bStop;
        sem_init(&bStop, 0, 0);
    REALSTART1:
        if (nPid != 0)
        {

            /**
             * Parent process
             * In charge of the following:
             * 1. Receive message from the headquater
             * 2. Decrypt the message
             * 3. Output the message
             */
            char strSocketBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            if (control_flag)
            {
                SockinSingle *pSockin = new SockinSingle(AGENT, agent, des, bStop);
                aio_read(pSockin->m_pReq);
                // Wait until the semaphore value > 0
                sem_wait(&bStop);
                // Release the semaphore
                sem_destroy(&bStop);
                // Kill the child process
                kill(nPid, SIGTERM);
                return;
            }
            control_flag = true;
            goto REALSTART1;
        }
        else
        {
            /**
             * Child process
             * In charge of the following:
             * 1. Get the message from the agent
             * 2. Encrypt the message
             * 3. Send the message to the headquarter
             */
            char strStdinBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            if (control_flag)
            {
                StdinSingle *pStdin = new StdinSingle(AGENT, agent, des, bStop);
                aio_read(pStdin->m_pReq);
                // Wait until the semaphore value > 0
                sem_wait(&bStop);
                // Release the semaphore
                sem_destroy(&bStop);
            }
            control_flag = true;
            goto REALSTART1;
        }
    }
    else if (role == M16)
    {
        // This part is in the headquarter's side
        string str_time = timeNow();
        bool control_flag = false;
        cout << "<Headquarter::System @ " + str_time + " # Message>:Silent Guardian is activated.." << endl;
        // Notify the agent to also activate the Silent Guardian
        send(socket, "Silent Guardian", 100, 0);
        while (true)
        {
            // Version 2.0: RSA algorithm is used to transmit the DES key
            // Gererate the RSA utils
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>:RSA initializaing......" << endl;
            int rounds;
            bool defaulyRSAKey;
            bool ifHighSecurity;
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Please enter the number of maxium rounds for RSA:" << endl;
            cin >> rounds;
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Please enter the default RSA key or not(1 for yes, 0 for no):" << endl;
            cin >> defaulyRSAKey;
            cout << "<SecretHideout::System @ " + timeNow() + " # Message>:Please enter the high security mode or not(1 for yes, 0 for no):" << endl;
            cin >> ifHighSecurity;
            if (rsa.init(rounds, defaulyRSAKey, ifHighSecurity) == FAILURE)
            {
                cout << "<Headquarter::System @ " + timeNow() + " # Message>:Oops!RSA Initialization failed!" << endl;
                cout << "<Headquarter::System @ " + timeNow() + " # Message>:It is probably due to the the related settings.Please try again." << endl;
                continue;
            }
            break;
        }

        // Send the RSA public key to the agent
        pair<uint64_t, uint64_t> publicKey = rsa.getPublicKey();
        // Send the public key to the agent in the format of "(e, n)"
        string strPublicKey = "(" + to_string(publicKey.first) + ", " + to_string(publicKey.second) + ")";
        send(socket, strPublicKey.c_str(), 100, 0);
        // Print the public key
        // cout << "<Headquarter::System @ " + timeNow() + " # Message>:RSA Public Key: " + strPublicKey << endl;
        if (send(socket, strPublicKey.c_str(), 100, 0) != 100)
        {
            cout << "<Headquarter::System @ " + timeNow() + " # Message>:There is a problem with the communication...31" << endl;
            cout << "-----Shutdown Communication-----" << endl;
            return;
        }
        else
            cout << "<Headquarter::System @ " + timeNow() + " # Message>:Successfully sent the RSA Key!" << endl;

        // Wait for the agent to send the DES key back
        char strSocketBuffer[255];
        memset(strSocketBuffer, 0, sizeof(strSocketBuffer));
        cout << "-----Waiting for the DES Key-----" << endl;
        // Using the TotalRecv function to ensure all data is received
        if (recv(socket, strSocketBuffer, 255, 0) < 0)
        {
            cout << "<Headquarter::System @ " + timeNow() + " # Message>:There is a problem with the communication...32" << endl;
            cout << "-----Shutdown Communication-----" << endl;
            return;
        }
        else
        {
            cout << "<Headquarter::System @ " + timeNow() + " # Message>:Successfully received the DES Key!" << endl;
            cout << "-----Decrypting the DES Key-----" << endl;
            // Decrypt the DES key using the RSA private key
            uint64_t encryptedKey = 0;
            memcpy(&encryptedKey, strSocketBuffer, sizeof(encryptedKey));
            // Print the encrypted DES key
            // cout << "<Headquarter::System @ " + timeNow() + " # Message>:Encrypted DES Key: " + to_string(encryptedKey) << endl;
            uint64_t decryptedKey = 0;
            rsa.decrypt(decryptedKey, encryptedKey);
            // Print the decrypted DES key
            // cout << "<Headquarter::System @ " + timeNow() + " # Message>:Decrypted DES Key: " + to_string(decryptedKey) << endl;
            cout << "-----Generating DES Key-----" << endl;
            des.genKey(decryptedKey);
            cout << "<Headquarter::System @ " + timeNow() + " # Message>:DES Key Generated." << endl;
        }
        cout << "-----Everything Ready-----" << endl;
        cout << "-----Communication Start-----" << endl;

        pid_t nPid;
        nPid = fork();
        sem_t bStop;
        sem_init(&bStop, 0, 0);
    REALSTART2:
        if (nPid != 0)
        {
            /**
             * Parent process
             * In charge of the following:
             * 1. Receive message from the agent
             * 2. Decrypt the message
             * 3. Output the message
             */
            char strSocketBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];

            // Register the signal handler
            signal(SIGTERM, signalHandler);
            if (control_flag)
            {
                SockinSingle *pSockin = new SockinSingle(M16, agent, des, bStop);
                aio_read(pSockin->m_pReq);
                // Wait until the semaphore value > 0
                sem_wait(&bStop);
                // Release the semaphore
                sem_destroy(&bStop);
                // Both the parent and the child process will return and exit
                // Then the control flow will return to the main function to wait for another agent to coming in ...
                kill(nPid, SIGTERM);
                return;
            }
            control_flag = true;
            goto REALSTART2;
        }
        else
        {
            /**
             * Child process
             * In charge of the following:
             * 1. Get the message from the headquarter
             * 2. Encrypt the message
             * 3. Send the message to the agent
             */
            char strStdinBuffer[255];
            char encryedtext[255];
            char decryptedtext[255];
            // Register the signal handler
            signal(SIGTERM, signalHandler);
            if (control_flag)
            {
                StdinSingle *pStdin = new StdinSingle(M16, agent, des, bStop);
                aio_read(pStdin->m_pReq);
                // Wait until the semaphore value > 0
                sem_wait(&bStop);
                // Release the semaphore
                sem_destroy(&bStop);
            }
            control_flag = true;
            goto REALSTART2;
        }
    }
}
