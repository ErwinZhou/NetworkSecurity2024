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
    rsa.init();
    uint64_t plaintext = 123456789;
    uint64_t encryptedtext = 0;
    uint64_t decryptedtext = 0;
    rsa.encrypt(plaintext, encryptedtext);
    rsa.decrypt(decryptedtext, encryptedtext);
    EXPECT_EQ(plaintext, decryptedtext);
}
