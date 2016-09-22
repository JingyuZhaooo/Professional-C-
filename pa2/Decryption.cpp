#include "Decryption.h"
#include <fstream>
#include <iostream>
#include <tbb/parallel_invoke.h>
#include "Sha1.h"
#include <string>
#include "Timer.h"
#include <ctype.h>

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
	
	int entryNumCount = 1;
	
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
			entryNumCount++;
		}
		else
		{
			currInfo->textSol = "?????";
			unsolved.push_back(currInfo);
			entryNumCount++;
		}
	}

	
	Timer timer1;
	timer1.start();
	for (auto& i : unsolved)
	{
		BruteForce(i);
		mSolved.emplace(i->entryNum, i);
	}
	double elapsed1 = timer1.getElapsed();
	std::cout << "Serial Brute Force: " << elapsed1 << " seconds" << std::endl;

	

	Timer timer;
	timer.start();
	tbb::parallel_invoke(
		[&, this]() { BruteForceHelper(unsolved, "aaaa", "d999"); },
		[&, this]() { BruteForceHelper(unsolved, "eaaa", "h999"); },
		[&, this]() { BruteForceHelper(unsolved, "iaaa", "l999"); },
		[&, this]() { BruteForceHelper(unsolved, "maaa", "p999"); },
		[&, this]() { BruteForceHelper(unsolved, "qaaa", "t999"); },
		[&, this]() { BruteForceHelper(unsolved, "uaaa", "x999"); },
		[&, this]() { BruteForceHelper(unsolved, "yaaa", "1999"); },
		[&, this]() { BruteForceHelper(unsolved, "2aaa", "5999"); },
		[&, this]() { BruteForceHelper(unsolved, "6aaa", "9999"); }
	);
	double elapsed = timer.getElapsed();
	std::cout << "Parallel Brute Force: " << elapsed << " seconds" << std::endl;
	for (auto i : unsolved)
	{
		mSolved.emplace(i->entryNum, i);
	}
	std::ofstream text("pass_solved.txt");
	if (text.is_open())
	{
		for (int i = 1; i < entryNumCount; i++)
		{
			text << mSolved.find(i)->second->hex_str << "," << mSolved.find(i)->second->textSol << "\n";
		}
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
		std::string possiblePassword;
		Convert36(countingMachine, possiblePassword);
		sha1::calc(possiblePassword.c_str(), possiblePassword.length(), hash);
		sha1::toHexString(hash, hex_str);
		if (strcmp(hex_str,info->hex_str.c_str()) == 0)
		{
			info->textSol = possiblePassword;
			return;
		}
		if (countingMachine[3] < 36) // if the last number is less than 35, we can add one to it
		{
			countingMachine[3] += 1;
		}
		else if (countingMachine[2] < 36 ) // if the last number is 35, we can set it back to zero and increase the 3rd number by one
		{
			countingMachine[2] += 1;
			countingMachine[3] = 0;
		}
		else if (countingMachine[1] < 36)
		{
			countingMachine[1] += 1;
			countingMachine[2] = 0;
			countingMachine[3] = 0;
		}
		else if (countingMachine[0] < 36)
		{
			countingMachine[0] += 1;
			countingMachine[1] = 0;
			countingMachine[2] = 0;
			countingMachine[3] = 0;
		}
		else if (countingMachine[0] = 36)
		{
			break;
		}
	}
	return;
}

void Decryption::Convert36(int count[], std::string& str) // convert the base 36 number to a string
{
	for (int i = 3; i >= 0; i--)
	{
		if (count[i] == -1)
		{
			break;
		}
		if (count[i] < 26)
		{
			str += (count[i] + 'a');
		}
		else
		{
			str += ((count[i] - 26) + '0');
		}
	}
}

void Decryption::BruteForceHelper(std::vector<decryptedInfo*>& unsolved, std::string starting, std::string ending)
{
	unsigned char hash[20];
	char hex_str[41];
	int countingMachine[4];
	char char1 = starting[0];
	char char2 = starting[1];
	char char3 = starting[2];
	char char4 = starting[3];
	//std::cout << int(char1) << std::endl;
	if (isalpha(char1))
	{ 
		countingMachine[0] = char1 - 97;
	}
	else
	{
		countingMachine[0] = char1 - 22;
	}
	if (isalpha(char2))
	{
		countingMachine[1] = char2 - 97;
	}
	else
	{
		countingMachine[1] = char2 - 22;
	}
	if (isalpha(char3))
	{
		countingMachine[2] = char3 - 97;
	}
	else
	{
		countingMachine[2] = char3 - 22;
	}
	if (isalpha(char4))
	{
		countingMachine[3] = char4 - 97;
	}
	else
	{
		countingMachine[3] = char4 - 22;
	}
	
	for (auto& info : unsolved)
	{
		while(true)
		{
			std::string toEvaluate;
			Convert36(countingMachine, toEvaluate);
			sha1::calc(toEvaluate.c_str(), toEvaluate.length(), hash);
			sha1::toHexString(hash, hex_str);
			if (strcmp(hex_str, info->hex_str.c_str()) == 0)
			{
				info->textSol = toEvaluate;
				break;
			}
			if (toEvaluate.compare(ending) == 0) // found it!
			{
				break;
			}
			if (countingMachine[3] < 36) // if the last number is less than 35, we can add one to it
			{
				countingMachine[3] += 1;
			}
			else if (countingMachine[2] < 36) // if the last number is 35, we can set it back to zero and increase the 3rd number by one
			{
				countingMachine[2] += 1;
				countingMachine[3] = 0;
			}
			else if (countingMachine[1] < 36)
			{
				countingMachine[1] += 1;
				countingMachine[2] = 0;
				countingMachine[3] = 0;
			}
			else if (countingMachine[0] < 36)
			{
				countingMachine[0] += 1;
				countingMachine[1] = 0;
				countingMachine[2] = 0;
				countingMachine[3] = 0;
			}
			else if (countingMachine[0] = 36)
			{
				break;
			}
		}
		return;
	}
}
	