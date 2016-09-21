#include "Decryption.h"
#include <fstream>

#include <iostream>

Decryption::Decryption()
{
	
}

Decryption::~Decryption()
{
	for (auto iter : mSolved)
	{
		delete iter.second;
	}
}
void Decryption::Decrypt(Dictionary& myDict, std::ifstream& fileName)
{
	std::vector<decryptedInfo*> mUnsolved;
	std::string str;
	std::ofstream text("pass_solved.txt");
	unsigned entryNumCount = 0;
	if(text.is_open())
	{ 
		while (std::getline(fileName, str))
		{
			std::string* password = myDict.FindMatch(str);

			decryptedInfo* currInfo = new decryptedInfo();
			currInfo->entryNum = entryNumCount;
			currInfo->hex_str = str;
			if (password != nullptr)
			{
				currInfo->textSol = *password;
				mSolved.emplace(currInfo->entryNum, currInfo);
				text << str << ",";
				text << *password << "\n";
				entryNumCount++;
			}
			else
			{
				mUnsolved.push_back(currInfo);
				text << str << ",";
				text << "?????\n";
				entryNumCount++;
			}
		}
		text.close();
	}
	
}