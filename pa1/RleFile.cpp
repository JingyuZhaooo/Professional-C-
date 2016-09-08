#include "RLEFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "FileSystem.h"

void RleFile::CreateArchive(const std::string& source)
{
	
	std::ifstream::pos_type size;
	char* memblock; // Open the file for input, in binary mode, and start ATE (at the end)
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg(); // Save the size of the file
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); // Seek back to start of file
		file.read(memblock, size);
		file.close();
		// File data has now been loaded into memblock array

		mData.Compress(memblock, static_cast<size_t>(size)); // call the compress function and pass in the parameters
		int comptressedSize = mData.mSize;
		int	percent = 100 * (comptressedSize) / static_cast<int>(size); // calculate the compression percentage
		std::cout << percent << "% compression was achieved!" << std::endl; 

		// Write the header
		mHeader.sig[0] = 'R';
		mHeader.sig[1] = 'L';
		mHeader.sig[2] = 'E';
		mHeader.sig[3] = 0x01;
		mHeader.fileSize = static_cast<int>(size);
		mHeader.fileNameLength = static_cast<unsigned char>(source.length());
		mHeader.fileName = source;
		
		std::string outFileName = source + ".rl1"; // create the archieve's file name
		// Open the file for output, in binary mode, and overwrite an existing file
		std::ofstream arc(outFileName, std::ios::out | std::ios::binary | std::ios::trunc);
		if (arc.is_open())
		{
			// Use arc.write function to write data here // where to use c_str??
			arc.write(mHeader.sig, 4);
			arc.write(reinterpret_cast<char*>(&(mHeader.fileSize)), 4);
			arc.write(reinterpret_cast<char*>(&(mHeader.fileNameLength)), 1);
			arc.write(reinterpret_cast<char*>(&(mHeader.fileName)), source.length());
			arc.write(mData.mData, mData.mSize);
		}

		// Make sure to clean up!
		delete[] memblock;
	}
	else
	{
		std::cout << "Cannot open file!" << std::endl;
	}
}

void RleFile::ExtractArchive(const std::string& source)
{
	std::ifstream::pos_type size;
	char* memblock; // Open the file for input, in binary mode, and start ATE (at the end)
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg(); // Save the size of the file
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); // Seek back to start of file
		file.read(memblock, size);
		file.close();


		// grab the header
		mHeader.sig[0] = memblock[0];
		mHeader.sig[1] = memblock[1];
		mHeader.sig[2] = memblock[2];
		mHeader.sig[3] = memblock[3];
	


		//check the header
		if (mHeader.sig[0] == 'R' && mHeader.sig[1] == 'L' && mHeader.sig[2] == 'E' && mHeader.sig[3] == 0x01)
		{
			mHeader.fileSize = *(reinterpret_cast<int*>(&memblock[4]));
			mHeader.fileNameLength = *(reinterpret_cast<char*>(&memblock[8]));  //grab the length of the file name
			for (int i = 1; i <= mHeader.fileNameLength; i++)
			{
				mHeader.fileName += memblock[8 + i];
			}
			mData.Decompress(memblock + 9 + mHeader.fileNameLength, static_cast<size_t>(size) - 9 - mHeader.fileNameLength, mHeader.fileSize);

			// Open the file for output, in binary mode, and overwrite an existing file
			std::ofstream arc(mHeader.fileName, std::ios::out | std::ios::binary | std::ios::trunc);
			if (arc.is_open())
			{
				// Use arc.write function to write data here // where to use c_str??
				arc.write(mData.mData, mData.mSize);
			}
			delete[] memblock;
		}

	}
	else
	{
		std::cout << "Invalid file" << std::endl;
	}
}
