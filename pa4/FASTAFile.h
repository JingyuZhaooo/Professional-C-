#pragma once
#include <string>
#include <iostream>
#include <fstream>

class FASTAFile
{
public:
	FASTAFile();
	void Load(std::string filePath);
	std::string GetHeader() const { return mHeader; };
	std::string GetSequence() const { return mSequence; };
private:
	std::string mHeader;
	std::string mSequence;
};