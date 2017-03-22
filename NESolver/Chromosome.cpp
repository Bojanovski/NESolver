#include "Chromosome.h"
#include "NeuralNetwork.h"
#include <ctime>

using namespace std;
using namespace NESolver;

// Method definitions
NNTChromosome::NNTChromosome()
{
}

NNTChromosome::~NNTChromosome()
{
}

void NNTChromosome::CalculateError(const MAT &X, const MAT &YOH)
{
	clock_t begin = clock(); // start the stopwatch
	vector<UINT16> hT(mGenomesNumber - 2);
	for (UINT16 i = 2; i < mGenomesNumber; ++i)
	{
		hT[i-2] = (INTEGER)mGenomes[i];
	}
	NeuralNetwork nn(2, 2, hT);
	nn.Train(X, YOH, 1000, (REAL)mGenomes[0], (REAL)mGenomes[1]);
	MAT Y_results;
	nn.Classify(X, &Y_results);
	clock_t end = clock(); // end the stopwatch

	REAL elapsedSecs = REAL(end - begin) / CLOCKS_PER_SEC;
	mError = nn.CalculateLoss(YOH) + elapsedSecs;
}