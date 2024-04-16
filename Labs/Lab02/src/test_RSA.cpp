#include <gtest/gtest.h>
#include "RSA.h"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
TEST(RSAUtilsTests, Test1)
{
    RSAUtils rsa;
    rsa.init(10000);
    cout << "Public key:(" << rsa.getPublicKey().first << ", " << rsa.getPublicKey().second << ")" << endl;
    cout << "Private key:(" << rsa.getPrivateKey().first << ", " << rsa.getPrivateKey().second << ")" << endl;
    uint64_t plaintext = 123456789;
    uint64_t encryptedtext = 0;
    uint64_t decryptedtext = 0;
    rsa.encrypt(plaintext, encryptedtext);
    cout << "Encrypted text: " << encryptedtext << endl;
    rsa.decrypt(decryptedtext, encryptedtext);
    cout << "Decrypted text: " << decryptedtext << endl;
    EXPECT_EQ(plaintext, decryptedtext);
}
TEST(RSAUtilsTests, Test2)
{
    RSAUtils rsa;
    rsa.init(10000);
    cout << "Public key:(" << rsa.getPublicKey().first << ", " << rsa.getPublicKey().second << ")" << endl;
    cout << "Private key:(" << rsa.getPrivateKey().first << ", " << rsa.getPrivateKey().second << ")" << endl;
    uint64_t plaintext = 987654321;
    uint64_t encryptedtext = 0;
    uint64_t decryptedtext = 0;
    rsa.encrypt(plaintext, encryptedtext);
    cout << "Encrypted text: " << encryptedtext << endl;
    rsa.decrypt(decryptedtext, encryptedtext);
    cout << "Decrypted text: " << decryptedtext << endl;
    EXPECT_EQ(plaintext, decryptedtext);
}