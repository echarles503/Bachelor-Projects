#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <stdint.h>
#include <math.h>

using namespace std;

const uint8_t COLORMAX{255};

enum Plane {X, Y};
/*
typedef struct
{
   double         X[3];
   double         Y[3];
   unsigned char color[3];
} Triangle;


typedef struct
{
   int numTriangles;
   Triangle *triangles;
} TriangleList;
*/

double C441(double f)
{
    return ceil(f-0.00001);
}


double F441(double f)
{
    return floor(f+0.00001);
}

class Pixel
{
private:
	// Private data
	uint8_t r, g, b;

public:
	// Constructors
	Pixel()
	{
		r = 0;
		g = 0;
		b = 0;
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

class Triangle
{
private:
	double X[3];
	double Y[3];
	Pixel color;

public:
	// Constructors
	Triangle() : X{0,0,0}, Y{0,0,0} 
	{

	}
	
	Triangle(double *_x, double *_y, const Pixel &_color=Pixel())
	{
		for (int i = 0; i < 3; i++)
		{
			X[i] = _x[i];
			Y[i] = _y[i];
		}
		color = _color;
	}

	~Triangle() = default;

	// Public fxns

	double getXY(Plane xy, int idx)
	{
		if (Plane::X)
		{
			return X[idx];
		}
		else if (Plane::Y)
		{
			return Y[idx];
		}
		else
		{
			// better exception handling
			std::cerr << "Triangle::getXY() -> Not a valid plane" << std::endl;
			return -1;
		}
	}

	void setXY(Plane xy, int idx, double val)
	{
		if (Plane::X)
		{
			X[idx] = val;
		}
		else if (Plane::Y)
		{
			Y[idx] = val;
		}
		else
		{
			// better exception handling 
			std::cerr << "Triangle::setXY() -> Not a valid plane" << std::endl;
		}
	}

	void setVertex(int idx, double _x, double _y)
	{
		X[idx] = _x;
		Y[idx] = _y;
	}

	Pixel getColor() { return color; }

	void setColor(Pixel &_color)
	{
		color = _color;
	} 

	

	void printVertices()
	{
		for (int i = 0; i < 3; i++)
		{
			(i != 2) ? printf("(%lf, %lf), ", X[i], Y[i]) : printf("(%lf, %lf)\n", X[i], Y[i]);
		}
	}

	void swapVertices(int a, int b)
	{
		double tmpX = X[a];
		double tmpY = Y[a];

		X[a] = X[b];
		Y[a] = Y[b];
		X[b] = tmpX;
		Y[b] = tmpY;
	}

	void sortVertices()
	{
		// Sort Planes such that XY[3] = {top, bot-middle, bot-right}

		if (Y[1] > Y[0]) 		// xy_1 is top
		{
			swapVertices(0, 1);	
		}
		if (Y[2] > Y[0])		// xy_2 is top
		{
			swapVertices(0, 2);
		}
		if (X[1] > X[2])		// xy_1 is bot-right
			swapVertices(1, 2);
	}

	double slope(int xy1, int xy2)
	{
		return (Y[xy2] - Y[xy1]) / (X[xy2] - X[xy1]);
	}

	int scanline(double m, int y)
	{
		if (isnan(m) || isinf(m))
			return X[0];

		double b = Y[0] - (m * X[0]);

		return (int)C441((y - b) / m);
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

	void rasterizeTriUp(Triangle &t)
	{

		t.sortVertices();

		int minRow = (int)C441(t.getXY(Plane::Y, 2));
		int maxRow = (int)F441(t.getXY(Plane::Y, 0));

		// Determine left hand slope
	
		for (int i = minRow; i <= maxRow; i++)
		{	
			double lSlope = t.slope(0, 1);
			double rSlope = t.slope(0, 2);

			int minCol = t.scanline(lSlope, i);
			int maxCol = t.scanline(rSlope, i);
			
			//printf("%d : %d -> %d\n", i, minCol, maxCol);
			for (int j = minCol; j <= maxCol; j++)
			{	
				// Invert y axis (height-i-1)
				if ((h-i-1 >= 0) && (h-i-1 <= h-1) && (j <= w-1) && (j >= 0))
					setPixel(j, h-i-1, t.getColor());
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


vector<Triangle> GetTriangles(void)
{
  	vector<Triangle> tl;

  	Pixel colors[6] = { Pixel{255, 128, 0  },  Pixel{255, 0,   127}, Pixel{0,   204, 204}, 
                        Pixel{76,  153, 0  },  Pixel{255, 204, 204}, Pixel{204, 204, 0  } };

	for (int i = 0 ; i < 100 ; i++)
	{
		Triangle tmp;

		int idxI = i%10;
		int posI = idxI*100;

		int idxJ = i/10;
		int posJ = idxJ*100;

		int firstPt = (i%3);

		tmp.setVertex(firstPt, posI, posJ+10*(idxJ+1));
		tmp.setVertex((firstPt+1)%3, posI+105, posJ);
		tmp.setVertex((firstPt+2)%3, posI+i, posJ);
		tmp.setColor(colors[i%6]);

		if (i == 50)
			tmp.setXY(Plane::X, firstPt, -10);

		if (i == 95)
			tmp.setXY(Plane::Y, firstPt, 1050);
		
		tl.push_back(tmp);
	}

	return tl;
}



int main()
{	

	Image img{1000, 1000};

	vector<Triangle> tl = GetTriangles();

	for (Triangle t : tl)
		img.rasterizeTriUp(t);

	img.generatePNM("proj1B_out.pnm");
}
