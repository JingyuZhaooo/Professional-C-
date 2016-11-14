#include "node.h"
#include <sstream>

void NBlock::AddStatement(NStatement* statement)
{
	mStatements.push_back(statement);
}

void NBlock::CodeGen(CodeContext& context) const
{
	// TODO: Loop through statements in list and code gen them
}

NNumeric::NNumeric(std::string& value)
{
	mvalue = std::atoi(value.c_str());
}

NRotate::NRotate(NNumeric* dir)
	: mDir(dir)
{
}

void NRotate::CodeGen(CodeContext& context) const
{
	if (mDir->mvalue == 0)
	{
		context.mOps.push_back("rotate,0");
	}
	else if (mDir->mvalue == 1)
	{
		context.mOps.push_back("rotate,1");
	}
}

void NForward::CodeGen(CodeContext & context) const
{
	
}

void NIs_Zombie::CodeGen(CodeContext & context) const
{
}

void NIs_Human::CodeGen(CodeContext & context) const
{
}

void NIs_Passable::CodeGen(CodeContext & context) const
{
}

void NIs_random::CodeGen(CodeContext & context) const
{
}

void NAttack::CodeGen(CodeContext & context) const
{
}

void NRanged_Attack::CodeGen(CodeContext & context) const
{
}
