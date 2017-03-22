
#ifndef CHROMOSOME_PRODUCER_H
#define CHROMOSOME_PRODUCER_H

#include "Chromosome.h"
#include <random>

namespace NESolver
{
	//*******************************************************************************
	//							A base chromosome producer
	//*******************************************************************************
	template<class GeneType> 
	class ChromosomeProducer
	{
		friend class GeneticAlgorithm;

	public:
		ChromosomeProducer(INTEGER rndGenSeed) : mGen(rndGenSeed) {};
		virtual ~ChromosomeProducer() = 0 {};

	protected:
		std::default_random_engine mGen; // random engine generator
	};

	//*******************************************************************************
	//	Chromosome producer type designed as a neural network topology definition.
	//*******************************************************************************
	class NNTChromosomeProducer : public ChromosomeProducer<NUM>
	{
		friend class GeneticAlgorithm;

	public:
		NNTChromosomeProducer(INTEGER rndGenSeed, INTEGER layerMinWidth, INTEGER layerMaxWidth, INTEGER maxHiddenLayers);
		~NNTChromosomeProducer();

		void GeneratePopulation(std::vector<NNTChromosome> *out, INTEGER N);
		void GenerateChromosome(NNTChromosome *out);
		void Crossover(const NNTChromosome &c1, const NNTChromosome &c2, NNTChromosome *out);
		void Mutate(REAL mutProbability, NNTChromosome *out);
		// Changes chromosome size by adding or removing one genome.
		void ChangeChromosomeSize(NNTChromosome *out);

	private:
		bool BernoulliTest(REAL p);

	private:
		std::uniform_int_distribution<UINT16> mRndInteger;
		std::uniform_real_distribution<REAL> mRndReal;
		// Minimal width of a hidden layer
		INTEGER mLayerMinWidth;
		// Maximal width of a hidden layer
		INTEGER mLayerMaxWidth;
		// Maximal number of hidden layers
		INTEGER mMaxHiddenLayers;
	};
}

#endif
