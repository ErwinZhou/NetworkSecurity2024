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

ssize_t TotalRecv(int s, void *buf, size_t len, int flags)
{
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
