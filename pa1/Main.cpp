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
		std::string suffix = ".rl1";
		if (fileName.find(suffix) != length - 4) // didn't find the suffix, compress file
		{
			std::cout << "test Create Archive" << std::endl;
			RleFile file;
			file.CreateArchive(fileName);
			
		}
		else									// decompress the file
		{
			std::cout << "test Extract Archive" << std::endl;
			RleFile file;
			file.ExtractArchive(fileName);
		}
		
	}
	return 0;
}

