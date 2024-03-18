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

std::string hexToBinary(const std::string &hex)
{
    std::bitset<16> set(std::stoull(hex, nullptr, 16));
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
std::string binaryToHex(const std::string &binary)
{
    std::bitset<64> set(binary);
    std::stringstream ss;
    ss << std::hex << set.to_ullong();
    return ss.str();
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