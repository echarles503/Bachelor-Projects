#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <stdint.h>
#include <math.h>
#include <algorithm>
#include <unistd.h>

using namespace std;

const uint8_t COLORMAX{255};

enum Plane {X, Y};

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

	double *getArrayXY(Plane xy)
	{
		if (xy == Plane::X)
			return X;
		else if (xy == Plane::Y)
			return Y;
		else
		{
			std::cerr << "Triangle::getArrayXY() -> Not a valid plane" << std::endl;
			return nullptr;
		}
	}

	double getXY(Plane xy, int idx)
	{
		if (xy == Plane::X)
		{
			return X[idx];
		}
		else if (xy == Plane::Y)
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
		if (xy == Plane::X)
		{
			X[idx] = val;
		}
		else if (xy == Plane::Y)
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
		// Sort Y coordinates in increasing order

		if (Y[0] > Y[1]) 		
			swapVertices(0, 1);

		if (Y[0] > Y[2])		
			swapVertices(0, 2);

		if (Y[1] > Y[2])
			swapVertices(1, 2);

	}

	double slope(int xy1, int xy2)
	{
		return (X[xy2] - X[xy1]) / (Y[xy2] - Y[xy1]);
	}

	double scanline(int idx, int y, double m)
	{
		return X[idx] + (y - Y[idx]) * m;
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
		if ((h-1-_height >= 0) && (h-1-_height <= h-1) && (_width <= w-1) && (_width >= 0))
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

	void rasterize(Triangle &t)
	{
		// Sort vertices by y-coordinate
		t.sortVertices();

		// Calculate edges
		double *X = t.getArrayXY(Plane::X);
		double *Y = t.getArrayXY(Plane::Y);

		double m1 = t.slope(0, 1);
		double m2 = t.slope(0, 2);
		double m3 = t.slope(1, 2);

		int minRow = C441(Y[0]);
		int maxRow = F441(Y[2]);

		for (int y = minRow; y <= maxRow; y++)
		{
			
			// Check if scanline is below midpoint
			if (y < Y[1])
			{
				double sx1 = t.scanline(0, y, m1);
				double sx2 = t.scanline(0, y, m2);

				int minCol = (int)C441(std::min(sx1, sx2));
				int maxCol = (int)F441(std::max(sx1, sx2));

				for (int x = minCol; x <= maxCol; x++)
					setPixel(x, h-1-y, t.getColor());
			}
			else
			{
				double sx1 = t.scanline(1, y, m3);
				double sx2 = t.scanline(0, y, m2);

				int minCol = (int)C441(std::min(sx1, sx2));
				int maxCol = (int)F441(std::max(sx1, sx2));

				for (int x = minCol; x <= maxCol; x++)
					setPixel(x, h-1-y, t.getColor());
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

vector<Triangle>
GetTriangles(int small_read)
{
	FILE *f = fopen("tris.txt", "r");
	if (f == NULL)
	{
	   fprintf(stderr, "You must place the tris.txt file in the current directory.\n");
	   exit(EXIT_FAILURE);
	}
	fseek(f, 0, SEEK_END);
	int numBytes = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (numBytes != 241511792)
	{
	   fprintf(stderr, "Your tris.txt file is corrupted.  It should be 241511792 bytes, but you only have %d.\n", numBytes);
	   exit(EXIT_FAILURE);
	}

	if (small_read == 1)
	{
	   numBytes = 10000;
	}

	char *buffer = (char *) malloc(numBytes);
	if (buffer == NULL)
	{
	   fprintf(stderr, "Unable to allocate enough memory to load file.\n");
	   exit(EXIT_FAILURE);
	}

	fread(buffer, sizeof(char), numBytes, f);

	char *tmp = buffer;
	int numTriangles = atoi(tmp);
	while (*tmp != '\n')
	   tmp++;
	tmp++;

	if (numTriangles != 2566541)
	{
	   fprintf(stderr, "Issue with reading file -- can't establish number of triangles.\n");
	   exit(EXIT_FAILURE);
	}

	if (small_read == 1)
	   numTriangles = 100;

	vector<Triangle> tl;

	for (int i = 0 ; i < numTriangles ; i++)
	{
		Triangle t;
		double x1, y1, x2, y2, x3, y3;
		int    r, g, b;
	/*
	* Weird: sscanf has a terrible implementation for large strings.
	* When I did the code below, it did not finish after 45 minutes.
	* Reading up on the topic, it sounds like it is a known issue that
	* sscanf fails here.  Stunningly, fscanf would have been faster.
	*     sscanf(tmp, "(%lf, %lf), (%lf, %lf), (%lf, %lf) = (%d, %d, %d)\n%n",
	*              &x1, &y1, &x2, &y2, &x3, &y3, &r, &g, &b, &numRead);
	*
	*  So, instead, do it all with atof/atoi and advancing through the buffer manually...
	*/
	   tmp++,

	   x1 = atof(tmp);
	   while (*tmp != ',')
	      tmp++;
	   tmp += 2; // comma+space
	   y1 = atof(tmp);
	   while (*tmp != ')')
	      tmp++;
	   tmp += 4; // right-paren+comma+space+left-paren
	   x2 = atof(tmp);
	   while (*tmp != ',')
	      tmp++;
	   tmp += 2; // comma+space
	   y2 = atof(tmp);
	   while (*tmp != ')')
	      tmp++;
	   tmp += 4; // right-paren+comma+space+left-paren
	   x3 = atof(tmp);
	   while (*tmp != ',')
	      tmp++;
	   tmp += 2; // comma+space
	   y3 = atof(tmp);
	   while (*tmp != ')')
	      tmp++;
	   tmp += 5; // right-paren+space+equal+space+left-paren
	   r = atoi(tmp);
	   while (*tmp != ',')
	      tmp++;
	   tmp += 2; // comma+space
	   g = atoi(tmp);
	   while (*tmp != ',')
	      tmp++;
	   tmp += 2; // comma+space
	   b = atoi(tmp);
	   while (*tmp != '\n')
	      tmp++;
	   tmp++; // onto next line
	   
	   t.setVertex(0, x1, y1);
	   t.setVertex(1, x2, y2);
	   t.setVertex(2, x3, y3);
	   Pixel p{r, g, b};
	   t.setColor(p);
	   tl.push_back(t);
	   //printf("Read triangle %f, %f, %f, %f, %f, %f, %d, %d, %d\n", x1, y1, x2, y2, x3, y3, r, g, b);
	}

	free(buffer);
	return tl;
}

int main()
{	
	Image img{1786, 1344};
	vector<Triangle> tl = GetTriangles(0);

	for (Triangle t : tl){
		img.rasterize(t);
	}

	img.generatePNM("proj1C_out.pnm");
}
