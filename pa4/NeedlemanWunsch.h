#pragma once
#include "FASTAFile.h"
#include <vector>

class NeedlemanWunsch
{
	enum Direction : char
	{
		AboveLeft,
		Left,
		Above
	};
public:
	NeedlemanWunsch();
	NeedlemanWunsch(FASTAFile fastaFileA, FASTAFile fastaFileB);
	void Run();
	void Output();
private:
	short MatchScore;
	short MisMatchScore;
	short GapScore;
	FASTAFile mFASTAFileA;
	FASTAFile mFASTAFileB;
	std::vector<std::vector<short>> mCells;
	std::vector<std::vector<Direction>> mPointers;
};