#pragma once
// You must place this macro inside the class definition of your derived Singleton
#define DECLARE_SINGLETON(SingletonClass) friend class Singleton<SingletonClass>;
#include "Singleton.h"
#include "Machine.h"
#include <vector>
#include <string>

class World
{
	
	//DECLARE_SINGLETON(World);
public:
	World();
	void AddZombie(std::shared_ptr<MachineState>& zombie);
	void AddSurvivor(std::shared_ptr<MachineState>& human);
	//void LoadZombieFile(std::string& fileName);
	//void LoadSurvivorFile(std::string& fileName);
	void SetMonth(int month) { mMonth = month; };
	int CheckTile(int x, int y);	// Wall is 1, zombie is 2, survivor is 3, empty is 4
	std::shared_ptr<MachineState> GetTarget(int x, int y);
	void RemoveDead(int x, int y);
	std::vector<std::shared_ptr<MachineState>> const GetZombies() { return mZombies; };
	std::vector<std::shared_ptr<MachineState>> const GetSurvivors() { return mSurvivors; };
	std::string GetZombieFileName() { return mZombieFileName; }
	std::string GetSurvivorFileName() { return mSurvivorFileName; }
	int const GetMonth() { return mMonth; };
	void SetZombieFileName(std::string fileName);
	void SetSurvivorFileName(std::string fileName);
	void Reset();
private:
	std::vector<std::shared_ptr<MachineState>> mZombies;
	std::vector<std::shared_ptr<MachineState>> mSurvivors;
	std::string mZombieFileName;
	std::string mSurvivorFileName;
	int mMonth;
};
