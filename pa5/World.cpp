#include "World.h"

World::World()
{
	mMonth = 0;
}

void World::AddZombie(std::shared_ptr<MachineState>& zombie)
{
	mZombies.push_back(zombie);
}

void World::AddSurvivor(std::shared_ptr<MachineState>& survivor)
{
	mSurvivors.push_back(survivor);
}

int World::CheckTile(int x, int y)
{
	
	if (x == -1 || y == -1 || x == 20 || y == 20)
	{
		return 1;
	}
	else
	{
		for (unsigned int i = 0; i < mZombies.size(); i++)
		{
			if (mZombies[i]->GetX() == x && mZombies[i]->GetY() == y)
			{
				return 2;
			}
		}
		for (unsigned int j = 0; j < mSurvivors.size(); j++)
		{
			if (mSurvivors[j]->GetX() == x && mSurvivors[j]->GetY() == y)
			{
				return 3;
			}
		}
		return 4;
	}
}

std::shared_ptr<MachineState> World::GetTarget(int x, int y)
{
	if (x == -1 || y == -1 || x == 20 || y == 20)
	{
		return nullptr;
	}
	else
	{
		for (unsigned int i = 0; i < mZombies.size(); i++)
		{
			if (mZombies[i]->GetX() == x && mZombies[i]->GetY() == y)
			{
				return mZombies[i];
			}
		}
		for (unsigned int j = 0; j < mSurvivors.size(); j++)
		{
			if (mSurvivors[j]->GetX() == x && mSurvivors[j]->GetY() == y)
			{
				return mSurvivors[j];
			}
		}
		return nullptr;
	}
}

void World::RemoveDead(int x, int y)
{
	for (unsigned int i = 0; i < mZombies.size(); i++)
	{
		if (mZombies[i]->GetX() == x && mZombies[i]->GetY() == y)
		{
			mZombies.erase(mZombies.begin() + i);
		}
	}
	for (unsigned int j = 0; j < mSurvivors.size(); j++)
	{
		if (mSurvivors[j]->GetX() == x && mSurvivors[j]->GetY() == y)
		{
			mSurvivors.erase(mSurvivors.begin() + j);
		}
	}
}

void World::SetZombieFileName(std::string fileName)
{
	int i = fileName.find_last_of("\\");
	std::string name = fileName.substr(i + 1);
	mZombieFileName = name;
}

void World::SetSurvivorFileName(std::string fileName)
{ 
	int i = fileName.find_last_of("\\");
	std::string name = fileName.substr(i + 1);
	mSurvivorFileName = name;
}

void World::Reset()
{
	mZombies.clear();
	mSurvivors.clear();
	mZombieFileName = "";
	mSurvivorFileName = "";
	mMonth = 0;
}
