
#include "ChromosomeProducer.h"

using namespace std;
using namespace NESolver;

NNTChromosomeProducer::NNTChromosomeProducer(INTEGER rndGenSeed, INTEGER layerMinWidth, INTEGER layerMaxWidth, INTEGER maxHiddenLayers)
	: ChromosomeProducer(rndGenSeed),
	mLayerMinWidth(layerMinWidth),
	mLayerMaxWidth(layerMaxWidth),
	mMaxHiddenLayers(maxHiddenLayers)
{
}

NNTChromosomeProducer::~NNTChromosomeProducer()
{
}

void NNTChromosomeProducer::GeneratePopulation(vector<NNTChromosome>* out, INTEGER N)
{
	out->clear();
	out->resize(N);
	for (INTEGER i = 0; i < N; ++i)
	{
		GenerateChromosome(&(*out)[i]);
	}
}

void NNTChromosomeProducer::GenerateChromosome(NNTChromosome *out)
{
	UINT16 numOfHiddenLayers = mRndInteger(mGen) % (mMaxHiddenLayers) + 1;
	out->mGenomesNumber = numOfHiddenLayers + 2;
	PDELARRAY(out->mGenomes);
	// Additional two parameters are for alpha (learning rate) and lambda (regularization coeffiecient)
	out->mGenomes = new NUM[out->mGenomesNumber];
	out->mGenomes[0] = (mRndReal(mGen) / mRndReal.max()) * 0.1f;	// alpha
	out->mGenomes[1] = (mRndReal(mGen) / mRndReal.max()) * 0.01f;	// lambda
	INTEGER d = mLayerMaxWidth - mLayerMinWidth + 1;
	for (UINT16 i = 2; i < out->mGenomesNumber; ++i)
	{
		NUM number = (NUM)(mRndInteger(mGen) % d + mLayerMinWidth);
		out->mGenomes[i] = number;
	}
}

void NNTChromosomeProducer::Crossover(const NNTChromosome &c1, const NNTChromosome &c2, NNTChromosome *out)
{
	out->Clear();
	const NNTChromosome &cMax = (c1.mGenomesNumber >= c2.mGenomesNumber) ? c1 : c2; // bigger parent
	const NNTChromosome &cMin = (c1.mGenomesNumber <= c2.mGenomesNumber) ? c1 : c2; // smaller parent
	UINT16 numMax = cMax.mGenomesNumber; // bigger parent size
	UINT16 numMin = cMin.mGenomesNumber; // smaller parent size
	UINT16 diff = numMax - numMin + 1;
	UINT16 numEx = (UINT16)(mRndInteger(mGen) % diff);		// number of chromosomes from the bigger parent
	UINT16 numChild = numEx + numMin;						// number of total chromosomes in the child
	out->mGenomes = new NUM[numChild];
	out->mGenomesNumber = numChild;
	out->mError = REAL_MAX;

	bool takeFromFirst = true;
	REAL p = 0.5; // crossover point probability
				 // use uniform crossover for part of the cromosome that is contained in both parents
	for (UINT16 i = 0; i < numMin; ++i)
	{
		takeFromFirst = ((mRndReal(mGen) / mRndReal.max()) < p) ? !takeFromFirst : takeFromFirst;
		out->mGenomes[i] = (takeFromFirst) ? c1.mGenomes[i] : c2.mGenomes[i];
	}
	// copy part of the cromosome that is contained only in the bigger parent
	for (UINT16 i = numMin; i < numChild; ++i)
	{
		out->mGenomes[i] = cMax.mGenomes[i];
	}
}

void NNTChromosomeProducer::Mutate(REAL mutProbability, NNTChromosome *out)
{
	// Gaussian mutation for alpha and lambda.
	normal_distribution<REAL> dist(0.0f, 0.01f);
	out->mGenomes[0] += BernoulliTest(mutProbability) ? dist(mGen) : 0.0 ;	// alpha
	out->mGenomes[1] += BernoulliTest(mutProbability) ? dist(mGen) : 0.0;	// lambda
	out->mGenomes[0] = max(0.0f, (REAL)out->mGenomes[0]);
	out->mGenomes[1] = max(0.0f, (REAL)out->mGenomes[1]);
	
	// Uniform mutation for the rest of the genes.
	for (UINT16 i = 2; i < out->mGenomesNumber; ++i)
	{
		INTEGER number = (mRndInteger(mGen) % 2) * 2 - 1; // -1 or 1 with 50% chance for each
		if (BernoulliTest(mutProbability))
			out->mGenomes[i] += number;
	}
	out->mError = REAL_MAX;
}

void NNTChromosomeProducer::ChangeChromosomeSize(NNTChromosome *out)
{
	NNTChromosome temp = *out;
	INTEGER addNum = (mRndInteger(mGen) % 2) * 2 - 1; // -1 or 1 with 50% chance for each
	out->mGenomesNumber = temp.mGenomesNumber + addNum;
	if (out->mGenomesNumber < 3)
	{
		out->mGenomesNumber = 3; // alpha, lambda and just one hidden layer is a bare minimum
		return;
	}
	else if (out->mGenomesNumber > mMaxHiddenLayers + 2)
	{
		out->mGenomesNumber = mMaxHiddenLayers + 2; // upper boundary must not be exceeded
		return;
	}
	PDELARRAY(out->mGenomes);
	out->mError = REAL_MAX;
	out->mGenomes = new NUM[out->mGenomesNumber];
	if (addNum > 0) // adding a genome
	{
		UINT16 insertIndex = (mRndInteger(mGen) % (temp.mGenomesNumber - 2)) + 2;
		for (UINT16 i = 0; i < out->mGenomesNumber; ++i)
		{
			if (i < insertIndex)
			{
				out->mGenomes[i] = temp.mGenomes[i];
			}
			else if (i == insertIndex)
			{
				NUM number = (insertIndex == 2) ? temp.mGenomes[2] : temp.mGenomes[i - 1];
				out->mGenomes[i] = number;
			}
			else // if (i > insertIndex)
			{
				out->mGenomes[i] = temp.mGenomes[i-1];
			}
		}
	}
	else // removing a genome
	{
		UINT16 removeIndex = (mRndInteger(mGen) % (temp.mGenomesNumber - 2)) + 2;
		for (UINT16 i = 0; i < out->mGenomesNumber; ++i)
		{
			if (i < removeIndex)
			{
				out->mGenomes[i] = temp.mGenomes[i];
			}
			else // if (i >= removeIndex)
			{
				// in case of deletion, just skip the neighbouring genome
				out->mGenomes[i] = temp.mGenomes[i + 1];
			}
		}
	}
}

bool NNTChromosomeProducer::BernoulliTest(REAL p)
{
	return ((mRndReal(mGen) / mRndReal.max()) < p);
}