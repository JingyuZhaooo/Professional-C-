#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>

std::vector<Location> ParseLocation(std::string& filename)
{
	std::ifstream mFile;
	mFile.open(filename);
	std::string line = "";
	std::vector<Location> retVal;
	if (!mFile.is_open()) // I guess this is not needed
	{
		std::cout << "Invalid File" << std::endl;
	}
	else
	{
		while (!mFile.eof())
		{
			std::getline(mFile, line);
			size_t index1 = line.find_first_of(","); // find the first comma
			size_t index2 = line.find_last_of(","); // find the last comma
			Location tempLoc;
			// index calculation ....
			tempLoc.mName = line.substr(0, index1);
			tempLoc.mLatitude = std::stod(line.substr(index1 + 1, index2 - index1 - 1));
			tempLoc.mLongitude = std::stod(line.substr(index2 + 1));
			retVal.push_back(tempLoc);
		}
	}
	return retVal;
}

std::vector<std::vector<int>> GenerateInitRandPop(std::mt19937& random, int popsize, int numOfLoc)
{
	std::vector<std::vector<int>> retVal;
	std::vector<int> initialVec;
	for(int i = 0; i < numOfLoc; i++)
	{
		initialVec.push_back(i);
	}
	for (int j = 0; j < popsize; j++)
	{
		std::vector<int> tempVec = initialVec;
		std::shuffle(tempVec.begin() + 1, tempVec.end(), random);
		retVal.push_back(tempVec);
	}
	return retVal;
}

void outputInitPop(std::vector<std::vector<int>>& population, std::ofstream& output)
{
	output << "INITIAL POPULATION:" << std::endl;
	for (std::vector<int>& i : population)
	{
		for (int j : i)
		{
			output << j;
			if (j != i.back())
			{
				output << ",";
			}
		}
		output << "\n";
	}
}

int main(int argc, const char* argv[])
{
	//parse out the input
	std::string inputfile = argv[1];
	int popsize = std::stoi(argv[2]);
	int generations = std::stoi(argv[3]);
	double mutationchance = std::stoi(argv[4]) / 100;
	int seed = std::stoi(argv[5]);

	// construct a std::mt19937 generator
	std::mt19937 randomGenerator(seed);
	// parse the locations from the file specified in the command line parameter
	std::vector<Location> Location = ParseLocation(inputfile);
	/*
	for (auto i : Location)
	{
		std::cout <<std::setprecision(10) << i.mName << i.mLatitude << i.mLongitude << std::endl;
	}
	*/
	// Generate the initial random population
	std::vector<std::vector<int>> initRandPop = GenerateInitRandPop(randomGenerator, popsize, Location.size());

	// Output initial population
	std::ofstream output;
	output.open("log.txt");
	if (!output.is_open())
	{
		std::cout << "Cannot create file" << std::endl;
	}
	else
	{
		outputInitPop(initRandPop, output);

	}




	return 0;
}
