// KernelFiltering_Test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include "LinkingTest.h"

#include <array>
#include "lodepng.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
using namespace boost::numeric::ublas;

#include "pngAs8bitMatrix.h"

#include "kernelGaussianLowPass.h"

//void runLinkingTest() {
//	// Initialize a Fibonacci relation sequence.
//// Write out the sequence values until overflow.	fibonacci_init(1, 1);
//
//	do {
//		std::cout << fibonacci_index() << ": "
//			<< fibonacci_current() << std::endl;
//	} while (fibonacci_next());
//	// Report count of values written before overflow.
//	std::cout << fibonacci_index() + 1 <<
//		" Fibonacci sequence values fit in an " <<
//		"unsigned 64-bit integer." << std::endl;
//}


int main()
{
	matrix<uint8_t> inputImage = readPNGRedAsMatrix("..\\..\\test_data\\subp_147_5-157_5.png");

	matrix<uint8_t> outputImage1 = matrix<uint8_t>(inputImage.size1(), inputImage.size2());
	applyLowPassGaussFilterToMatrix<uint8_t, double>(&outputImage1, &inputImage, 1, 1, 0.8);
	auto r1 = writePNGRedAsMatrix("..\\..\\test_data\\_lp_1_1_08_subp_147_5-157_5.png", &outputImage1);

	matrix<uint8_t> outputImage2 = matrix<uint8_t>(inputImage.size1(), inputImage.size2());
	apply2dLowPassGaussFilterToMatrix<uint8_t, double>(&outputImage2, &inputImage, 1, 3, 0.3, 1.0);
	auto r2 = writePNGRedAsMatrix("..\\..\\test_data\\_2dlp_1_3_03_10_subp_147_5-157_5.png", &outputImage2);

	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.uin
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
