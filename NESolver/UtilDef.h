
#ifndef UTIL_DEF_H
#define UTIL_DEF_H

#include "..\Eigen\Dense"

namespace NESolver
{
	// Math
#define INT16		short
#define INT32		int
#define INT64		long long
#define UINT16		unsigned short
#define UINT32		unsigned int
#define UINT64		unsigned long long
#define BYTE		unsigned char

	using Eigen::Matrix2f;
	using Eigen::Matrix2d;
	using Eigen::VectorXf;
	using Eigen::VectorXd;

	template<class RealType, class IntegerType>
	union Num
	{
		RealType r;		// Real interpetration of the number
		IntegerType i;	// Integer interpetration of the number

		// Constructors
		Num() {}

		// Copy constructors
		explicit Num(const double &r)				{ this->r = (RealType)r; }
		explicit Num(const INT64 &w)				{ this->i = (IntegerType)w; }
		explicit Num(const float &r)				{ this->r = (RealType)r; }
		explicit Num(const INT32 &w)				{ this->i = (IntegerType)w; }
		explicit Num(const INT16 &w)				{ this->i = (IntegerType)w; }

		// Conversion functions
		explicit operator double()					{ return (double)r; }
		explicit operator INT64()					{ return (INT64)i; }
		explicit operator float()					{ return (float)r; }
		explicit operator INT32()					{ return (INT32)i; }
		explicit operator INT16()					{ return (INT16)i; }

		// Assignment operators
		RealType	operator=(const double &r)		{ return this->r = (RealType)r; }
		IntegerType	operator=(const INT64 &w)		{ return this->i = (IntegerType)w; }
		RealType	operator=(const float &r)		{ return this->r = (RealType)r; }
		IntegerType	operator=(const INT32 &w)		{ return this->i = (IntegerType)w; }
		IntegerType	operator=(const INT16 &w)		{ return this->i = (IntegerType)w; }

		// Arithmetic operators
		// *
		RealType	operator*(const double &r)		{ return this->r * (RealType)r; }
		IntegerType	operator*(const INT64 &w)		{ return this->i * (IntegerType)w; }
		RealType	operator*(const float &r)		{ return this->r * (RealType)r; }
		IntegerType	operator*(const INT32 &w)		{ return this->i * (IntegerType)w; }
		IntegerType	operator*(const INT16 &w)		{ return this->i * (IntegerType)w; }
		RealType	operator*=(const double &r)		{ return this->r *= (RealType)r; }
		IntegerType	operator*=(const INT64 &w)		{ return this->i *= (IntegerType)w; }
		RealType	operator*=(const float &r)		{ return this->r *= (RealType)r; }
		IntegerType	operator*=(const INT32 &w)		{ return this->i *= (IntegerType)w; }
		IntegerType	operator*=(const INT16 &w)		{ return this->i *= (IntegerType)w; }
		// +
		RealType	operator+(const double &r)		{ return this->r + (RealType)r; }
		IntegerType	operator+(const INT64 &w)		{ return this->i + (IntegerType)w; }
		RealType	operator+(const float &r)		{ return this->r + (RealType)r; }
		IntegerType	operator+(const INT32 &w)		{ return this->i + (IntegerType)w; }
		IntegerType	operator+(const INT16 &w)		{ return this->i + (IntegerType)w; }
		RealType	operator+=(const double &r)		{ return this->r += (RealType)r; }
		IntegerType	operator+=(const INT64 &w)		{ return this->i += (IntegerType)w; }
		RealType	operator+=(const float &r)		{ return this->r += (RealType)r; }
		IntegerType	operator+=(const INT32 &w)		{ return this->i += (IntegerType)w; }
		IntegerType	operator+=(const INT16 &w)		{ return this->i += (IntegerType)w; }
		// /
		RealType	operator/(const double &r)		{ return this->r / (RealType)r; }
		IntegerType	operator/(const INT64 &w)		{ return this->i / (IntegerType)w; }
		RealType	operator/(const float &r)		{ return this->r / (RealType)r; }
		IntegerType	operator/(const INT32 &w)		{ return this->i / (IntegerType)w; }
		IntegerType	operator/(const INT16 &w)		{ return this->i / (IntegerType)w; }
		RealType	operator/=(const double &r)		{ return this->r /= (RealType)r; }
		IntegerType	operator/=(const INT64 &w)		{ return this->i /= (IntegerType)w; }
		RealType	operator/=(const float &r)		{ return this->r /= (RealType)r; }
		IntegerType	operator/=(const INT32 &w)		{ return this->i /= (IntegerType)w; }
		IntegerType	operator/=(const INT16 &w)		{ return this->i /= (IntegerType)w; }
		// -
		RealType	operator-(const double &r)		{ return this->r - (RealType)r; }
		IntegerType	operator-(const INT64 &w)		{ return this->i - (IntegerType)w; }
		RealType	operator-(const float &r)		{ return this->r - (RealType)r; }
		IntegerType	operator-(const INT32 &w)		{ return this->i - (IntegerType)w; }
		IntegerType	operator-(const INT16 &w)		{ return this->i - (IntegerType)w; }
		RealType	operator-=(const double &r)		{ return this->r -= (RealType)r; }
		IntegerType	operator-=(const INT64 &w)		{ return this->i -= (IntegerType)w; }
		RealType	operator-=(const float &r)		{ return this->r -= (RealType)r; }
		IntegerType	operator-=(const INT32 &w)		{ return this->i -= (IntegerType)w; }
		IntegerType	operator-=(const INT16 &w)		{ return this->i -= (IntegerType)w; }
	};

#define NUM_32_BIT

#ifdef NUM_32_BIT
#define NUM Num<float, INT32>
#define REAL float
#define INTEGER INT32
#define REAL_MAX FLT_MAX
#define EXP expf
#define LOG logf
#define MAT Eigen::MatrixXf
#define VEC Eigen::VectorXf
#endif

#ifdef NUM_64_BIT
#define NUM Num<double, INT64>
#define REAL double
#define INTEGER INT64
#define REAL double
#define REAL_MAX DBL_MAX
#define EXP exp
#define LOG log
#define MAT Eigen::MatrixXd
#define VEC Eigen::VectorXd
#endif

	// Pointer handling
#define PDEL(pt) if (pt) {delete pt; pt = 0;}
#define PDELARRAY(pt) if (pt) {delete[] pt; pt = 0;}
}

#endif
