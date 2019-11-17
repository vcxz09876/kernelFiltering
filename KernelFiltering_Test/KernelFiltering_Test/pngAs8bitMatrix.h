#pragma once

#include "pch.h"

#include <array>
#include "lodepng.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

using namespace boost::numeric::ublas;

std::array<matrix<uint8_t>, 4> readPNGAsMatrix(const char* filename);
matrix<uint8_t>readPNGRedAsMatrix(const char* filename);

unsigned writeMatrixAsPNG(const char* filename, std::array<matrix<uint8_t>, 4>* image);
unsigned writePNGRedAsMatrix(const char* filename, matrix<uint8_t>* image);