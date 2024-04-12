#include <gtest/gtest.h>
#include "RSA.h"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
TEST(RSAUtilsTests, ModInverseTest)
{
    RSAUtils rsa;

    // Test case 1: a = 1769, n = 550
    uint64_t result = rsa.modInverse(1769, 550);
    EXPECT_EQ(result, 379);

    // Test case 2: a = 3413568960882452063, n = 8097058363760347660
    result = rsa.modInverse(3413568960882452063, 8097058363760347660);
    EXPECT_EQ(result, 2395369615547321587);
}