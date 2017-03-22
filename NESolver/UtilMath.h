
#ifndef UTIL_MATH_H
#define UTIL_MATH_H

#include "UtilDef.h"

// Fills the matrix with random generated numbers in range from a to b
inline void FillRnd(REAL a, REAL b, MAT *mat)
{
	REAL d = b - a;
	for (UINT16 i = 0; i < mat->rows(); ++i)
	{
		for (UINT16 j = 0; j < mat->cols(); ++j)
		{
			(*mat)(i, j) = ((REAL)rand() / RAND_MAX)*d + a;
		}
	}
}

inline MAT CopyMatrixColumns(const MAT &mat, UINT32 n)
{
	MAT strechedMat = mat;
	strechedMat.conservativeResize(strechedMat.rows(), n);
	for (UINT32 i = 1; i < n; ++i)
	{
		strechedMat.col(i) = strechedMat.col(0);
	}
	return strechedMat;
}

// c - number of classes
inline MAT EncodeOneHot(const MAT &mat, UINT32 c)
{
	MAT encodedMat = mat;
	encodedMat.conservativeResize(c, encodedMat.cols());
	for (UINT16 i = 0; i < encodedMat.rows(); ++i)
	{
		for (UINT16 j = 0; j < encodedMat.cols(); ++j)
		{
			encodedMat(i, j) = (mat(j) == (REAL)(i)) ? (REAL)1.0 : (REAL)0.0;
		}
	}
	return encodedMat;
}

inline REAL ReLU(REAL x)
{
	if (x < (REAL)0.0)
		return (REAL)0.0;
	else
		return x;
}

inline REAL Sigmoid(REAL x)
{
	return (REAL)1.0 / ((REAL)1.0 + EXP(-x));
}

inline REAL DSigmoid(REAL sigmoid) // used for sigmoid derivation
{
	return sigmoid*((REAL)1.0 - sigmoid);
}

inline REAL Step(REAL x)
{
	if (x <= (REAL)0.0)
		return (REAL)0.0;
	else
		return (REAL)1.0;
}

inline void ApplyFunctionElementwise(MAT *mat, REAL(*funcPt)(REAL))
{
	for (UINT16 i = 0; i < mat->rows(); ++i)
	{
		for (UINT16 j = 0; j < mat->cols(); ++j)
		{
			(*mat)(i, j) = funcPt((*mat)(i, j));
		}
	}
}

inline void Softmax(MAT *mat)
{
	// apply exponential function
	ApplyFunctionElementwise(mat, EXP);
	// sum over rows
	MAT matSum = *mat;
	for (UINT16 i = 1; i < matSum.rows(); ++i)
	{
		matSum.row(0) += matSum.row(i);
	}
	// copy the sum over rows
	for (UINT16 i = 1; i < matSum.rows(); ++i)
	{
		matSum.row(i) = matSum.row(0);
	}
	// divide by sum elementwise
	for (UINT16 i = 0; i < mat->rows(); ++i)
	{
		for (UINT16 j = 0; j < mat->cols(); ++j)
		{
			(*mat)(i, j) = (*mat)(i, j) / matSum(i, j);
		}
	}
}

inline MAT Sum(const MAT &mat, UINT16 axis)
{
	assert(axis == 0 || axis == 1);
	if (axis == 1) // sum columns
	{
		MAT matSum = MAT::Zero(mat.rows(), 1);
		for (UINT16 i = 0; i < mat.cols(); ++i)
		{
			matSum.col(0) += mat.col(i);
		}
		return matSum;
	}
	else if (axis == 0) // sum rows
	{
		MAT matSum = MAT::Zero(1, mat.cols());
		for (UINT16 i = 0; i < mat.rows(); ++i)
		{
			matSum.row(0) += mat.row(i);
		}
		return matSum;
	}
	throw;
}

#endif
