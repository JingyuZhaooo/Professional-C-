#include "AminoAcidHist.h"

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
		{ 'Y', 'Y', 'Q', 'Q'},			// 7	TA*			using 'Q' to indicate STOP and GOTO 0
		{ 'C', 'C', 'Q', 'W'},			// 8	TG*
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
	mHeight = 30;
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
	for (unsigned i = 0; i < mSequence.length(); i++)
	{
		if (mSequence[i] == 'T')
		{

		}
		else if (mSequence[i] == 'C')
		{

		}
		else if (mSequence[i] == 'A')
		{

		}
		else if (mSequence[i] == 'G')
		{

		}
	}
}

void AminoAcidHist::Draw()
{
}
