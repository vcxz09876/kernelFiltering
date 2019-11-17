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


template <class T>
matrix<T> loadMatrixFromCSV(char * filepath, char delimiter = ';')
{
	rapidcsv::Document doc(filepath, rapidcsv::LabelParams(-1, -1), rapidcsv::SeparatorParams(delimiter));
	int w = doc.GetColumnCount();
	int h = doc.GetRowCount();

	matrix<T>m(w, h);

	for (int i = 0; i < w; ++i)
	{
		for (int j = 0; j < h; ++j)
		{
			m(i, j) = doc.GetCell<T>(i, j);
		}
	}
	return m;
}


template <class T>
int saveMatrixAsCSV(matrix<T> m, char * filepath, char delimiter = ';', char * endline = "\n")
{
	int w = m.size1();
	int h = m.size2();
	string csv_string = string("");
	string delimiter_string = string(1, delimiter);

	for (int r = 0; r < h; r++)
	{
		for (int c = 0; c < w; ++c)
		{
			string value = to_string((double)m(c, r));
			csv_string.append(value);
			if (w > c + 1)
				csv_string.append(delimiter_string);
		}
		if (h > r + 1)
			csv_string.append(endline);
	}
	std::ofstream outfile(filepath);
	outfile << csv_string;
	outfile.close();
	return 0;
}
