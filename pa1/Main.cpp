// Main.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include "RleTests.h"
#include "RleData.h"
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include "RLEFile.h"


void Part1Tests()
{
	TestFixtureFactory::theInstance().runTests();
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		// Just the tests
		Part1Tests();
	}
	else
	{
		// TODO: Get the file name from argv[1] and either compress/decompress
		std::string fileName = argv[1];
		int length = strlen(argv[1]);
		std::cout << length << std::endl;
		std::string suffix = ".rl1";
		if (fileName.find(suffix) == length - 4) // found the suffix, compress file
		{
				
			RleFile file;
			file.CreateArchive(argv[1]);
			
		}
		else if (fileName.find(suffix) == std::string::npos || fileName.find(suffix) != length - 4) // decompress the file
		{
			RleFile file;
			file.ExtractArchive(argv[1]);
		}
		
	}
	return 0;
}

