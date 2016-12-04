#pragma once
#include <string>
#include <vector>
#include <random>

struct Location
{
	std::string mName;
	double mLatitude;
	double mLongitude;
};

struct Population
{
	std::vector<std::vector<int>> mMembers;
};

// Part 1
std::vector<Location> ParseLocation(std::string& filename);
std::vector<std::vector<int>> GenerateInitRandPop(std::mt19937& random, int popsize, int numOfLoc);
std::vector<int> ShuffleNumbers(std::mt19937& rand, int numOfLoc);
void OutputInitPop(std::vector<std::vector<int>>& population, std::ofstream& output);

// Part 2
std::vector<std::pair<int, double>> ComputeFitness(std::vector<std::vector<int>>& population, std::vector<Location>& locations);
std::pair<int, double> CalcEachFitScore(std::vector<int>& populationNums, int& i, std::vector<Location>& locations);
std::vector<std::pair<double, double>> GetLongLats(std::vector<int> &populationNums, std::vector<Location> &locations);
double Haversine(std::pair<double, double>& start, std::pair<double, double>& stop);
void OutputFitness(std::vector<std::pair<int, double>>& fitness, std::ofstream& output);

// Part 3
std::vector<std::pair<int, int>> SelectedPairs(std::vector<std::pair<int, double>> fitness, int popsize, std::mt19937& randomGenerator);
bool myComparator(std::pair<int, double>& lhs, std::pair<int, double>& rhs);
std::vector<double> GenerateProbVec(int popsize, std::vector<std::pair<int, double>> selectedPairs);
int ChooseParent(double rand, std::vector<double> probabilities);
std::pair<int, int> MakePair(std::vector<double>& probabilities, std::mt19937& randomGenerator);
void OutputSelection(std::vector<std::pair<int, int>> selectedPairs, std::ofstream& output);

// Part 3
std::vector<std::vector<int>> Crossover(std::mt19937& randomGenerator, int size, std::vector<std::pair<int, int>>& selectedPairs, std::vector<std::vector<int>>& initRandPop, double mutationchance);
std::vector<int> CopyElements(std::vector<int>& parent1, std::vector<int>& parent2, int crossoverIndex);
void OutputGenerations(std::vector<std::vector<int>> newPopulation, std::ofstream& output, int i);