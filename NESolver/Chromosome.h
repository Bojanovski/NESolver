
#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include "UtilMath.h"
#include <vector>

namespace NESolver
{
	//*******************************************************************************
	//							A base chromosome type
	//*******************************************************************************
	template<class GeneType> 
	class Chromosome
	{
		template<class GeneType>
		friend class ChromosomeProducer;
		friend class GeneticAlgorithm;

	public:
		Chromosome();
		Chromosome(const Chromosome &c);
		virtual ~Chromosome();

		Chromosome &operator=(Chromosome &c);
		void Clear();
		bool operator==(const Chromosome &right);
		REAL GetError() const;

		static void CopyTo(const Chromosome &src, Chromosome *dest);

	protected:
		// Pointer to a list of all the genomes for this chromosome.
		GeneType *mGenomes;
		// Number of the genomes in this chromosome.
		UINT16 mGenomesNumber;
		// Calculated error value.
		REAL mError;

	};

	// Method definitions
	template<class GeneType> 
	inline Chromosome<GeneType>::Chromosome()
		: mGenomes(0),
		mGenomesNumber(0),
		mError(REAL_MAX)
	{
	}

	template<class GeneType> 
	inline Chromosome<GeneType>::Chromosome(const Chromosome &c)
	{
		CopyTo(c, this);
	}

	template<class GeneType>
	inline Chromosome<GeneType>::~Chromosome()
	{
		PDELARRAY(mGenomes)
	}

	template<class GeneType>
	inline Chromosome<GeneType> &Chromosome<GeneType>::operator=(Chromosome &c)
	{
		CopyTo(c, this);
		return *this;
	}

	template<class GeneType> 
	inline void Chromosome<GeneType>::Clear()
	{
		PDELARRAY(mGenomes);
		mGenomesNumber = 0;
		mError = REAL_MAX;
	}

	template<class GeneType>
	inline bool Chromosome<GeneType>::operator==(const Chromosome & right)
	{
		return false;
	}

	template<class GeneType>
	inline REAL Chromosome<GeneType>::GetError() const
	{
		return mError;
	}

	template<class GeneType>
	inline void Chromosome<GeneType>::CopyTo(const Chromosome & src, Chromosome * dest)
	{
		dest->mGenomesNumber = src.mGenomesNumber;
		PDELARRAY(dest->mGenomes);
		dest->mGenomes = new GeneType[dest->mGenomesNumber];
		for (int i = 0; i < dest->mGenomesNumber; ++i)
		{
			dest->mGenomes[i] = src.mGenomes[i];
		}
		dest->mError = src.mError;
	}

	//*******************************************************************************
	//		Chromosome type designed as a neural network topology definition.
	//*******************************************************************************
	class NNTChromosome : public Chromosome<NUM>
	{
		friend class NNTChromosomeProducer;

	public:
		NNTChromosome();
		~NNTChromosome();

		void CalculateError(const MAT &X, const MAT &YOH);
	};
}

#endif
