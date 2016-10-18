#pragma once
#include <string>
#include "FASTAFile.h"
#include <vector>

class AminoAcidHist
{
public:
	struct AminoAcid
	{
		std::string name;
		int count;
		double percentage;
	};

	AminoAcidHist(std::string title, std::string sequence);
	void Translate();
	void Draw();

private:
	int mHeight;
	std::string mTitle;
	std::string mSequence;
	std::vector<AminoAcid> mAminoAcids;
};