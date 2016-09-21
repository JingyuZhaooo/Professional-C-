#include "Dictionary.h"
#include <fstream>
#include "Sha1.h"
#include "Timer.h"
#include <iostream>

Dictionary::Dictionary()
{
	mDictionaryMap.rehash(100000);
}

Dictionary::~Dictionary()
{

}

std::string* Dictionary::FindMatch(std::string& key)
{
	auto iter = mDictionaryMap.find(key);
	if (iter != mDictionaryMap.end())
	{
		return &(iter->second);
	}
	else
	{
		return nullptr;
	}
}

void Dictionary::Load(std::ifstream& fileName)
{
	Timer timer;
	timer.start();
	std::string str;
	unsigned char hash[20];
	char hex_str[41];
	while (std::getline(fileName, str))
	{
		sha1::calc(str.c_str(), str.length(), hash); // hash the string str, expecting a C-style string
		sha1::toHexString(hash, hex_str); // convert the hash into hexadecimal string
		mDictionaryMap.emplace(static_cast<std::string>(hex_str), str);	// add the PasswordToHash to mDictionaryMap
	}
	double elapsed = timer.getElapsed();
	std::cout << elapsed << std::endl;
}
