#include "AminoAcidHist.h"
#include <sstream>

AminoAcidHist::AminoAcidHist()
{
}

AminoAcidHist::AminoAcidHist(std::string title, std::string sequence)
	: mStateMachine{
	//  { T, C, A, G }
										//State
		{ 0, 0, 1, 0 },					// 0
		{ 2, 0, 1, 0 },					// 1
		{ 0, 0, 1, 'M' },				// 2
		{ 4, 9, 14, 19},				// 3
		{ 5, 6, 7, 8},					// 4	T**
		{ 'F', 'F', 'L', 'L'},			// 5	TT*
		{ 'S', 'S', 'S', 'S'},			// 6	TC*
		{ 'Y', 'Y', 'Z', 'Z'},			// 7	TA*			using 'Z' to indicate STOP and GOTO 0
		{ 'C', 'C', 'Z', 'W'},			// 8	TG*
		{ 10, 11, 12, 13},				// 9		C**
		{ 'L', 'L', 'L', 'L'},			// 10		CT*
		{ 'P', 'P', 'P', 'P'},			// 11		CC*
		{ 'H', 'H', 'Q', 'Q'},			// 12		CA*
		{ 'R', 'R', 'R', 'R'},			// 13		CG*
		{ 15, 16, 17, 18},				// 14	A**
		{ 'I', 'I', 'I', 'M'},			// 15	AT*
		{ 'T', 'T', 'T', 'T'},			// 16	AC*
		{ 'N', 'N', 'K', 'K'},			// 17	AA*
		{ 'S', 'S', 'R', 'R'},			// 18	AG*
		{ 20, 21, 22, 23},				// 19		G**
		{ 'V', 'V', 'V', 'V'},			// 20		GT*
		{ 'A', 'A', 'A', 'A'},			// 21		GC*
		{ 'D', 'D', 'E', 'E'},			// 22		GA*
		{ 'G', 'G', 'G', 'G'},			// 23		GG*
}
{
	mTotalCount = 0;
	mTitle = title;
	mSequence = sequence;
	AminoAcid Alanine{"Alanine", 0, 0.0 }
		, Cysteine{"Cysteine", 0, 0.0}
		, Aspartic{"Aspartic acid", 0, 0.0}
		, Glutamic{"Glutamic acid", 0, 0.0}
		, Phenylalanine{"Phenylalanine", 0, 0.0}
		, Glycine{"Glycine", 0, 0.0}
		, Histidine{"Histidine", 0, 0.0}
		, Isoleucine{"Isoleucine", 0, 0.0}
		, Lysine{"Lysine", 0, 0.0}
		, Leucine{"Leucine", 0, 0.0}
		, Methionine{"Methionine", 0, 0.0}
		, Asparagine{"Asparagine", 0, 0.0}
		, Proline{"Proline", 0, 0.0}
		, Glutamine{"Glutamine", 0, 0.0}
		, Arginine{"Arginine", 0, 0.0}
		, Serine{"Serine", 0, 0.0}
		, Threonine{"Threonine", 0, 0.0}
		, Valine{"Valine", 0, 0.0}
		, Tryptophan{"Tryptophan", 0, 0.0}
		, Tyrosine{"Tyrosine", 0, 0.0}
	;
	mAminoAcids = { Alanine, Cysteine, Aspartic, Glutamic, Phenylalanine, Glycine, Histidine, Isoleucine, Lysine, Leucine, Methionine
		, Asparagine, Proline, Glutamine, Arginine, Serine, Threonine, Valine, Tryptophan, Tyrosine };

}

void AminoAcidHist::Translate()
{
	char state = 0;
	for (unsigned i = 0; i < mSequence.length(); i++)
	{
		if (mSequence[i] == 'T') // If we haven't started yet, getting a T means accessing state machine [0]
		{		
			state = mStateMachine[state][0];	// update the state
		}
		else if (mSequence[i] == 'C')
		{
			state = mStateMachine[state][1];
		}
		else if (mSequence[i] == 'A')
		{
			state = mStateMachine[state][2];
		}
		else if (mSequence[i] == 'G')
		{
			state = mStateMachine[state][3];
		}

		if (state >= 'A')		// if we are not getting an int (getting a char)
		{
			switch (state)			// add number to the corresponding amino acid
			{
				case 'A':
				{
					mAminoAcids[0].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'C':
				{
					mAminoAcids[1].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'D':
				{
					mAminoAcids[2].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'E':
				{
					mAminoAcids[3].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'F':
				{
					mAminoAcids[4].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'G':
				{
					mAminoAcids[5].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'H':
				{
					mAminoAcids[6].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'I':
				{
					mAminoAcids[7].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'K':
				{
					mAminoAcids[8].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'L':
				{
					mAminoAcids[9].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'M':
				{
					mAminoAcids[10].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'N':
				{
					mAminoAcids[11].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'P':
				{
					mAminoAcids[12].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'Q':
				{
					mAminoAcids[13].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'R':
				{
					mAminoAcids[14].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'S':
				{
					mAminoAcids[15].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'T':
				{
					mAminoAcids[16].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'V':
				{
					mAminoAcids[17].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'W':
				{
					mAminoAcids[18].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'Y':
				{
					mAminoAcids[19].count += 1;
					mTotalCount += 1;
					break;
				}
				case 'Z':				// if we get a Stop amino acid, set state back to 0
				{
					state = 0;
					break;
				}
			}
			if (state != 0)
			{
				state = 3;				// set the state back to 3
			}
		}
	}
	for (auto& i : mAminoAcids)		// calculate each amino acid's percentage
	{
		if (mTotalCount != 0)
		{
			i.percentage = static_cast<double>(i.count) * 100 / static_cast<double>(mTotalCount);
			i.percentage = round(i.percentage * 100) / 100;
		}
	}
	for (auto& i : mAminoAcids)		// put name + percentage + count together
	{
		std::ostringstream strs;
		strs << i.percentage;
		std::string str = strs.str();
		i.name += ": ";
		i.name += str;
		i.name += "% (" + std::to_string(i.count) + ")";
	}
}

