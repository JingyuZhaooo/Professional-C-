#include "Decryption.h"
#include <fstream>
#include <iostream>
#include <tbb/parallel_for_each.h>
#include "Sha1.h"
#include <string>
#include "Timer.h"

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
	std::vector<decryptedInfo*> unsolved;
	std::string str;
	std::ofstream text("pass_solved.txt");
	int entryNumCount = 1;
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
				currInfo->textSol = "?????";
				unsolved.push_back(currInfo);
				text << str << ",";
				text << "?????\n";
				entryNumCount++;
			}
		}
		Timer timer;
		timer.start();
		for (auto i : unsolved)
		{
			BruteForce(i);
		}
		double elapsed = timer.getElapsed();
		std::cout << elapsed << std::endl;
		
		int unsolvedCount = 0;
		int solvedCount = 0;
		for (int i = 1; i <= entryNumCount; i++)
		{
			if (i == unsolved[unsolvedCount]->entryNum)
			{
				text << unsolved[unsolvedCount]->hex_str << "," << unsolved[unsolvedCount]->textSol << "\n";
				unsolvedCount++;
			}
			else
			{
				text << mSolved.find(i)->second->hex_str << "," << mSolved.find(i)->second->textSol << "\n";
				solvedCount++;
			}
		}
		
		/*
		tbb::parallel_for_each(unsolved.begin(), unsolved.end(), [this](decryptedInfo* info)
		{
			BruteForce(info);
		});
		*/
		text.close();
	}
	
}

void Decryption::BruteForce(decryptedInfo* info)
{
	int countingMachine[] = { -1, -1, -1, -1 }; //using base 36
	int count = 0; // this records the last number
	unsigned char hash[20];
	char hex_str[41];
	while (true)
	{
		countingMachine[0] = count;
		std::string possiblePassword;
		Convert36(countingMachine, possiblePassword);
		sha1::calc(possiblePassword.c_str(), possiblePassword.length(), hash);
		sha1::toHexString(hash, hex_str);
		if (strcmp(hex_str,info->hex_str.c_str()) == 0)
		{
			info->textSol = possiblePassword;
			return;
		}
		if (countingMachine[0] < 35) // if the last number is less than 35, we can add one to it
		{
			count++;
		}
		else if (countingMachine[0] == 35) // if the last number is 35, we can set it back to zero and increase the 3rd number by one
		{
			countingMachine[1] += 1;
			count = 0;
			if (countingMachine[1] == 35)
			{
				countingMachine[1] = 0;
				countingMachine[2] += 1;
				if (countingMachine[2] == 35)
				{
					countingMachine[1] = 0;
					countingMachine[2] = 0;
					countingMachine[3] += 1;
					if (countingMachine[3] = 35)
					{
						break;
					}
				}
			}
		}
	}
	return;
}

void Decryption::Convert36(int count[], std::string& str) // convert the base 36 number to a string
{
	for (unsigned i = 0; i < 4; i++)
	{
		if (count[i] == -1)
		{
			break;
		}
		else if (count[i] < 26)
		{
			str += ('a' + count[i]);
		}
		else
		{
			str += ('0' + (count[i] - 26));
		}
	}
}