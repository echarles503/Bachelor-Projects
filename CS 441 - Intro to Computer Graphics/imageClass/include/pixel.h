#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>
#include <iomanip>
#include <string>

class Pixel
{
private:
	// Private data
	unsigned char r, g, b;

public:
	// Constructors
	Pixel();
	Pixel(unsigned char r, unsigned char g, unsigned char b);
	Pixel(int r, int g, int b);
	~Pixel() = default;

	// Public functions
	unsigned char getRed() const { return r; }
    unsigned char getGreen() const { return g; }
    unsigned char getBlue() const { return b; }
    void setRed(unsigned char _red);
    void setGreen(unsigned char _green);
    void setBlue(unsigned char _blue);
	void setRGB(unsigned char _red, unsigned char _green, unsigned char _blue);
};

#endif