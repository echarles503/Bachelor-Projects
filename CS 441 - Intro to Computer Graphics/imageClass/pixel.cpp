#include "pixel.h"

Pixel::Pixel()
{
	r = 255;
	g = 255;
	b = 255;
}

Pixel::Pixel(unsigned char _red, unsigned char _green, unsigned char _blue)
{
	r = _red;
	g = _green;
	b = _blue;
}

Pixel::Pixel(int _red, int _green, int _blue)
{
	r = static_cast<unsigned char>(_red);
	g = static_cast<unsigned char>(_green);
	b = static_cast<unsigned char>(_blue);
}

void Pixel::setRed(unsigned char _red)
{
    r = _red;
}

void Pixel::setGreen(unsigned char _green)
{
    g = _green;
}

void Pixel::setBlue(unsigned char _blue)
{
    b = _blue;
}

void Pixel::setRGB(unsigned char _red, unsigned char _green, unsigned char _blue)
{
    r = _red;
    g = _green;
    b = _blue;
}