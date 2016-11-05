#include "Op.h"
#include "Machine.h"
#include <iostream>
#include "World.h"

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	std::cout << state.mProgramCounter << ":" << mOpName << "," << mParam << std::endl;
}

void OpRotate::Execute(MachineState& state)
{
	//DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::RIGHT;
		}
		else
		{
			state.mFacing = MachineState::LEFT;
		}
		break;
	case (MachineState::RIGHT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::DOWN;
		}
		else
		{
			state.mFacing = MachineState::UP;
		}
		break;
	case (MachineState::DOWN) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::LEFT;
		}
		else
		{
			state.mFacing = MachineState::RIGHT;
		}
		break;
	default:
	case (MachineState::LEFT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::UP;
		}
		else
		{
			state.mFacing = MachineState::DOWN;
		}
		break;
	}

	/*std::cout << "Now facing ";
	switch (state.mFacing)
	{
	case (MachineState::UP):
		std::cout << "UP";
		break;
	case (MachineState::RIGHT):
		std::cout << "RIGHT";
		break;
	case (MachineState::DOWN):
		std::cout << "DOWN";
		break;
	case (MachineState::LEFT):
		std::cout << "LEFT";
		break;
	}
	std::cout << std::endl;*/
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
	state.mProgramCounter = mParam;
}

void OpAttack::Execute(MachineState & state)
{
	state.mProgramCounter += 1;
	state.mActionsTaken += 1;
	switch (state.mFacing)
	{
		case 0:
		{
			ExecuteHelper(0, -1, state);
			break;
		}
		case 1:
		{
			ExecuteHelper(1, 0, state);
			break;
		}
		case 2:
		{
			ExecuteHelper(0, 1, state);
			break;
		}
		case 3:
		{
			ExecuteHelper(-1, 0, state);
			break;
		}
	}
}

void OpAttack::ExecuteHelper(int i, int j, MachineState & state)
{
	std::shared_ptr<MachineState> target = state.GetWorld()->GetTarget(state.GetX() + i, state.GetY() + j);
	if (target != nullptr)
	{
		if (target->GetInfect() && !state.GetInfect())		//human attack zombie
		{
			state.GetWorld()->RemoveDead(state.GetX() + i, state.GetY() + j);	//zombie dies
		}
		else if (!target->GetInfect() && state.GetInfect())		// zombie attacks and infects human
		{
			state.GetWorld()->RemoveDead(state.GetX() + i, state.GetY() + j);
			//spawn new zombie
			std::shared_ptr<MachineState> zombie = std::make_shared<MachineState>();
			zombie->SetX(state.GetX() + i);
			zombie->SetY(state.GetY() + j);
			zombie->SetWorld(state.GetWorld());
			state.GetWorld()->AddZombie(zombie);
			state.GetZombieMachine()->BindState(*zombie);
			zombie->SetSurvivorMachine(state.GetSurvivorMachine());
			zombie->SetZombieMachine(state.GetZombieMachine());
		}
		else if (!target->GetInfect() && !state.GetInfect())	// human attacks human
		{
			state.GetWorld()->RemoveDead(state.GetX() + i, state.GetY() + j);
		}
	}
}

void OpRangedAttack::Execute(MachineState & state)
{
	state.mProgramCounter += 1;
	state.mActionsTaken += 1;
	switch (state.mFacing)
	{
	case 0:
	{
		ExecuteHelper(0, -2, state);
		break;
	}
	case 1:
	{
		ExecuteHelper(2, 0, state);
		break;
	}
	case 2:
	{
		ExecuteHelper(0, 2, state);
		break;
	}
	case 3:
	{
		ExecuteHelper(-2, 0, state);
		break;
	}
	}
}

void OpRangedAttack::ExecuteHelper(int i, int j, MachineState & state)
{
	std::shared_ptr<MachineState> target = state.GetWorld()->GetTarget(state.GetX() + i, state.GetY() + j);
	if (target != nullptr)
	{
		if (target->GetInfect() && !state.GetInfect())		//human attack zombie
		{
			state.GetWorld()->RemoveDead(state.GetX() + i, state.GetY() + j);	//zombie dies
		}
		else if (!target->GetInfect() && state.GetInfect())		// zombie attacks and infects human
		{
			state.GetWorld()->RemoveDead(state.GetX() + i, state.GetY() + j);
			//spawn new zombie
			std::shared_ptr<MachineState> zombie = std::make_shared<MachineState>();
			zombie->SetX(state.GetX() + i);
			zombie->SetY(state.GetY() + j);
			zombie->SetWorld(state.GetWorld());
			state.GetWorld()->AddZombie(zombie);
			state.GetZombieMachine()->BindState(*zombie);
			zombie->SetSurvivorMachine(state.GetSurvivorMachine());
			zombie->SetZombieMachine(state.GetZombieMachine());
		}
		else if (!target->GetInfect() && !state.GetInfect())	// human attacks human
		{
			state.GetWorld()->RemoveDead(state.GetX() + i, state.GetY() + j);
		}
	}
}

void OpForward::Execute(MachineState & state)
{
	state.mProgramCounter += 1;
	state.mActionsTaken += 1;
	switch (state.mFacing)
	{
	case 0:
		if (state.GetY() > 0 && state.GetWorld()->CheckTile(state.GetX(), state.GetY() - 1) == 4)
		{
			state.SetY(state.GetY() - 1);
			break;
		}
		break;
	case 1:
		if (state.GetX() < 20 && state.GetWorld()->CheckTile(state.GetX() + 1, state.GetY()) == 4)
		{
			state.SetX(state.GetX() + 1);
		}
		break;
	case 2:
		if (state.GetY() < 20 && state.GetWorld()->CheckTile(state.GetX(), state.GetY() + 1) == 4)
		{
			state.SetY(state.GetY() + 1);
		}
		break;
	case 3:
		if (state.GetX() > 0 && state.GetWorld()->CheckTile(state.GetX() - 1, state.GetY()) == 4)
		{
			state.SetX(state.GetX() - 1);
		}
		break;
	}
}

void OpEndTurn::Execute(MachineState & state)
{
	state.mProgramCounter += 1;
	state.mActionsTaken = state.GetActionsPerTurn();
}

void OpTestHuman::Execute(MachineState & state)
{
	state.mProgramCounter += 1;
	switch (state.mFacing)
	{
	case 0:
		if (state.GetWorld()->CheckTile(state.GetX(), state.GetY() - mParam) == 3)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 1:
		if (state.GetWorld()->CheckTile(state.GetX() + mParam, state.GetY()) == 3)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 2:
		if (state.GetWorld()->CheckTile(state.GetX(), state.GetY() + mParam) == 3)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 3:
		if (state.GetWorld()->CheckTile(state.GetX() - mParam, state.GetY()) == 3)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	}
}

void OpTestWall::Execute(MachineState & state)
{
	state.mProgramCounter += 1;
	switch (state.mFacing)
	{
	case 0:
		if (state.GetWorld()->CheckTile(state.GetX(), state.GetY() - 1) == 1)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 1:
		if (state.GetWorld()->CheckTile(state.GetX() + 1, state.GetY()) == 1)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 2:
		if (state.GetWorld()->CheckTile(state.GetX(), state.GetY() + 1) == 1)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 3:
		if (state.GetWorld()->CheckTile(state.GetX() - 1, state.GetY()) == 1)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	}
}

void OpTestZombie::Execute(MachineState & state)
{
	state.mProgramCounter += 1;
	switch (state.mFacing)
	{
	case 0:
		if (state.GetWorld()->CheckTile(state.GetX(), state.GetY() - mParam) == 2)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 1:
		if (state.GetWorld()->CheckTile(state.GetX() + mParam, state.GetY()) == 2)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 2:
		if (state.GetWorld()->CheckTile(state.GetX(), state.GetY() + mParam) == 2)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 3:
		if (state.GetWorld()->CheckTile(state.GetX() - mParam, state.GetY()) == 2)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	}
}

void OpTestRandom::Execute(MachineState & state)
{
	state.mProgramCounter++;
	double r = ((double)rand() / (RAND_MAX));
	if (r < 0.5)
	{
		state.mTest = true;
	}
	else
	{
		state.mTest = false;
	}
}

void OpTestPassable::Execute(MachineState & state)
{
	state.mProgramCounter += 1;
	switch (state.mFacing)
	{
	case 0:
		if (state.GetWorld()->CheckTile(state.GetX(), state.GetY() - 1) == 4)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 1:
		if (state.GetWorld()->CheckTile(state.GetX() + 1, state.GetY()) == 4)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 2:
		if (state.GetWorld()->CheckTile(state.GetX(), state.GetY() + 1) == 4)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	case 3:
		if (state.GetWorld()->CheckTile(state.GetX() - 1, state.GetY()) == 4)
		{
			state.mTest = true;
		}
		else
		{
			state.mTest = false;
		}
		break;
	}
}

void OpJe::Execute(MachineState & state)
{
	if (state.mTest)
	{
		state.mProgramCounter = mParam;
		state.mTest = false;
	}
	else
	{
		state.mProgramCounter += 1;
	}
}

void OpJne::Execute(MachineState & state)
{
	if (!state.mTest)
	{
		state.mProgramCounter = mParam;
	}
	else
	{
		state.mProgramCounter += 1;
		state.mTest = false;
	}
}
