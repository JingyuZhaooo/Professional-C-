#pragma once
#include <string>
#include <iostream>
#include <fstream>

class FASTAFile
{
public:
	FASTAFile();
	void Load(std::string filePath);
private:
	std::string mHeader;
	std::string mSequence;
};