#include "utils.h"

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

void silentGuardain()
{
    /*
     * Secret Code Name:Silent Guardian
     * This is a secret communicating channel for the headquarter and the agent
     * In case there is enemy's interception, this will be activated to ensure the safety of the communication
     * The communication is based on the DES algorithm
     */
}