#include "pch.h"

#include <limits>

#include "kernelCommon.h"

// Boost matrix
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/math/constants/constants.hpp>

using namespace boost::numeric::ublas;
using namespace std;


template <class G>
void printMatrix(const matrix<G> &m)
{
	for (unsigned i = 0; i < m.size1(); ++i)
	{
		cout << "| ";
		for (unsigned j = 0; j < m.size2(); ++j)
		{
			cout << m(i, j) << " | ";
		}
		cout << "|" << endl;
	}
}


template <class T, class G>
G s2dGauss(T m, T n, G sigmaM, G sigmaN) {
	const G pi = boost::math::constants::pi<G>();
	G km = pow(m, 2.0) / (2 * pow(sigmaM, 2.0));
	G kn = pow(n, 2.0) / (2 * pow(sigmaN, 2.0));
	G result = exp(-1 * (km + kn)) / pi;  // TODO: 2*pi
	return result;
}

template <class G>
matrix<G> s2dGaussKernel(int radiusM, int radiusN, G sigmaM, G sigmaN)
{
	int actM = 1 + (radiusM * 2);
	int actN = 1 + (radiusN * 2);
	matrix<G> k(actM, actN);

	for (int m = 0; m < actM; m++) {
		for (int n = 0; n < actN; n++) {
			k(m, n) = s2dGauss<int, G>(m - radiusM, n - radiusN, sigmaM, sigmaN);
		}
	}
	return k;
}


template <class T, class G>
G sGauss(T m, T n, G sigma) {
	const G pi = boost::math::constants::pi<G>();
	G k = 1 / (2 * pi *  pow(sigma, 2.0));
	G e = exp(-1 * (pow(m, 2.0) + pow(n, 2.0)) / (2 * pow(sigma, 2.0)));
	G result = k * e;
	return result;
}


template <class G>
matrix<G> sGaussKernel(int radiusM, int radiusN, G sigma)
{
	int actM = 1 + (radiusM * 2);
	int actN = 1 + (radiusN * 2);
	matrix<G> k(actM, actN);

	for (int m = 0; m < actM; m++) {
		for (int n = 0; n < actN; n++) {
			k(m, n) = sGauss<int, G>(m - radiusM, n - radiusN, sigma); // TODO: Could be optimized by vector multiplication
		}
	}
	return k;
}


template <class T, class G>
T calculateGLP(matrix_range<matrix<T>> * x, matrix<G> * kernel)
{
	T result = 0;
	for (int m = 0; m < (x)->size1(); m++) {
		for (int n = 0; n < (x)->size2(); n++)
		{
			T dx = (x)->operator()(m, n);
			G dk = (kernel)->operator()(m, n);
			T r = dx * dk;
			result += r;
		}

	}
	return result;
}

template <class T, class G>
void applyLowPassGaussFilterToMatrix(
	matrix<T> * d, matrix<T> * x,
	uint32_t radiusM, uint32_t radiusN, G sigma
)
{
	matrix<G> kernel = sGaussKernel<G>(radiusM, radiusN, sigma);
	printMatrix<G>(kernel);

	int startM = radiusM;
	int endM = (x)->size1() - radiusM;
	int startN = radiusN;
	int endN = (x)->size2() - radiusN;
	for (int m = startM; m < endM; m++)
		for (int n = startN; n < endN; n++)
		{
			matrix_range<matrix<T>> frame = getFrame(x, m, n, radiusM, radiusN);
			T value = calculateGLP<T, G>(&frame, &kernel);
			(d)->operator()(m, n) = value;
		}
}


template <class T, class G>
void apply2dLowPassGaussFilterToMatrix(
	matrix<T> * d, matrix<T> * x,
	uint32_t radiusM, uint32_t radiusN, G sigmaM, G sigmaN
)
{
	matrix<G> kernel = s2dGaussKernel<G>(radiusM, radiusN, sigmaM, sigmaN);
	printMatrix<G>(kernel);

	int startM = radiusM;
	int endM = (x)->size1() - radiusM;
	int startN = radiusN;
	int endN = (x)->size2() - radiusN;
	for (int m = startM; m < endM; m++)
		for (int n = startN; n < endN; n++)
		{
			matrix_range<matrix<T>> frame = getFrame(x, m, n, radiusM, radiusN);
			T value = calculateGLP<T, G>(&frame, &kernel);
			(d)->operator()(m, n) = value;
		}
}