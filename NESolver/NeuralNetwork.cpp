#include "NeuralNetwork.h"

using namespace std;
using namespace NESolver;

NeuralNetwork::NeuralNetwork(UINT16 inputs, UINT16 outputs, vector<UINT16> hiddenTopology)
	: mLayersCount((UINT16)hiddenTopology.size() + 2)
{
	assert(inputs > 0);
	assert(outputs > 0);

	hiddenTopology.insert(hiddenTopology.begin(), inputs);
	hiddenTopology.insert(hiddenTopology.end(), outputs);

	for (UINT16 i = 0; i < mLayersCount - 1; ++i)
	{
		MAT W(hiddenTopology[i + 1], hiddenTopology[i]);
		MAT b(hiddenTopology[i + 1], 1);
		FillRnd(-0.9f, 0.9f, &W);
		FillRnd(-0.9f, 0.9f, &b);
		mWeights.push_back(W);
		mBiases.push_back(b);
	}
}

NeuralNetwork::~NeuralNetwork()
{

}
//#include <iostream>
void NeuralNetwork::Train(const MAT &X, const MAT &Y_true, UINT32 iter, REAL alpha, REAL lambda)
{
	UINT32 N = X.cols(); // number of input vectors
	while (iter--) 
	{
		ForwardPass(X);
		//cout << CalculateLoss(Y_true) << endl;
		BackwardPass(Y_true);
		UpdateParameters(alpha, lambda, N);
	};
}

void NeuralNetwork::Classify(const MAT & X, MAT *Y_result)
{
	ForwardPass(X);
	*Y_result = mHn[mLayersCount - 1];
}

void NeuralNetwork::ForwardPass(const MAT &X)
{
	UINT32 N = X.cols(); // number of input vectors
	mHn.clear();
	mSn.clear();
	mHn.push_back(X);
	MAT res = X;
	for (UINT16 i = 0; i < mLayersCount - 1; ++i)
	{
		MAT s = mWeights[i] * res;
		MAT stretchedBias = CopyMatrixColumns(mBiases[i], N);
		s += stretchedBias;
		MAT h = s;

		// transition function
		if (i == mLayersCount - 2) // last layer
		{
			Softmax(&h);
		}
		else // hidden layer
		{
			ApplyFunctionElementwise(&h, ReLU);
		}

		// append cache variables
		mHn.push_back(h);
		mSn.push_back(s);

		res = h;
	}
}

REAL NeuralNetwork::CalculateLoss(const MAT & Y_true) const
{
	MAT Y = mHn[mLayersCount - 1];
	ApplyFunctionElementwise(&Y, LOG);
	MAT negativeLogLikelyhood = -Y_true.cwiseProduct(Y);
	negativeLogLikelyhood = Sum(negativeLogLikelyhood, 0);
	negativeLogLikelyhood = Sum(negativeLogLikelyhood, 1);

	return negativeLogLikelyhood(0, 0);
}

void NeuralNetwork::BackwardPass(const MAT &Y_true)
{
	mWGrads.clear();
	mBGrads.clear();
	mWGrads.resize(mLayersCount - 1);
	mBGrads.resize(mLayersCount - 1);
	MAT Gh_prev;

	for (UINT16 i = mLayersCount - 2; i != (UINT16)-1; --i)
	{
		if (i == mLayersCount - 2) // last layer
		{
			MAT GSn = mHn[i + 1].transpose() - Y_true.transpose(); // N x C      OPTIMIZIRAAAATTTTT
			MAT GSnT = GSn.transpose();
			MAT Wn = GSnT * mHn[i].transpose(); // C x H
			MAT Bn = Sum(GSnT, 1); // C x 1

			mWGrads[i] = Wn;
			mBGrads[i] = Bn;
			Gh_prev = GSn * mWeights[i]; // N x H_prev
		}
		else // hidden layer
		{
			MAT GSi = Gh_prev; // N x H
			//ApplyFunctionElementwise(&GSi, DSigmoid);

			MAT si = mSn[i].transpose(); // N x H
			ApplyFunctionElementwise(&si, Step);
			GSi = GSi.cwiseProduct(si);
			MAT GSiT = GSi.transpose(); // H x N

			MAT Wi = GSiT * mHn[i].transpose(); // H x (H_prev or D)
			MAT Bi = Sum(GSiT, 1); // H x 1

			mWGrads[i] = Wi;
			mBGrads[i] = Bi;

			Gh_prev = GSi * mWeights[i]; // N x H_prev
		}
	}
}

void NeuralNetwork::UpdateParameters(REAL alpha, REAL lambda, UINT32 N)
{
	for (UINT16 i = 0; i < mLayersCount - 1; ++i)
	{
		mWeights[i] += -(alpha / N) * mWGrads[i];
		mBiases[i] += -(alpha / N) * mBGrads[i];
	}
}
