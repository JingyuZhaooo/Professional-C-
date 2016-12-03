#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <iostream>

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
	std::vector<std::vector<int>> retVal(popsize);
	std::generate(retVal.begin(), retVal.end(), [&random, &numOfLoc]
	{
		return ShuffleNumbers(random, numOfLoc);
	});
	return retVal;
}

std::vector<int> ShuffleNumbers(std::mt19937 & random, int numOfLoc)
{
	std::vector<int> initialVec(numOfLoc);
	int num = 0;
	std::generate(initialVec.begin(), initialVec.end(), [&num]
	{
		return num++;
	});
	std::shuffle(initialVec.begin() + 1, initialVec.end(), random);
	return initialVec;
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

std::vector<std::pair<int, double>> ComputeFitness(const std::vector<std::vector<int>>& population, const std::vector<Location>& locations)
{
	std::vector<std::pair<int, double>> fitness(population.size());
	int i = -1;
	//std::generate(fitness.begin(),)


	return std::vector<std::pair<int, double>>();
}
