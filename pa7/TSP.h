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
void OutputInitPop(std::vector<std::vector<int>>& population, std::ofstream& output);
std::vector<std::pair<int, double>> ComputeFitness(std::vector<std::vector<int>>& population, std::vector<Location>& locations);
std::pair<int, double> CalcEachFitScore(std::vector<int>& populationNums, int& i, std::vector<Location>& locations);
std::vector<std::pair<double, double>> GetLongLats(std::vector<int> &indices, std::vector<Location> &locations);
double Haversine(std::pair<double, double>& start, std::pair<double, double>& stop);