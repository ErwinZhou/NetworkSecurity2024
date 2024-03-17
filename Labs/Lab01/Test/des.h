#pragma once

#include <vector>
#include <bitset>
#include <cstdint>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstring>

class DES
{
	static const uint8_t IP[64];		  // ????????
	static const uint8_t FP[64];		  // ?????????
	static const uint8_t E_box[48];		  // ???????E??
	static const uint8_t P_box[32];		  // ???????P??
	static const uint8_t S_box[8][4][16]; // 8??S??
	static const uint8_t PC_1[56];		  // PC-1???
	static const uint8_t PC_2[48];		  // ???????????
	static const uint8_t shift[16];		  // ????????¦Ë??

public:
	DES(const std::string &key);										 // ???????????64¦Ë??????????
	std::vector<uint8_t> encrypt(const std::vector<uint8_t> &plaintext); // ??????????
	std::vector<uint8_t> decrypt(const std::vector<uint8_t> &ciphertext);
	static std::vector<uint8_t> strToVec(const std::string &input)
	{
		return std::vector<uint8_t>(input.begin(), input.end());
	}
	static std::string vecToStr(const std::vector<uint8_t> &input)
	{
		return std::string(input.begin(), input.end());
	}
	static std::string vecToHexStr(const std::vector<uint8_t> &vec);

private:
	enum MODE
	{
		ENCRYPT,
		DECRYPT
	};
	std::bitset<48> subKeys[16];										   // ?›¥?????16???????
	std::bitset<64> execute(const std::bitset<64> &data, int mode);		   // ????/????
	void genSubKeys(const std::bitset<64> &key);						   // ????16???????
	std::bitset<32> f(const std::bitset<32> &R, const std::bitset<48> &K); // f????
	template <size_t N>
	std::bitset<N> leftRotate(const std::bitset<N> &bits, int shift); // ???????
	template <size_t N>
	uint8_t get(const std::bitset<N> &b, size_t pos) { return b[N - 1 - pos]; }
	template <size_t N>
	void set(std::bitset<N> &b, size_t pos, size_t value) { b[N - 1 - pos] = value; }
	std::vector<uint8_t> pad(const std::vector<uint8_t> &data, size_t blockSize);	// PKCS#7???
	std::vector<uint8_t> unpad(const std::vector<uint8_t> &data, size_t blockSize); // PKCS#7????
};
