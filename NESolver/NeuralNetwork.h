
#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "UtilMath.h"
#include <vector>

namespace NESolver
{
	class NeuralNetwork
	{
	public:
		NeuralNetwork(UINT16 inputs, UINT16 outputs, std::vector<UINT16> hiddenTopology);
		~NeuralNetwork();

		void Train(const MAT &X, const MAT &Y_true, UINT32 iter, REAL alpha, REAL lambda);
		void Classify(const MAT &X, MAT *Y_result);
		REAL CalculateLoss(const MAT &Y_true) const;

	private:
		void ForwardPass(const MAT &X);
		void BackwardPass(const MAT &Y_true);
		void UpdateParameters(REAL alpha, REAL lambda, UINT32 N);

	private:
		std::vector<MAT> mWeights;
		std::vector<MAT> mBiases;

		// cache values
		std::vector<MAT> mHn;
		std::vector<MAT> mSn;
		std::vector<MAT> mWGrads;
		std::vector<MAT> mBGrads;

		const UINT16 mLayersCount;
	};
}

#endif
