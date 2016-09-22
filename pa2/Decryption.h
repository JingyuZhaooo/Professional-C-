#pragma once
#include "Dictionary.h"
#include <vector>

class Decryption
{
	struct decryptedInfo
	{
		unsigned entryNum;
		std::string hex_str;
		std::string textSol;
	};
public:
	Decryption();
	~Decryption();
	void Decrypt(Dictionary& myDict, std::ifstream& fileName);
	void BruteForce(decryptedInfo* info);
	void Convert36(int count[], std::string& str);
private:
	
	std::unordered_map<unsigned, decryptedInfo*> mSolved;
	void BruteForceHelper(std::vector<decryptedInfo*>& unsolved, std::string starting, std::string ending);
};