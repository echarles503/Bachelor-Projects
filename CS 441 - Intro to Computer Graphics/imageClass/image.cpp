#include "image.h"


Image::Image(const unsigned int _width, const unsigned int _height, const Pixel &_pixel)
{
	w = _width;
	h = _height;
	buffer = nullptr;	
	buffer = new Pixel[w * h];

	int dim = w * h;	// dimension of image
	for (int i = 0; i < dim; i++)
	{
		buffer[i] = _pixel;
	}
}

Image::~Image()
{
	if (buffer != nullptr)
		delete [] buffer;

	w = 0;
	h = 0;
}

Pixel Image::getPixel(const unsigned int _width, const unsigned int _height)
{
	return buffer[_width + w * _height];
}

void Image::setPixel(const unsigned int _width, const unsigned int _height, const Pixel &_pixel)
{
	buffer[_width + w * _height] = _pixel;
}

void Image::fillRectangle(unsigned int minRow,
					      unsigned int maxRow,
					      unsigned int minCol,
					      unsigned int maxCol,
					      const Pixel &_pixel)
{

	if (minCol < 0 || minRow < 0 || maxCol > h || maxRow > w)
	{
		std::cerr << "Error -> Image::fillRectangle() - out of bounds\n";
		return;
	}

	for (int r = minRow; r < maxRow; r++)
	{
		for (int c = minCol; c < maxCol; c++)
		{
			setPixel(r, c, _pixel);
		}
	}

}

void Image::generatePNM(std::string fname)
{
	std::fstream file;
	file.open(fname, std::ios::out);

	file << "P6" << std::endl;
	file << std::to_string(w) << " " << std::to_string(h) << std::endl;
	file << std::to_string(COLORMAX) << std::endl;

	int dim = w * h;
	for (int i = 0; i < dim; i++)
	{
		file << buffer[i].getRed();
		file << buffer[i].getGreen();
		file << buffer[i].getBlue();
	}
	file.close();
}