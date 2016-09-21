// Main.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>
#include "Sha1.h"
#include <fstream>
#include "Dictionary.h"
#include "Decryption.h"

int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		unsigned char hash[20]; 
		sha1::calc(argv[1], strlen(argv[1]), hash);
		char hex_str[41];
		sha1::toHexString(hash, hex_str);
		std::cout << hex_str << std::endl;
	}
	
	if (argc == 3)
	{
		std::ifstream myDict(argv[1]);
		if (myDict.is_open())		//check if the file exists
		{
			Dictionary myDictionary;
			myDictionary.Load(myDict);
			
			std::ifstream myPasswords(argv[2]);
			if (myPasswords.is_open())
			{
				Decryption myDecryption;
				myDecryption.Decrypt(myDictionary,myPasswords);
			}
			
		}
		else
		{
			std::cout << "Cannot open dictionary file" << std::endl;
		}
		
	}
	
	return 0;
}

