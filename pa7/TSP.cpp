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

void OutputInitPop(std::vector<std::vector<int>>& population, std::ofstream& output)
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

std::vector<std::pair<int, double>> ComputeFitness(std::vector<std::vector<int>>& population, std::vector<Location>& locations)
{
	std::vector<std::pair<int, double>> fitness(population.size());
	int i = -1;
	std::generate(fitness.begin(), fitness.end(), [&population, &i, &locations]
	{
		i++;
		return CalcEachFitScore(population[i], i, locations);	// Calaulate each pair
	});
	return fitness;
}

std::pair<int, double> CalcEachFitScore(std::vector<int>& populationNums, int& i, std::vector<Location>& locations)
{
	std::pair<int, double> retVal;
	retVal.first = i;				// the first number in the pair is the index
	std::vector<std::pair<double, double>> longLats = GetLongLats(populationNums, locations);		// Get the Longitude/Latitude pair of every stop
	std::vector<double> distances(longLats.size());
	int j = -1;
	std::generate(distances.begin(), distances.end() - 1, [&longLats, &j]() {
		j++;
		return Haversine(longLats[j], longLats[j + 1]);									// Calculate the distance between each stop
	});
	double sum = Haversine(longLats[longLats.size() - 1], longLats[0]);					// Get the distance from the last stop back to LAX
	sum = std::accumulate(distances.begin(), distances.end(), sum, [&sum](const double& a, const double& b) {		// Add the distances up using Sum Vector
		return a + b;
	});
	retVal.second = sum;
	return retVal;
}

std::vector<std::pair<double, double>> GetLongLats(std::vector<int>& indices, std::vector<Location>& locations)
{
	std::vector<std::pair<double, double>> longLats(indices.size());
	int i = -1;
	std::generate(longLats.begin(), longLats.end(), [&indices, &i, &locations]() {
		i++;
		return std::make_pair(locations[indices[i]].mLatitude, locations[indices[i]].mLongitude);	// Get the corresponding Latitude and Longitude
	});
	return longLats;
}

double Haversine(std::pair<double, double>& start, std::pair<double, double>& stop)
{
	double multiplier = 0.0174533;
	double dlon = (stop.second * multiplier) - (start.second * multiplier);
	double dlat = (stop.first * multiplier) - (start.first * multiplier);
	double a = sin(dlat / 2) * (sin(dlat / 2)) + cos(start.first * multiplier) * cos(stop.first * multiplier) * sin(dlon / 2) * sin(dlon / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return 3961 * c;
}

