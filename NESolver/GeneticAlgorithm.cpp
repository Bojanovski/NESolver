#include "GeneticAlgorithm.h"

using namespace std;
using namespace NESolver;

GeneticAlgorithm::GeneticAlgorithm(NNTChromosomeProducer &cp)
	: mCP(cp) // copy the generator (seed) from the chromosome producer
{
}

GeneticAlgorithm::~GeneticAlgorithm()
{

}

void NESolver::GeneticAlgorithm::Initialize()
{
	mCP.GeneratePopulation(&mPopulation, 10);
}

void GeneticAlgorithm::SelectCrossAndMutate()
{
	// Select
	K_TournamentSelection(3); // 3 tournament selection
}

void GeneticAlgorithm::K_TournamentSelection(INTEGER k)
{
	assert(k <= (INTEGER)mPopulation.size());
	vector<UINT16> out;
	GenerateSelectionIndices(&out, mPopulation.size(), k);

	// order them up ...
}

void GeneticAlgorithm::GenerateSelectionIndices(vector<UINT16> *out, INTEGER populationSize, INTEGER selectionSize)
{
	// based on https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
	vector<UINT16> populationIndices;
	populationIndices.resize(populationSize);
	for (INTEGER i = 0; i < populationSize; ++i)
	{
		populationIndices[i] = i;
	}
	out->clear();
	out->resize(selectionSize);
	for (INTEGER i = 0; i < selectionSize; ++i)
	{
		INTEGER index = (mCP.mRndInteger(mCP.mGen) % (populationIndices.size() - i)); // rand_between(0, vec.size() - i - 1);
		(*out)[i] = populationIndices[index];
		swap(populationIndices[index], populationIndices[populationIndices.size() - i - 1]);
	}
}

