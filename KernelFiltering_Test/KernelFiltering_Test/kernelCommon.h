#include "pch.h"

// Slow, but simple csv parsing lib
#include "include/rapidcsv/rapidcsv.h"

// Boost matrix
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/range/algorithm.hpp>

using namespace boost::numeric::ublas;
using namespace std;


////------------------------------COMMON OPERATIONS-----------------------------//

template <class T>
pair<T, T> getMatrixMinMax(matrix<T> * x)  // Could be changed to boost vector minMax
{
	pair<T, T> minMax;
	minMax.first = (x)->operator()(0, 0); // Min
	minMax.second = (x)->operator()(0, 0); // Max

	for (int m = 0; m < (x)->size1(); ++m)
		for (int n = 0; n < (x)->size2(); n++)
		{
			T value = (x)->operator()(m, n);
			if (value < minMax.first)
				minMax.first = value;
			if (value > minMax.second)
				minMax.second = value;

		}
	return minMax;
}

template <class T>
void normlizeMatrix(matrix<T> * x) {

	pair<T, T> minMax = getMatrixMinMax<T>(x);
	T range = minMax.second - minMax.first;
	for (int m = 0; m < (x)->size1(); ++m)
		for (int n = 0; n < (x)->size2(); n++)
		{
			T old_value = (x)->operator()(m, n);
			(x)->operator()(m, n) = (old_value - minMax.first) / range;
		}
}

template <class T>
void unsignMatrix(matrix<T> * x) {

	pair<T, T> minMax = getMatrixMinMax<T>(x);
	if (minMax.first >= 0) // If content not have no negative values -> do nothing
	{
		return;
	}
	else // else, add make all values geater, than zero by substracting min value
	{
		for (int m = 0; m < (x)->size1(); ++m)
			for (int n = 0; n < (x)->size2(); n++)
			{
				T old_value = (x)->operator()(m, n);
				(x)->operator()(m, n) = old_value - minMax.first;
			}
	}
}

template <class T>
T getClosest(matrix<T> * x, int m, int n)
{
	int mLimit = (x)->size1();
	int nLimit = (x)->size2();

	if (m < 0)
		m = 0;
	if (n < 0)
		n = 0;
	if (m >= mLimit)
		m = mLimit - 1;
	if (n >= nLimit)
		n = nLimit - 1;

	return (x)->operator()(m, n);
};

template <class T>
matrix_range<matrix<T>> getFrame(matrix<T> * x, int m, int n, int radiusM, int radiusN)
{
	matrix_range<matrix<T>> frame(*x, range(m - radiusM, m + radiusM), range(n - radiusN, n + radiusN));
	return frame;
};