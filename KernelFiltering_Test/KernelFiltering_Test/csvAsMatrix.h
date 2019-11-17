#pragma once


#include "pch.h"

// Boost matrix
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/range/algorithm.hpp>

using namespace boost::numeric::ublas;

template <class T>
matrix<T> loadMatrixFromCSV(char * filepath, char delimiter = ';');

template <class T>
int saveMatrixAsCSV(matrix<T> m, char * filepath, char delimiter = ';', char * endline = "\n");