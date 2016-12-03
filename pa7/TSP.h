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

std::vector<Location> ParseLocation(std::string& filename);
std::vector<std::vector<int>> GenerateInitRandPop(std::mt19937& random, int popsize, int numOfLoc);
std::vector<int> ShuffleNumbers(std::mt19937& rand, int numOfLoc);
void outputInitPop(std::vector<std::vector<int>>& population, std::ofstream& output);
std::vector<std::pair<int, double>> ComputeFitness(const std::vector<std::vector<int>>& population, const std::vector<Location>& locations);