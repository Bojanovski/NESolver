
#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include "ChromosomeProducer.h"

namespace NESolver
{
	class GeneticAlgorithm
	{
	public:
		GeneticAlgorithm(NNTChromosomeProducer &cp);
		~GeneticAlgorithm();

		void Initialize();
		void SelectCrossAndMutate();

	private:
		void K_TournamentSelection(INTEGER k);
		void GenerateSelectionIndices(std::vector<UINT16> *out, INTEGER populationSize, INTEGER selectionSize);

	private:
		std::vector<NNTChromosome> mPopulation;
		NNTChromosomeProducer mCP;
	};
}

#endif
