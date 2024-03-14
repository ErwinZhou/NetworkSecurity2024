#include "DES.h"
#include "utils.h"

INT32 DESUtils::shiftKey(uint32_t *key, int round)
{
    /**
     * @brief Permutate the key using the shiftBits array
     * param key: the key to be permutated
     * param mode: 1 for using PC1, 2 for using PC2
     */
}
INT32 DESUtils::permutationKey(uint64_t *key, int mode)
{
    /**
     * @brief Permutate the key
     * param key: the key to be permutated
     * param mode: 1 for using PC1, 2 for using PC2
     */
    uint64_t outputKey;
    switch (mode)
    {
    case 1:
        // Permutate the key using PC1(64bits to 56bits)
        for (int i = 0; i < 56; i++)
            outputKey |= ((*key >> (64 - PC1[i])) & 0x1) << (55 - i);
        break;
    case 2:
        for (int i = 0; i < 56; i++)
            outputKey |= ((*key >> (64 - PC2[i])) & 0x1) << (55 - i);
        break;
    }
    *key = outputKey;
    return 0;
}

INT32 DESUtils::genKey(uint64_t *key)
{
    /**
     * @brief Generate 16 subkeys for DES
     */
    uint64_t tempKey = *key;
    // First step: Permutate the key using PC1 from 64bits to 56bits
    permutationKey(&tempKey, 1);
    // Second step: Generate 16 subkeys
    uint32_t leftKey = tempKey >> 28;
    uint32_t rightKey = tempKey & 0xfffffff;

    for (int i = 1; i <= 16; i++)
    {
        uint32_t currLeftKey = leftKey;
        uint32_t currRightKey = rightKey;
        shiftKey(&leftKey, i);
        shiftKey(&rightKey, i);
        uint64_t subKey = (uint64_t(leftKey) << 28) | rightKey;
        permutationKey(&subKey, 2);
        roundKeys[i - 1] = subKey;
    }
    return 0;
}

int main()
{
    bitset<64> b("0001001100110100010101110111100110011011101111001101111111110001");
    uint64_t num = b.to_ullong();
    printBits64(num);
    DESUtils des;
    char *a = "111100001100110010101010111101010101100110011110001111";

    des.genKey(&num);

    return 0;
}