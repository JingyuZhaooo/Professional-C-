#include "FASTAFile.h"
#include "Timer.h"
#include "Exceptions.h"
#include <wx/msgdlg.h>

FASTAFile::FASTAFile()
{
	mHeader = "";
	mSequence = "";
}

void FASTAFile::Load(std::string filePath)
{
	std::ifstream::pos_type size;
	std::ifstream myFASTA;
	myFASTA.open(filePath, std::ios::in | std::ios::ate);
	if (myFASTA.is_open())
	{
		Timer timer;
		timer.Start();
		size = myFASTA.tellg(); // Save the size of the file
		myFASTA.seekg(0, std::ios::beg); // Seek back to start of file
		mSequence.reserve(static_cast<int>(size));
		std::string str;
		std::getline(myFASTA, str); //get the first line (header)
		if (str[0] != '>')
		{
			try
			{
				throw FileLoadExcept();
			}

			catch (std::exception& e)
			{
				wxMessageBox("FASTA file is invalid", "Error", wxOK | wxICON_ERROR);
				std::cout << e.what() << std::endl;
			}
			return;
		}
		else
		{
			mHeader = str.substr(1); // store the header
			std::cout << mHeader << std::endl;
			while (!myFASTA.eof())
			{
				char newChar;
				myFASTA.get(newChar);
				if (newChar == 'T' || newChar == 'C' || newChar == 'A' || newChar == 'G')
				{
					mSequence += newChar;
				}
				else if (newChar == '\n' || newChar == '\r' || newChar == '\xff')
				{
					continue;
				}
				else
				{
					try
					{
						throw FileLoadExcept();
					}
					catch (std::exception& e)
					{
						wxMessageBox("FASTA file is invalid", "Error", wxOK | wxICON_ERROR);
						std::cout << e.what() << std::endl;
					}
					return;
				}
			}
			std::cout << mSequence << std::endl;
		}
			
		
		double elapsed = timer.GetElapsed();
		std::cout << "Loading the FASTA file takes: " << elapsed << " seconds" << std::endl;
	}
}
