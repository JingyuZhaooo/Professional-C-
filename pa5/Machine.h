#pragma once
#include <wx/msgdlg.h> 
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "Op.h"
#include "Exceptions.h"
#include <fstream>
#include "World.h"
#include "Traits.h"

#ifndef INCLUDE_GUARD
#define INCLUDE_GUARD
class World;
// Defines state data accessible by the machine and ops
struct MachineState
{
	// Friend Machine so BindState works
	template <typename MachineTraits>
	friend class Machine;

	enum Facing { UP, RIGHT, DOWN, LEFT };
	MachineState()
		: mProgramCounter(1)
		, mActionsTaken(0)
		, mFacing(UP)
		, mTest(false)
	{ }

	~MachineState()
	{
	}

	// Active line number in behavior program
	int mProgramCounter;
	// Number of actions taken this turn
	int mActionsTaken;
	// Current facing of this character
	Facing mFacing;
	// Test flag for branches
	bool mTest;

	int GetActionsPerTurn() const noexcept { return mActionsPerTurn; }
	bool GetInfect() const noexcept { return mInfectOnAttack; }
	
	//getter and setters
	std::pair<int, int> GetTilePos() { return mTilePos; };
	int GetX() const { return mTilePos.first; };
	int GetY() const { return mTilePos.second; };
	std::shared_ptr<Machine<ZombieTraits>> GetZombieMachine() { return mZombieMachine; };
	std::shared_ptr<Machine<HumanTraits>> GetSurvivorMachine() { return mSurvivorMachine; };
	std::shared_ptr<World> GetWorld() { return mWorld; };

	void SetWorld(std::shared_ptr<World>& world) { mWorld = world; };
	void SetX(int x) { mTilePos.first = x; };
	void SetY(int y) { mTilePos.second = y; };
	void SetZombieMachine(std::shared_ptr<Machine<ZombieTraits>> machine) { mZombieMachine = machine; };
	void SetSurvivorMachine(std::shared_ptr<Machine<HumanTraits>> machine) { mSurvivorMachine = machine; };
	
private:
	// Data which is set by the traits
	int mActionsPerTurn;
	bool mInfectOnAttack;
	std::shared_ptr<World> mWorld = nullptr;
	std::pair<int, int> mTilePos = std::make_pair(0, 0); // starts at (0,0)
	std::shared_ptr<Machine<ZombieTraits>> mZombieMachine = nullptr;
	std::shared_ptr<Machine<HumanTraits>> mSurvivorMachine = nullptr;
};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
public:
	// Load in all the ops for this machine from the specified file
	void LoadMachine(const std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);

	// Destructor
	~Machine();

	void SetWorld(std::shared_ptr<World>& world) { mWorld = world; };
	void AddCharacter(std::shared_ptr<MachineState> &character) { mCharacters.push_back(character); }
	std::vector<std::shared_ptr<MachineState>> GetCharacters() const { return mCharacters; };
	void SetCharacters(std::vector<std::shared_ptr<MachineState>> characters) { mCharacters = characters; };
	std::shared_ptr<World> const GetWorld() { return mWorld; };
private:
	std::vector<std::shared_ptr<Op>> mOps;
	std::vector<std::shared_ptr<MachineState>> mCharacters;
	std::shared_ptr<World> mWorld = nullptr;
};

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const std::string& filename)
{
	/*
	// TEMP CODE: Add your parsing code here!
	mOps.clear();
	mOps.push_back(std::make_shared<OpRotate>(0));
	mOps.push_back(std::make_shared<OpRotate>(0));
	mOps.push_back(std::make_shared<OpRotate>(1));
	mOps.push_back(std::make_shared<OpGoto>(1));
	// END TEMP CODE
	*/
	
	try
	{
		std::ifstream file;
		file.open(filename);
		if (file.is_open())
		{
			std::string str;
			std::vector<std::string> ops;
			while (!file.eof())
			{
				std::getline(file, str);
				std::size_t commaIndex = str.find_first_of(",");
				if (commaIndex == std::string::npos)	// There is not a comma, so it belongs to attack, ranged_attack, forward, endturn, test_wall, test_random, test_passable
				{
					std::size_t semicolonIndex = str.find_first_of(";");
					if (semicolonIndex != std::string::npos) // if there is a semicolon, we don't need the comment part
					{
						std::string str2 = str.substr(0, semicolonIndex);
						for (unsigned i = 0; i < str2.length(); i++)
						{
							if (str2[i] == ' ' || str2[i] == '\n' || str2[i] == '\t') // get rid of any space or new line or tab
							{
								str2.erase(str2.begin() + i); // get rid of any space or new line or tab
							}
							ops.push_back(str2);
						}
					}
					else		// if there is neither a semicolon nor a comma
					{
						ops.push_back(str);
					}

				}
				else	// There is a comma, so belongs to Rotate, test-human, test-zombie, je, jne, or goto
				{
					std::string str2 = str.substr(commaIndex + 1); // get the rest of the string after ","
					std::size_t semicolonIndex = str2.find_first_of(";"); // find the index of ";"
					if (semicolonIndex != std::string::npos)	// if there is a semicolon, we don't need the comment part
					{
						std::string str3 = str2.substr(0, semicolonIndex); // get the parameter
						for (unsigned i = 0; i < str3.length(); i++)
						{
							if (str3[i] == ' ' || str3[i] == '\n' || str3[i] == '\t')
							{
								str3.erase(str3.begin() + i); // get rid of any space or new line or tab
							}
						}
						ops.push_back(str3);
					}
					else	// if there's a comma but not a semicolon, then str2 includes the parameter
					{
						for (unsigned i = 0; i < str2.length(); i++)
						{
							if (str2[i] == ' ' || str2[i] == '\n' || str2[i] == '\t')
							{
								str2.erase(str2.begin() + i); // get rid of any space or new line or tab
							}
						}
						ops.push_back(str2);
					}
				}
				if (ops.size() == 1) // The op is attack, ranged_attack, forward, endturn, test_wall, test_random, test_passable
				{
					std::string op = ops[0];
					if (op == "attack")
					{
						mOps.push_back(std::make_shared<OpAttack>());
						break;
					}
					else if (op == "ranged_attack")
					{
						mOps.push_back(std::make_shared<OpRangedAttack>());
						break;
					}
					else if (op == "forward")
					{
						mOps.push_back(std::make_shared<OpForward>());
						break;
					}
					else if (op == "endturn")
					{
						mOps.push_back(std::make_shared<OpEndTurn>());
						break;
					}
					else if (op == "test_wall")
					{
						mOps.push_back(std::make_shared<OpTestWall>());
						break;
					}
					else if (op == "test_random")
					{
						mOps.push_back(std::make_shared<OpTestRandom>());
						break;
					}
					else if (op == "test_passable")
					{
						mOps.push_back(std::make_shared<OpTestPassable>());
						break;
					}
				}
				else if (ops.size() == 2) // The op is rotate, test-human, test-zombie, je, jne, or goto
				{
					std::string op = ops[0];
					int param = static_cast<int>(ops[1].at(0));
					if (op == "rotate")
					{
						mOps.push_back(std::make_shared<OpRotate>(param));
						break;
					}
					else if (op == "test_human")
					{
						mOps.push_back(std::make_shared<OpTestHuman>(param));
						break;
					}
					else if (op == "test_zombie")
					{
						mOps.push_back(std::make_shared<OpTestZombie>(param));
						break;
					}
					else if (op == "je")
					{
						mOps.push_back(std::make_shared<OpJe>(param));
						break;
					}
					else if (op == "jne")
					{
						mOps.push_back(std::make_shared<OpJne>(param));
						break;
					}
					else if (op == "goto")
					{
						mOps.push_back(std::make_shared<OpGoto>(param));
						break;
					}
				}
				ops.clear();
			}
		}
		else
		{
			throw FileOpenError();
		}
	}
	catch(FileOpenError& e)
	{
		wxMessageBox(e.what(), "Error Opening File");
	}
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.mActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.mInfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	std::cout << "TAKING TURN" << std::endl;
	state.mActionsTaken = 0;
	while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		mOps.at(state.mProgramCounter - 1)->Execute(state);
	}
}

template <typename MachineTraits>
Machine<MachineTraits>::~Machine()
{
	mOps.clear();
}
#endif;