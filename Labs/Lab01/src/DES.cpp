#include "DES.h"

INT32 permutataionKey(uint64_t *key, int mode)
{
    /**
     * @brief Permutate the key
     * param key: the key to be permutated
     * param mode: 1 for using PC1, 2 for using PC2
     */
}

INT32 genKey(uint32_t leftKey, uint32_t rightKey)
{
    /**
     * @brief Generate 16 subkeys for DES
     */
    uint64_t tempKey = (leftKey << 32) | rightKey;
    permutataionKey(&tempKey, 1);
}
