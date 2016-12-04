#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>

void multiply3(double& a) { a = a * 3; };

int main(int argc, const char* argv[])
{
	//parse out the input
	std::string inputfile = argv[1];
	int popsize = std::stoi(argv[2]);
	int generations = std::stoi(argv[3]);
	double mutationchance = std::stod(argv[4]) / 100.0;
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

	// Part 2: Compute the fitness of each member of the population
	std::vector<std::pair<int, double>> fitness = ComputeFitness(initRandPop, Location);
	OutputFitness(fitness, output);
	
	std::vector<std::vector<int>> newPopulation;
	for (int i = 1; i <= generations; i++)
	{
		// Part 3
		std::vector<std::pair<int, int>> selectedPairs = SelectedPairs(fitness, popsize, randomGenerator);
		OutputSelection(selectedPairs, output);
	
		// Part 4
		newPopulation = Crossover(randomGenerator, Location.size(), selectedPairs, initRandPop, mutationchance);
		initRandPop = newPopulation;
		OutputGenerations(newPopulation, output, i);

		fitness = ComputeFitness(newPopulation, Location);
		OutputFitness(fitness, output);
	}
	
	// Output Solution
	std::sort(fitness.begin(), fitness.end(), myComparator);
	output << "SOLUTION:" << std::endl;
	std::for_each(newPopulation[fitness[0].first].begin(), newPopulation[fitness[0].first].end(), [&output, &Location](int i)
	{
		output << Location[i].mName << std::endl;
	});
	output << Location[0].mName << std::endl;
	output << "DISTANCE: " << fitness[0].second << " miles";
	return 0;
}
