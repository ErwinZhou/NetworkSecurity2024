#include "DES.h"

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
        subKey <<= 16; // Shift the subKey 48 bits to the high 48 bits
        roundKeys[i - 1] = subKey;
    }
    return SUCCESS;
}

INT32 DESUtils::padding(vector<uint8_t> &data)
{
    /**
     * @brief Padding the data to be a multiple of 64 bits (8 bytes)
     * @param data: the data to be padded, modified in place
     * @return: the result of padding
     */
    size_t blockSize = 8; // Block size in bytes (64 bits)
    size_t padLength = blockSize - (data.size() % blockSize);

    // If the data size is already a multiple of the block size, add a new block of padding
    if (padLength == 8)
        padLength = 0;
    for (size_t i = 0; i < padLength; i++)
        data.push_back(static_cast<uint8_t>(padLength));
    return SUCCESS;
}
INT32 DESUtils::unpadding(vector<uint8_t> &data)
{
    /**
     * @brief Unpadding the data to be a multiple of 64 bits (8 bytes)
     * @param data: the data to be unpadded, modified in place
     * @return: the result of unpadding
     */
    size_t blockSize = 8; // Block size in bytes (64 bits)
    if (data.size() == 0)
        // If the data is empty, there is no need to unpad
        return SUCCESS;
    size_t padLength = data.back();
    if (padLength > blockSize || padLength > data.size())
        // If the pad length is greater than the block size, return the data without unpadding
        return SUCCESS;
    data.resize(data.size() - padLength);
    return SUCCESS;
}

INT32 DESUtils::initialPermutation(uint64_t *data)
{
    /**
     * @brief The first step of DES encryption using IP, before 16 rounds
     * param data: the data to be permutated
     * return: the result of permutation
     */
    uint64_t output = 0;
    for (int i = 0; i < 64; i++)
        output |= (*data >> (64 - ip[i]) & 0x1) << (63 - i);
    *data = output;
    return SUCCESS;
}
INT32 DESUtils::expansionBox(uint32_t *data, uint64_t &output)
{
    /**
     * @brief Expansion Box used for roundHandler to expand the data from 32bits to 48bits
     * @param data: right data with 32bits waiting to be expanded in 48bits
     * @param output: the expanded data saved in 64 bits
     * @return: the result of the expansion
     */
    for (int i = 0; i < 48; i++)
    {
        output |= (static_cast<uint64_t>((*data >> (32 - EBox[i])) & 0x1) << (63 - i));
    }
    return SUCCESS;
}
INT32 DESUtils::extractionBox(uint64_t *data, uint32_t &output)
{
    /**
     * @brief Extraction Box used for roundHandler to extract data from 48bits back to 32bits
     * param data: the xored data with 48bits waiting to be extracted to 32bits
     * return: the result of the extraction
     */
    for (int i = 0; i < 8; i++)
    {
        bitset<6> subDataBits((*data >> (16 + 6 * i)) & 0x3F); // Store 6 bits in a bitset
        output |= static_cast<uint32_t>((SBox[7 - i][subDataBits.to_ulong()] | 0x0) << (4 * i));
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
    uint32_t output = 0;
    for (int i = 0; i < 32; i++)
        output |= ((*data >> (32 - PBox[i])) & 0x1) << (31 - i);
    *data = output;
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
    // f function for this round
    uint64_t expandedData = 0;
    expansionBox(rightData, expandedData);
    expandedData ^= *roundKey;
    uint32_t extractedData = 0;
    extractionBox(&expandedData, extractedData);
    permutation(&extractedData);
    uint32_t tempLeftData = *leftData;
    *leftData = *rightData;
    *rightData = tempLeftData ^ extractedData;
    return SUCCESS;
}
INT32 DESUtils::initialPermutationReverse(uint64_t *data)
{
    /**
     * @brief The last step of DES encryption using IP^-1, after 16 rounds
     * param data: the data to be permutated
     * return: the result of permutation
     */
    uint64_t output = 0;
    for (int i = 0; i < 64; i++)
        output |= (*data >> (64 - ip_reverse[i]) & 0x1) << (63 - i);
    *data = output;
    return SUCCESS;
}
void DESUtils::encrypt(const char *plaintext, char *ciphertext)
{
    /**
     * @brief Encrypt the plaintext using DES
     * param plaintext: the plaintext to be encrypted
     * param key: the key for encryption
     * param ciphertext: the result of encryption
     */
    // Divide the plaintext into 64bits in a group
    // Convert the plaintext from a char array to a vector of uint8_t for further processing
    vector<uint8_t> plaintextVec(plaintext, plaintext + strlen(plaintext));
    // Apply padding to the plaintext vector to ensure its size is a multiple of the block size
    padding(plaintextVec);
    int length = plaintextVec.size();
    for (int j = 0; j < length; j += 8)
    {
        uint64_t data = 0;
        int blockLength = (j + 8 < length) ? 8 : length - j; // Determine the length of the current block
        for (int i = 0; i < blockLength * 8; i++)
            data |= ((uint64_t)(plaintextVec[j + i / 8] >> (7 - (i % 8))) & 0x1) << (63 - i);

        uint64_t tempData = 0;
        // IP
        initialPermutation(&data);
        uint32_t leftData = data >> 32;
        uint32_t rightData = data & 0xffffffff;
        for (int i = 0; i < 16; i++)
        {
            // 16 rounds
            roundHandler(&leftData, &rightData, &roundKeys[i]);
            if (i == 15)
                swap(leftData, rightData);
            tempData = ((uint64_t)leftData << 32) | rightData;
        }
        data = tempData;
        // IP^-1
        initialPermutationReverse(&data);
        // Convert the 64-bit data back to char array
        for (int i = 0; i < blockLength; i++)
            ciphertext[j + i] = (char)((data >> (56 - 8 * i)) & 0xFF);
    }
    return;
}
void DESUtils::decrypt(const char *ciphertext, char *plaintext)
{
    /**
     * @brief Decrypt the ciphertext using DES
     *        The only difference between encryption and decryption is the order of the roundKeys
     * param ciphertext: the ciphertext to be decrypted
     * param key: the key for decryption
     * param plaintext: the result of decryption
     */
    // Divide the ciphertext into 64bits in a group
    // Convert the ciphertext from a char array to a vector of uint8_t for further processing
    vector<uint8_t> ciphertextVec(ciphertext, ciphertext + strlen(ciphertext));
    int length = ciphertextVec.size();
    for (int j = 0; j < length; j += 8)
    {
        uint64_t data = 0;
        int blockLength = (j + 8 < length) ? 8 : length - j; // Determine the length of the current block
        for (int i = 0; i < blockLength * 8; i++)
            data |= ((uint64_t)(ciphertext[j + i / 8] >> (7 - (i % 8))) & 0x1) << (63 - i);
        uint64_t tempData = 0;
        // IP
        initialPermutation(&data);
        uint32_t leftData = data >> 32;
        uint32_t rightData = data & 0xffffffff;
        for (int i = 0; i < 16; i++)
        {
            // 16 rounds
            roundHandler(&leftData, &rightData, &roundKeys[15 - i]);
            if (i == 15)
                swap(leftData, rightData);
            tempData = ((uint64_t)leftData << 32) | rightData;
        }
        data = tempData;
        // IP^-1
        initialPermutationReverse(&data);
        // Convert the 64-bit data back to char array
        for (int i = 0; i < blockLength; i++)
            plaintext[j + i] = (char)((data >> (56 - 8 * i)) & 0xFF);
    }
    // Unpadding the plaintext
    vector<uint8_t> plaintextVec(plaintext, plaintext + strlen(plaintext));

    unpadding(plaintextVec);
    for (int i = 0; i < plaintextVec.size(); i++)
        plaintext[i] = plaintextVec[i];

    plaintext[plaintextVec.size()] = '\0';
    return;
}
// int main()
// {
//     DESUtils des;
//     char *plaintext = "123456"; // 原始明文
//     char ciphertext[64];        // 存储加密后的密文
//     char decryptedtext[64];     // 存储解密后的文本

//     // 清零
//     memset(ciphertext, 0, sizeof(ciphertext));
//     memset(decryptedtext, 0, sizeof(decryptedtext));
//     uint64_t key = 0x133457799BBCDFF1;
//     des.genKey(&key);
//     // 输出16轮密钥
//     cout << "Round Keys: " << endl;
//     for (int i = 0; i < 16; i++)
//     {
//         cout << "Round " << i + 1 << ": " << hex << showbase << des.roundKeys[i] << endl;
//     }
//     // 加密
//     cout << "Encrypting..." << endl;
//     des.encrypt(plaintext, ciphertext);
//     cout << "plaintext: " << plaintext << endl;
//     for (int i = 0; i < strlen(plaintext); i++)
//     {
//         printf("%02x ", (unsigned char)plaintext[i]);
//     }
//     printf("\n");
//     cout << "ciphertext: " << ciphertext << endl;
//     for (int i = 0; i < strlen(ciphertext); i++)
//     {
//         printf("%02x ", (unsigned char)ciphertext[i]);
//     }
//     printf("\n");
//     // 解密
//     cout << "Decrypting..." << endl;
//     des.decrypt(ciphertext, decryptedtext);

//     cout << "strlen(decryptedtext): " << strlen(decryptedtext) << endl;
//     for (int i = 0; i < strlen(decryptedtext); i++)
//     {
//         printf("%02x ", (unsigned char)decryptedtext[i]);
//     }
//     printf("\n");
//     // 检查解密后的文本是否与原始明文相同
//     if (strcmp(plaintext, decryptedtext) == 0)
//     {
//         std::cout << "DES encryption and decryption are correct!" << std::endl;
//     }
//     else
//     {
//         std::cout << "Error in DES encryption or decryption." << std::endl;
//     }

//     return 0;
// }
