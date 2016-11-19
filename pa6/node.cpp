#include "node.h"
#include <sstream>

void NBlock::AddStatement(NStatement* statement)
{
	mStatements.push_back(statement);
}

void NBlock::CodeGen(CodeContext& context) const
{
	// TODO: Loop through statements in list and code gen them
	for (auto& i : mStatements)
	{
		i->CodeGen(context);
	}
	if (mbMainBlock)
	{
		context.mOps.push_back("goto,");
		context.mGotoMap.emplace(context.mOps.size() - 1, 1);
		
		int firstElement = context.mGotoMap.begin()->first;
		int endOfLoop = firstElement;
		while (context.mGotoMap.find(endOfLoop) != context.mGotoMap.end())
		{
			endOfLoop = context.mGotoMap.find(endOfLoop)->second;
		}
		for (auto& i : context.mGotoMap)
		{
			context.mOps[i.first] += std::to_string(endOfLoop);
		}
	}
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
	context.mOps.push_back("forward");
}

void NIs_Zombie::CodeGen(CodeContext & context) const
{
	context.mOps.push_back("test_zombie," + std::to_string(mNum->mvalue));
}

void NIs_Human::CodeGen(CodeContext & context) const
{
	context.mOps.push_back("test_human," + std::to_string(mNum->mvalue));
}

void NIs_Passable::CodeGen(CodeContext & context) const
{
	context.mOps.push_back("test_passable");
}

void NIs_random::CodeGen(CodeContext & context) const
{
	context.mOps.push_back("test_random");
}

void NAttack::CodeGen(CodeContext & context) const
{
	context.mOps.push_back("attack");
}

void NRanged_Attack::CodeGen(CodeContext & context) const
{
	context.mOps.push_back("ranged_attack");
}


NIfelse::NIfelse(NBoolean * boolean, NBlock * block1, NBlock * block2)
{
	mBool = boolean;
	mBlock1 = block1;
	mBlock2 = block2;
}

void NIfelse::CodeGen(CodeContext & context) const
{
	mBool->CodeGen(context);
	size_t indexJe = context.mOps.size();
	context.mOps.push_back("je,");
	mBlock2->CodeGen(context);
	size_t indexGoto = context.mOps.size();
	context.mOps.push_back("goto,");
	mBlock1->CodeGen(context);
	size_t JeNum = context.mOps.size();
	context.mOps[indexJe] += std::to_string(JeNum);
	//context.mOps[indexGoto] += std::to_string(JeNum + 1);
	context.mGotoMap.emplace(indexGoto, JeNum);
}
