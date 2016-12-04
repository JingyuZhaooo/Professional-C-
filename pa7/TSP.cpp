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
	std::vector<int> initialVec(numOfLoc);	// Create a new vector each time
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
	double sum = std::accumulate(distances.begin(), distances.end(), 0.0f, [](const double& a, const double& b) {		// Add the distances up using Sum Vector
		return a + b;
	});
	double sum2 = Haversine(longLats[longLats.size() - 1], longLats[0]);					// Get the distance from the last stop back to LAX
	retVal.second = sum + sum2;
	return retVal;
}

std::vector<std::pair<double, double>> GetLongLats(std::vector<int>& populationNums, std::vector<Location>& locations)
{
	std::vector<std::pair<double, double>> longLats(populationNums.size());
	int i = -1;
	std::generate(longLats.begin(), longLats.end(), [&populationNums, &i, &locations]() {
		i++;
		return std::make_pair(locations[populationNums[i]].mLatitude, locations[populationNums[i]].mLongitude);	// Get the corresponding Latitude and Longitude
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

std::vector<std::pair<int, int>> SelectedPairs(std::vector<std::pair<int, double>> fitness, int popsize, std::mt19937& randomGenerator)
{
	std::sort(fitness.begin(), fitness.end(), myComparator);
	std::vector<double> probabilities = GenerateProbVec(popsize, fitness);
	std::vector<std::pair<int, int>> retVal;
	// Pick two random parents to form a pair
	for (int i = 0; i < popsize; i++)
	{
		std::pair<int, int> tempPair = MakePair(probabilities, randomGenerator);
		retVal.push_back(tempPair);
	}
	return retVal;
}

bool myComparator(std::pair<int, double>& lhs, std::pair<int, double>& rhs)
{
	return lhs.second < rhs.second;
}

std::vector<double> GenerateProbVec(int popsize, std::vector<std::pair<int, double>> selectedPairs)
{
	std::vector<double> probabilities(popsize);
	std::generate(probabilities.begin(), probabilities.end(), [&popsize]()
	{
		return 1.0 / popsize;
	});
	probabilities[selectedPairs[0].first] = probabilities[selectedPairs[0].first] * 6;
	probabilities[selectedPairs[1].first] = probabilities[selectedPairs[1].first] * 6;
	for (int i = 2; i < popsize / 2; i++)
	{
		probabilities[selectedPairs[i].first] *= 3.0f;
	}
	double sum = std::accumulate(probabilities.begin(), probabilities.end(), 0.0f, [](const double& a, const double& b) {
		return a + b;
	});
	std::transform(probabilities.begin(), probabilities.end(), probabilities.begin(), [&sum](double n)
	{
		return n / sum;
	});
	return probabilities;
}

int ChooseParent(double rand, std::vector<double> probabilities)
{
	double sum = 0.0;
	int index = -1;
	while (sum < rand)
	{
		index++;
		sum += probabilities[index];
	}
	return index;
}

std::pair<int, int> MakePair(std::vector<double>& probabilities, std::mt19937 & randomGenerator)
{
	std::uniform_real_distribution<double> dist(0, 1);
	double rand1 = dist(randomGenerator);
	int parentIndex = ChooseParent(rand1, probabilities);
	double rand2 = dist(randomGenerator);
	int parentIndex2 = ChooseParent(rand2, probabilities);
	std::pair<int, int> retVal = std::make_pair(parentIndex, parentIndex2);
	return retVal;
}

void OutputSelection(std::vector<std::pair<int, int>> selectedPairs, std::ofstream& output)
{
	output << "SELECTED PAIRS:" << std::endl;
	std::for_each(selectedPairs.begin(), selectedPairs.end(), [&output](std::pair<int, int> pair)
	{
		output << "(" << pair.first << "," << pair.second << ")" << std::endl;
	});
}

