#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <stdint.h>
#include <math.h>

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

double C441(double f)
{
    return ceil(f-0.00001);
}

double F441(double f)
{
    return floor(f+0.00001);
}

TriangleList *
GetTriangles(void)
{
   TriangleList *tl = (TriangleList *) malloc(sizeof(TriangleList));
   tl->numTriangles = 100;
   tl->triangles = (Triangle *) malloc(sizeof(Triangle)*tl->numTriangles);

   unsigned char colors[6][3] = { {255,128,0}, {255, 0, 127}, {0,204,204}, 
                                  {76,153,0}, {255, 204, 204}, {204, 204, 0}};
   for (int i = 0 ; i < 100 ; i++)
   {
       int idxI = i%10;
       int posI = idxI*100;
       int idxJ = i/10;
       int posJ = idxJ*100;
       int firstPt = (i%3);
       tl->triangles[i].X[firstPt] = posI;
       if (i == 50)
           tl->triangles[i].X[firstPt] = -10;
       tl->triangles[i].Y[firstPt] = posJ+10*(idxJ+1);
       tl->triangles[i].X[(firstPt+1)%3] = posI+105;
       tl->triangles[i].Y[(firstPt+1)%3] = posJ;
       tl->triangles[i].X[(firstPt+2)%3] = posI+i;
       tl->triangles[i].Y[(firstPt+2)%3] = posJ;
       if (i == 95)
          tl->triangles[i].Y[firstPt] = 1050;
       tl->triangles[i].color[0] = colors[i%6][0];
       tl->triangles[i].color[1] = colors[i%6][1];
       tl->triangles[i].color[2] = colors[i%6][2];
   }

   return tl;
}

void swap(double *X, double *Y, int a, int b)
{
	double tmpX = X[a];
	double tmpY = Y[a];
	X[a] = X[b];
	Y[a] = Y[b];
	X[b] = tmpX;
	Y[b] = tmpY;
}

void sortVertices(Triangle *t)
{
	
	// Sort coords such that XY[3] = {top, bot-middle, bot-right}

	double *X = t->X;
	double *Y = t->Y;


	if (Y[1] > Y[0])	// y.1 highest y val 
	{
		swap(t->X, t->Y, 1, 0);
	}

	if (Y[2] > Y[0])	// y.2 highest y val
	{
		swap(t->X, t->Y, 2, 0);
	}

	// After locating xy.0 with y-coords, order xy.1 and xy.2 with x-coords

	if (X[1] > X[2])
	{	
		swap(t->X, t->Y, 1, 2);
	}
}

int minRow(Triangle *t, int i)
{
	double *X = t->X;
	double *Y = t->Y;

	double rise = (Y[0] - Y[1]);
	double run = (X[0] - X[1]);

	if (!run){
		//cout << "minRow(): run == 0" << endl;
		return X[1];
	}

	double m = rise / run;

	double b = Y[0] - (m * X[0]);

	return (int)C441((i - b) / m);
}

int maxRow(Triangle *t, int i)
{
	double *X = t->X;
	double *Y = t->Y;

	double rise = (Y[0] - Y[2]);

	double run = (X[0] - X[2]);

	if (!run)
		cout << "maxRow(): run == 0" << endl;

	double m = rise / run;

	double b = Y[0] - (m * X[0]);

	return (int)F441((i - b) / m);
}

void rasterizeTriangleUp(Triangle *t, Image *img)
{
	sortVertices(t);
	//printf("(%lf, %lf), (%lf, %lf), (%lf, %lf)\n", t->X[0], t->Y[0], t->X[1], t->Y[1], t->X[2], t->Y[2]);
	double *X = t->X;
	double *Y = t->Y;

	int minY = (int)C441(Y[2]);
	int maxY = (int)F441(Y[0]);

	// Determine left hand slope

	for (int i = minY; i <= maxY; i++)
	{	
		
		for (int j = minRow(t, i); j <= maxRow(t, i); j++)
		{	
			// Invert y axis (1000-i-1)
			if ((999-i >= 0) && (999-i <= 999) && (j <= 999) && (j >= 0))
				img->setPixel(j, 999-i, Pixel(t->color[0], t->color[1], t->color[2]));
		}
		/*
		int minrow = minRow(t, i);
		int maxrow = maxRow(t, i);
		printf("%d : %d -> %d\n", i, minrow, maxrow);
		*/
	}
}

int main()
{	
	Image img{1000, 1000};
	TriangleList *tl = GetTriangles();

	for (int i = 0 ; i < tl->numTriangles; i++)
	{
		Triangle t = *(tl->triangles+i);

		//cout << "Rasterizing...\n";
		rasterizeTriangleUp(&t, &img);
		
	}

	img.generatePNM("proj1B_out.pnm");
}
