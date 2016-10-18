#include "AminoAcidHist.h"

AminoAcidHist::AminoAcidHist(std::string title, std::string sequence)
{
	mHeight = 30;
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

}

void AminoAcidHist::Draw()
{
}
