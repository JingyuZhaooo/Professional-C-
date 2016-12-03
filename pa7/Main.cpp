#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>

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
		OutputInitPop(initRandPop, output);

	}

	// Compute the fitness of each member of the population
	std::vector<std::pair<int, double>> fitness = ComputeFitness(initRandPop, Location);
	// Output the Fitness result
	output << "FITNESS:" << std::endl;
	for (auto &i : fitness)
	{
		output << i.first << ":" << i.second << std::endl;
	}
	return 0;
}
