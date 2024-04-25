#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <stdint.h>

using namespace std;

const uint8_t COLORMAX{255};

class Pixel
{
private:
	// Private data
	uint8_t r, g, b;

public:
	// Constructors
	Pixel()
	{
		r = 255;
		g = 255;
		b = 255;
	}

	Pixel(uint8_t _red, uint8_t _green, uint8_t _blue)
	{
		r = _red;
		g = _green;
		b = _blue;
	}

	Pixel(int _red, int _green, int _blue)
	{
		r = static_cast<uint8_t>(_red);
		g = static_cast<uint8_t>(_green);
		b = static_cast<uint8_t>(_blue);
	}

	~Pixel() = default;

	// Public functions
	uint8_t getRed() const { return r; }
    uint8_t getGreen() const { return g; }
    uint8_t getBlue() const { return b; }

    void setRed(uint8_t _red) { r = _red; }
    void setGreen(uint8_t _green) { g = _green; }
    void setBlue(uint8_t _blue) { b = _blue; }

	void setRGB(uint8_t _red, uint8_t _green, uint8_t _blue)
	{
		r = _red;
	    g = _green;
	    b = _blue;
	}
};

class Image
{

private:
	// Private data
	int h = 0;
	int w = 0;
	Pixel *buffer = nullptr;

public:
	// Constructor
	Image(const int _width,
		  const int _height,
		  const Pixel &_pixel=Pixel())
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

	// Destructor
	~Image()
	{
		if (buffer != nullptr)
			delete [] buffer;

		w = 0;
		h = 0;
	}
	
	// Public functions
	int getHeight() const { return h; }
	int getWidth() const {return w; }

	Pixel getPixel(const int _width, const int _height)
	{
		return buffer[_width + w * _height];
	}

	void setPixel(const int _width,
				  const int _height,
				  const Pixel &_pixel=Pixel())
	{
		buffer[_width + w * _height] = _pixel;
	}

	void fillRectangle(int minRow,
					   int maxRow,
					   int minCol,
					   int maxCol,
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
	void generatePNM(std::string fname)
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
};


struct Palette
{
	Pixel black{0,0,0};
	Pixel gray{128,128,128};
	Pixel white{255,255,255};
	Pixel red{255,0,0};
	Pixel green{0,255,0};
	Pixel blue{0,0,255};
	Pixel pink{255,0,255};
	Pixel cyan{0,255,255};
	Pixel yellow{255,255,0};
};


int main()
{	
	Palette rgb;
	Image img{300, 300};

	img.fillRectangle(0, 100, 0, 100, rgb.black);
	img.fillRectangle(100, 200, 0, 100, rgb.gray);
	img.fillRectangle(200, 300, 0, 100, rgb.white);
	img.fillRectangle(0, 100, 100, 200, rgb.red);
	img.fillRectangle(100, 200, 100, 200, rgb.green);
	img.fillRectangle(200, 300, 100, 200, rgb.blue);
	img.fillRectangle(0, 100, 200, 300, rgb.pink);
	img.fillRectangle(100, 200, 200, 300, rgb.cyan);
	img.fillRectangle(200, 300, 200, 300, rgb.yellow);

	img.generatePNM("out.pnm");
}