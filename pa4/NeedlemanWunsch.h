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
	void Output(std::string path);
private:
	short MatchScore;
	short MisMatchScore;
	short GapScore;
	FASTAFile mFASTAFileA;
	FASTAFile mFASTAFileB;
	std::vector<std::vector<short>> mCells;
	std::vector<std::vector<Direction>> mPointers;
	int mFinalScore;
	std::string resultA;
	std::string resultB;
};