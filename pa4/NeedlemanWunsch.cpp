#include "NeedlemanWunsch.h"

NeedlemanWunsch::NeedlemanWunsch()
{
}

NeedlemanWunsch::NeedlemanWunsch(FASTAFile fastaFileA, FASTAFile fastaFileB)
{
	MatchScore = 1;
	MisMatchScore = -1;
	GapScore = -1;
	mFASTAFileA = fastaFileA;
	mFASTAFileB = fastaFileB;
}

void NeedlemanWunsch::Run()
{
	std::string sequenceA = mFASTAFileA.GetSequence();
	std::string sequenceB = mFASTAFileB.GetSequence();
	size_t sequenceALength = sequenceA.length();
	size_t sequenceBLength = sequenceB.length();
	
	// Implementation Part 1
	mCells.resize(sequenceBLength + 1);							// the number of rows is the sequence B length + 1,
	for (auto& i : mCells)										// the number of columns is the sequence A length + 1
	{
		i.resize(sequenceALength + 1);
	}
	mPointers.resize(sequenceBLength + 1);							// the number of rows is the sequence B length + 1,
	for (auto& i : mPointers)										// the number of columns is the sequence A length + 1
	{
		i.resize(sequenceALength + 1);
	}
	// Implementation Part 2
	// Initialize the grids; The top left corner should be 0
	mCells[0][0] = 0;
	for (unsigned i = 1; i <= sequenceBLength; i++)
	{
		mCells[i][0] = i * GapScore;
		mPointers[i][0] = Above;				// cells on the first column (except cell[0][0]) all points to the Above
	}
	for (unsigned i = 1; i <= sequenceALength; i++)
	{
		mCells[0][i] = i * GapScore;
		mPointers[0][i] = Left;			// cells on the first row (except cell[0][0]) all points to the left
	}
	// Implementation Part 3
	for (unsigned i = 1; i <= sequenceBLength; i++)
	{
		for (unsigned j = 1; j <= sequenceALength; j++)
		{
			short scoreSelected;
			short score1;
			if (sequenceB[i - 1] == sequenceA[j - 1])		// if there is a match
			{
				score1 = mCells[i - 1][j - 1] + MatchScore;	// socre of neighbor cell to the above left + match score
			}
			else									// if there is a mismatch
			{
				score1 = mCells[i - 1][j - 1] + MisMatchScore;	// socre of neighbor cell to the above left + mismatch score
			}
			short score2 = mCells[i][j - 1] + GapScore;	// socre of neighbor cell to the left + gap score
			short score3 = mCells[i - 1][j] + GapScore; // socre of neighbor cell above + gap score
			scoreSelected = score1;
			mPointers[i][j] = AboveLeft;
			if (scoreSelected < score2)
			{
				scoreSelected = score2;
				mPointers[i][j] = Left;
			}
			if (scoreSelected < score3)
			{
				scoreSelected = score3;
				mPointers[i][j] = Above;
			}
			mCells[i][j] = scoreSelected;
		}
	}



	// testing
	for (unsigned i = 1; i <= sequenceBLength; i++)
	{
		for (unsigned j = 1; j <= sequenceALength; j++)
		{
			std::cout << mCells[i][j] << std::endl;
		}
	}
	
}

void NeedlemanWunsch::Output()
{
	
}
