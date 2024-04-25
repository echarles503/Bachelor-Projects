#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <string>
#include "pixel.h"

const int COLORMAX{255};

class Image
{

private:
	// Private data
	unsigned int h = 0;
	unsigned int w = 0;
	Pixel *buffer = nullptr;

public:
	// Constructor
	Image(const unsigned int _width, const unsigned int _height, const Pixel &_pixel=Pixel());
	~Image();

	// Public functions
	unsigned int getHeight() const { return h; }
	unsigned int getWidth() const {return w; }
	Pixel getPixel(const unsigned int _width, const unsigned int _height);
	void setPixel(const unsigned int _width, const unsigned int _height, const Pixel &_pixel=Pixel());
	void fillRectangle(unsigned int minRow,
					   unsigned int maxRow,
					   unsigned int minCol,
					   unsigned int maxCol,
					   const Pixel &_pixel);
	void generatePNM(std::string fname);
};	

#endif