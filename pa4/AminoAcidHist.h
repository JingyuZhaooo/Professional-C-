#pragma once
#include <string>
#include "FASTAFile.h"
#include <vector>
struct AminoAcid
{
	std::string name;
	int count;
	double percentage;
};

class AminoAcidHist
{
public:
	AminoAcidHist();
	AminoAcidHist(std::string title, std::string sequence);
	void Translate();
	void Draw();
	std::vector<AminoAcid> GetAminoAcids() const { return mAminoAcids; };
private:
	int mTotalCount;
	std::string mTitle;
	std::string mSequence;
	std::vector<AminoAcid> mAminoAcids;
	char mStateMachine[24][4];
};