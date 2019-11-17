#include "pch.h"
#include "pngAs8bitMatrix.h"


std::array<matrix<uint8_t>, 4> readPNGAsMatrix(const char* filename)
{
	const uint8_t channelCount = 4;

	std::vector<unsigned char> rawImage;
	unsigned width, height;
	unsigned error = lodepng::decode(rawImage, width, height, filename);

	std::array<matrix<uint8_t>, 4> image;
	for (int i = 0; i < channelCount; i++)
		image[i] = matrix<uint8_t>(width, height);

	if (error)
		return image;  // Retruning black image on error

	int count = 0;
	for (int r = 0; r < height; r++)
		for (int c = 0; c < width; c++)
			for (int n = 0; n < channelCount; n++)
			{
				image[n](c, r) = (uint8_t)rawImage.at(count);
				count++;
			}
	return image;
}


matrix<uint8_t>readPNGRedAsMatrix(const char* filename)
{
	std::array<matrix<uint8_t>, 4> img = readPNGAsMatrix(filename);
	return img[0];
}

unsigned writeMatrixAsPNG(const char* filename, std::array<matrix<uint8_t>, 4>* image)
{
	const uint8_t channelCount = 4;
	int width = (image)->at(0).size1();
	int height = (image)->at(0).size2();

	std::vector<unsigned char> pngBeforeEncoding;
	for (int r = 0; r < height; r++)
		for (int c = 0; c < width; c++)
			for (int n = 0; n < channelCount; n++)
				pngBeforeEncoding.push_back((unsigned char)(image)->at(n)(c, r));

	std::vector<unsigned char> pngAfterEncoding;
	unsigned error = lodepng::encode(pngAfterEncoding, pngBeforeEncoding, width, height);

	if (!error)
		lodepng::save_file(pngAfterEncoding, filename);

	return error;
}

unsigned writePNGRedAsMatrix(const char* filename, matrix<uint8_t>* image)
{
	const uint8_t channelCount = 4;
	const uint8_t nonTransperent = 255;

	int width = (image)->size1();
	int height = (image)->size2();

	std::array<matrix<uint8_t>, 4> pngImage;
	for (int i = 0; i < channelCount - 1; i++)
		pngImage[i] = *image;

	pngImage[channelCount - 1] = matrix<uint8_t>(width, height);
	for (int r = 0; r < height; r++)
		for (int c = 0; c < width; c++)
			pngImage[channelCount - 1](c, r) = nonTransperent;

	return writeMatrixAsPNG(filename, &pngImage);
}