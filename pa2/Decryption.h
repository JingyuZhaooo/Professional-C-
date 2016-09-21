#pragma once
#include "Dictionary.h"
#include <vector>

class Decryption
{
public:
	Decryption();
	~Decryption();
	void Decrypt(Dictionary& myDict, std::ifstream& fileName);
private:
	struct decryptedInfo
	{
		unsigned entryNum;
		std::string hex_str;
		std::string textSol;
	};
	std::unordered_map<unsigned, decryptedInfo*> mSolved;
};