#include "DES.h"
#include "utils.cpp"

INT32 DESUtils::shiftKey(uint32_t *key, int round)
{
    /**
     * @brief Shift the leftKey or the rightKey based on different round
     * param key: the key to be shifted
     * param round: one of 16
     */
    *key = (*key << shiftBits[round - 1]) | (*key >> (28 - shiftBits[round - 1]));
    *key &= 0x0FFFFFFF;
    return SUCCESS;
}
INT32 DESUtils::permutationKey(uint64_t *key, int mode)
{
    /**
     * @brief Permutate the key
     * param key: the key to be permutated
     * param mode: 1 for using PC1, 2 for using PC2
     */
    uint64_t outputKey = 0;
    switch (mode)
    {
    case 1:
        // Permutate the key using PC1(64bits to 56bits)
        for (int i = 0; i < 56; i++)
            outputKey |= ((*key >> (64 - PC1[i])) & 0x1) << (55 - i);
        break;
    case 2:
        // Choose 48bits from 56bits using PC2
        for (int i = 0; i < 48; i++)
            outputKey |= ((*key >> (56 - PC2[i])) & 0x1) << (47 - i);
        break;
    }
    *key = outputKey;
    return SUCCESS;
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
        shiftKey(&leftKey, i);
        shiftKey(&rightKey, i);
        uint64_t subKey = (uint64_t(leftKey) << 28) | rightKey;
        permutationKey(&subKey, 2);
        roundKeys[i - 1] = subKey;
    }
    return SUCCESS;
}

INT32 DESUtils::initialPermutation(uint64_t *data)
{
    /**
     * @brief The first step of DES encryption using IP, before 16 rounds
     * param data: the data to be permutated
     * return: the result of permutation
     */
    uint64_t *output = 0;
    for (int i = 0; i < 64; i++)
        *output |= (*data >> (64 - ip[i]) & 0x1) << (63 - i);
    *data = *output;
    return SUCCESS;
}
INT32 DESUtils::expansionBox(uint32_t *data, uint64_t *output)
{
    /**
     * @brief Expansion Box used for roundHandler to expand the data from 32bits to 48bits
     * param data: right data with 32bits waiting to be expanded in 48bits
     * return: the result of the expansion
     */
    for (int i = 0; i < 32; i++)
        *output |= ((*data >> (32 - EBox[i])) & 0x1) << (31 - i);
    return SUCCESS;
}
INT32 DESUtils::extractionBox(uint64_t *data, uint32_t *output)
{
    /**
     * @brief Extraction Box used for roundHandler to extract data from 48bits back to 32bits
     * param data: the xored data with 48bits waiting to be extracted to 32bits
     * return: the result of the extraction
     */
    uint8_t *subData;
    for (int i = 0; i < 8; i++)
    {
        *subData = (*data >> (6 * i)) & 0x3F; // Extract 6 bits at a time
        *output |= SBox[i][*subData] << (6 * i);
    }
    return SUCCESS;
}
INT32 DESUtils::permutation(uint32_t *data)
{
    /**
     * @brief 4st and the last step of the roundHandler
     * param data: the data to be permutated
     * return: the result of the permutation
     */
    uint32_t *output = 0;
    for (int i = 0; i < 32; i++)
        *output |= (*data >> (32 - PBox[i]) & 0x1) << (31 - i);
    *data = *output;
    return SUCCESS;
}
INT32 DESUtils::roundHandler(uint32_t *leftData, uint32_t *rightData, uint64_t *roundKey)
{
    /**
     * @brief Handler for 16 rounds of encryption for DES
     * param leftData:Li-1 for each round 32 bits
     * param rightData:Ri-1 for each round 32 bits
     * param roundKey: one of 16 roundKey for the current round
     * return: the Li and Ri that will be facilitated for next round
     */
    uint32_t *tempRightData = rightData;
    // f function for this round
    uint64_t *expandedData = 0;
    expansionBox(tempRightData, expandedData);
    *expandedData &= *roundKey;
    uint32_t *extractedData = 0;
    extractionBox(expandedData, extractedData);
    permutation(extractedData);
    *rightData = *leftData ^ *extractedData;
    *leftData = *tempRightData;
    return SUCCESS;
}
INT32 DESUtils::initialPermutationReverse(uint64_t *data)
{
    /**
     * @brief The last step of DES encryption using IP^-1, after 16 rounds
     * param data: the data to be permutated
     * return: the result of permutation
     */
    uint64_t *output = 0;
    for (int i = 0; i < 64; i++)
        *output |= (*data >> (64 - ip_reverse[i]) & 0x1) << (63 - i);
    *data = *output;
    return SUCCESS;
}
void DESUtils::encrypt(const char *plaintext, const char *key, char *ciphertext)
{
    /**
     * @brief Encrypt the plaintext using DES
     * param plaintext: the plaintext to be encrypted
     * param key: the key for encryption
     * param ciphertext: the result of encryption
     */
    // Divide the plaintext into 64bits in a group

    int length = strlen(plaintext);
    for (int j = 0; j < length; j += 8)
    {
        uint64_t *data = 0;
        int blockLength = (j + 8 < length) ? 8 : length - j; // Determine the length of the current block
        for (int i = 0; i < blockLength * 8; i++)
            *data |= ((uint64_t)(plaintext[j + i / 8] >> (7 - (i % 8))) & 0x1) << (63 - i);
        uint64_t tempData = 0;
        // IP
        initialPermutation(data);
        for (int i = 0; i < 16; i++)
        {
            // 16 rounds
            uint32_t leftData = (*data) >> 32;
            uint32_t rightData = (*data) & 0xffffffff;
            roundHandler(&leftData, &rightData, &roundKeys[i]);
            tempData = ((uint64_t)leftData << 32) | rightData;
        }
        *data = tempData;
        // IP^-1
        initialPermutationReverse(data);
        // Convert the 64-bit data back to char array
        for (int i = 0; i < blockLength; i++)
            ciphertext[j + i] = (char)((*data >> (56 - 8 * i)) & 0xFF);
    }
    return;
}
INT32 DESUtils::roundHandlerReverse(uint32_t *leftData, uint32_t *rightData, uint64_t *roundKey)
{
    /**
     * @brief Handler for 16 rounds of decryption for DES
     *        There are 2 differences in it:
     *        (1)The round Key are used in reverse order
     *        (2)The left and right part of the data will be exchanged
     * param leftData:Li-1 for each round 32 bits
     * param rightData:Ri-1 for each round 32 bits
     * param roundKey: one of 16 roundKey for the current round
     * return: the Li and Ri that will be facilitated for next round
     */
    uint64_t expandedData = 0;
    expansionBox(rightData, &expandedData);
    expandedData ^= *roundKey;

    uint32_t extractedData = 0;
    extractionBox(&expandedData, &extractedData);
    permutation(&extractedData);

    uint32_t temp = *leftData;
    *leftData = *rightData;
    *rightData = temp ^ extractedData;

    return SUCCESS;
}
void DESUtils::decrypt(const char *ciphertext, const char *key, char *plaintext)
{
    /**
     * @brief Decrypt the ciphertext using DES
     * param ciphertext: the ciphertext to be decrypted
     * param key: the key for decryption
     * param plaintext: the result of decryption
     */
    // Divide the ciphertext into 64bits in a group

    int length = strlen(ciphertext);
    for (int j = 0; j < length; j += 8)
    {
        uint64_t *data = 0;
        int blockLength = (j + 8 < length) ? 8 : length - j; // Determine the length of the current block
        for (int i = 0; i < blockLength * 8; i++)
            *data |= ((uint64_t)(ciphertext[j + i / 8] >> (7 - (i % 8))) & 0x1) << (63 - i);
        uint64_t tempData = 0;
        // IP^-1
        initialPermutationReverse(data);
        for (int i = 0; i < 16; i++)
        {
            // 16 rounds
            uint32_t leftData = (*data) >> 32;
            uint32_t rightData = (*data) & 0xffffffff;
            roundHandlerReverse(&leftData, &rightData, &roundKeys[15 - i]);
            tempData = ((uint64_t)leftData << 32) | rightData;
        }
        *data = tempData;
        // IP
        initialPermutation(data);
        // Convert the 64-bit data back to char array
        for (int i = 0; i < blockLength; i++)
            plaintext[j + i] = (char)((*data >> (56 - 8 * i)) & 0xFF);
    }
    return;
}
