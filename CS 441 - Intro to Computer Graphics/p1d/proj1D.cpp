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

struct Vec3 {
	double X, Y, Z;

	Vec3()
	: X(0), Y(0), Z(-1) {}

	Vec3(double _x, double _y, double _z)
	: X(_x), Y(_y), Z(_z) {}

	~Vec3() {}
};

struct RGB {
	double R, G, B;

	RGB()
	: R(255), G(255), B(255) {}

	RGB(double _r, double _g, double _b)
	: R(_r), G(_g), B(_b) {}

	double operator[](int i) const {
		
		switch(i) {
			case 0: return R;
			case 1: return G;
			case 2: return B;

			default: 
				throw std::out_of_range("Index out of range");
		}
	}

	double& operator[](int i) {
		
		switch(i) {
			case 0: return R;
			case 1: return G;
			case 2: return B;

			default: 
				throw std::out_of_range("Index out of range");
		}
	}

	~RGB() {}
};

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
	double depth;
public:
	// Constructors
	Pixel()
	{
		r = 0;
		g = 0;
		b = 0;
		depth = -1.0;
	}

	Pixel(uint8_t _red, uint8_t _green, uint8_t _blue, double _depth=-1.0)
	{
		r = _red;
		g = _green;
		b = _blue;
		depth = _depth;
	}

	Pixel(int _red, int _green, int _blue, double _depth=-1.0)
	{
		r = static_cast<uint8_t>(_red);
		g = static_cast<uint8_t>(_green);
		b = static_cast<uint8_t>(_blue);
		depth = _depth;
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

	double getDepth(){ return depth; }
	void setDepth(double _depth){ depth = _depth; }

};

class Triangle
{

public:
	std::vector<Vec3> vertices;
	std::vector<RGB> colors;

	// Constructors
	Triangle() 
	: vertices{{0, 0, -1}, {0, 0, -1}, {0, 0, -1}}, colors{{0,0,0},{0,0,0},{0,0,0}} {}
	
	Triangle(std::vector<Vec3> &_vertices, std::vector<RGB> &_colors)
	: vertices{_vertices}, colors{_colors} {}

	~Triangle() {}

	void printVertices()
	{
		for (int i = 0; i < 3; i++)
		{
			cout << "Vtx" << i << "= ";
			cout << vertices[i].X << " " << vertices[i].Y << " " << vertices[i].Z << "= ";
			cout << colors[i].R << " " << colors[i].G << " " << colors[i].B << endl;
		}
	}

	void swapVertices(int a, int b)
	{
		std::swap(vertices[a], vertices[b]);
		std::swap(colors[a], colors[b]);
	}

	void sortVertices()
	{
		// Sort Y coordinates in increasing order

		if (vertices[0].Y > vertices[1].Y) 		
			swapVertices(0, 1);

		if (vertices[0].Y > vertices[2].Y)		
			swapVertices(0, 2);

		if (vertices[1].Y > vertices[2].Y)
			swapVertices(1, 2);
	}

	double slope(int vtx1, int vtx2)
	{
		double x1 = vertices[vtx1].X, x2 = vertices[vtx2].X;
		double y1 = vertices[vtx1].Y, y2 = vertices[vtx2].Y;

		return (x2 - x1) / (y2 - y1);
	}

	double scanline(int vtx, int y, double m)
	{
		double x1 = vertices[vtx].X;
		double y1 = vertices[vtx].Y;

		return x1 + (y - y1) * m;
	}

	vector<double> barycentricCoords(int x, int y)
	{
		double x1 = vertices[0].X, x2 = vertices[1].X, x3 = vertices[2].X;
		double y1 = vertices[0].Y, y2 = vertices[1].Y, y3 = vertices[2].Y;
		double z1 = vertices[0].Z, z2 = vertices[1].Z, z3 = vertices[2].Z;

		double u = ((y2 - y3) * (x - x3) + (x3 - x2) * (y - y3)) /
				   ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));

		double v = ((y3 - y1) * (x - x3) + (x1 - x3) * (y - y3)) /
				   ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));

		double w = 1 - u - v;

		return {u, v, w};
	}

	double interpolateZ(vector<double> bc)
	{
		// bc: vector of barycentric coordinates
		double z1 = vertices[0].Z, z2 = vertices[1].Z, z3 = vertices[2].Z;
		double u = bc[0], v = bc[1], w = bc[2];

		return u * z1 + v * z2 + w * z3;
	}

	RGB interpolateColor(vector<double> bc)
	{
		// bc: vector of barycentric coordinates
		double u = bc[0], v = bc[1], w = bc[2];
		double r1 = colors[0].R, r2 = colors[1].R, r3 = colors[2].R;
		double g1 = colors[0].G, g2 = colors[1].G, g3 = colors[2].G;
		double b1 = colors[0].B, b2 = colors[1].B, b3 = colors[2].B;
		
		double iR = u * r1 + v * r2 + w * r3;
		double iG = u * g1 + v * g2 + w * g3;
		double iB = u * b1 + v * b2 + w * b3;

		return RGB(iR, iG, iB);
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

	bool inbounds(int x, int y)
	{
		return (h-1-y >= 0) && (h-1-y <= h-1) && (x <= w-1) && (x >= 0);
	}

	void rasterize(Triangle &t)
	{

		// Sort vertices by y-coordinate
		t.sortVertices();

		double x1 = t.vertices[0].X, x2 = t.vertices[1].X, x3 = t.vertices[2].X;
		double y1 = t.vertices[0].Y, y2 = t.vertices[1].Y, y3 = t.vertices[2].Y;
		double z1 = t.vertices[0].Z, z2 = t.vertices[1].Z, z3 = t.vertices[2].Z;

		
		// Calculate xy slope for each edge
		double m1 = t.slope(0, 1);
		double m2 = t.slope(0, 2);
		double m3 = t.slope(1, 2);

		// Determine bounding rows
		int minRow = C441(y1);
		int maxRow = F441(y3);

		for (int y = minRow; y <= maxRow; y++)
		{
			// Check if scanline is below midpoint
			if (y < y2)
			{
				// Calculate scanline ends
				double sx1 = t.scanline(0, y, m1);
				double sx2 = t.scanline(0, y, m2);

				double xL = min(sx1, sx2);
				double xR = max(sx1, sx2);
				// Determine left/right scanline ends, round to integers
				int minCol = (int)C441(xL);
				int maxCol = (int)F441(xR);

				for (int x = minCol; x <= maxCol; x++)
				{
					// Barycentric-Coordinate interpolation method
					vector<double> bc = t.barycentricCoords(x, y); 
					
					// Interpolate z
					double z = t.interpolateZ(bc);

					// Interpolate colors
					RGB iColors = t.interpolateColor(bc); 

					// Convert rgb values to byte integers
					uint8_t r = (uint8_t)C441(iColors.R * 255);
					uint8_t g = (uint8_t)C441(iColors.G * 255);
					uint8_t b = (uint8_t)C441(iColors.B * 255);

					// Assign pixel if its depth is greater than the current pixel
					if (inbounds(x,y))
					{
						double pixelDepth = getPixel(x, h-1-y).getDepth();
						if (z >= pixelDepth)
							setPixel(x, h-1-y, Pixel(r, g, b, z));
					}
				}

			}
			else
			{
				// Calculate scanline ends
				double sx1 = t.scanline(0, y, m2);
				double sx2 = t.scanline(1, y, m3);

				double xL = min(sx1, sx2);
				double xR = max(sx1, sx2);

				int minCol = (int)C441(xL);
				int maxCol = (int)F441(xR);

				for (int x = minCol; x <= maxCol; x++)
				{
					// Barycentric-Coordinate interpolation method
					vector<double> bc = t.barycentricCoords(x, y); 
					
					// Interpolate z
					double z = t.interpolateZ(bc);
					// Interpolate colors
					RGB iColors = t.interpolateColor(bc); 

					// Convert rgb values to byte integers
					uint8_t r = (uint8_t)C441(iColors.R * 255);
					uint8_t g = (uint8_t)C441(iColors.G * 255);
					uint8_t b = (uint8_t)C441(iColors.B * 255);

					if (inbounds(x,y))
					{
						double pixelDepth = getPixel(x, h-1-y).getDepth();

						if (z >= pixelDepth)
							setPixel(x, h-1-y, Pixel(r, g, b, z));
					}
				}
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

char *
ReadTuple3(char *tmp, double *v1, double *v2, double *v3)
{
    tmp++; /* left paren */
    *v1 = atof(tmp);
    while (*tmp != ',')
       tmp++;
    tmp += 2; // comma+space
    *v2 = atof(tmp);
    while (*tmp != ',')
       tmp++;
    tmp += 2; // comma+space
    *v3 = atof(tmp);
    while (*tmp != ')')
       tmp++;
    tmp++; /* right paren */
    return tmp;
}

vector<Triangle>
Get3DTriangles()
{
   FILE *f = fopen("tris_w_r_rgb.txt", "r");
   if (f == NULL)
   {
       fprintf(stderr, "You must place the tris_w_r_rgb.txt file in the current directory.\n");
       exit(EXIT_FAILURE);
   }
   fseek(f, 0, SEEK_END);
   int numBytes = ftell(f);
   fseek(f, 0, SEEK_SET);
   if (numBytes != 13488634)
   {
       fprintf(stderr, "Your tris_w_r_rgb.txt file is corrupted.  It should be 13488634 bytes, but you have %d.\n", numBytes);
       exit(EXIT_FAILURE);
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
 
   if (numTriangles != 42281)
   {
       fprintf(stderr, "Issue with reading file -- can't establish number of triangles.\n");
       exit(EXIT_FAILURE);
   }
   /*
   TriangleList *tl = (TriangleList *) malloc(sizeof(TriangleList));
   tl->numTriangles = numTriangles;
   tl->triangles = (Triangle *) malloc(sizeof(Triangle)*tl->numTriangles);
    */
   vector<Triangle> tl;

   for (int i = 0 ; i < numTriangles ; i++)
   {
       double x1, y1, z1, x2, y2, z2, x3, y3, z3;
       double r[3], g[3], b[3];
       Triangle t;
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
       tmp = ReadTuple3(tmp, &x1, &y1, &z1);
       tmp += 3; /* space+equal+space */
       tmp = ReadTuple3(tmp, r+0, g+0, b+0);
       tmp += 2; /* comma+space */
       tmp = ReadTuple3(tmp, &x2, &y2, &z2);
       tmp += 3; /* space+equal+space */
       tmp = ReadTuple3(tmp, r+1, g+1, b+1);
       tmp += 2; /* comma+space */
       tmp = ReadTuple3(tmp, &x3, &y3, &z3);
       tmp += 3; /* space+equal+space */
       tmp = ReadTuple3(tmp, r+2, g+2, b+2);
       tmp++;    /* newline */

		std::vector<Vec3> vertices
   		{ 
			{Vec3(x1, y1, z1)},
			{Vec3(x2, y2, z2)},
			{Vec3(x3, y3, z3)} 
		};

		std::vector<RGB> colors
		{
			{RGB(r[0], g[0], b[0])},
			{RGB(r[1], g[1], b[1])},
			{RGB(r[2], g[2], b[2])}
		};

		t = Triangle(vertices, colors);

		tl.push_back(t);
       //printf("Read triangle (%f, %f, %f) / (%f, %f, %f), (%f, %f, %f) / (%f, %f, %f), (%f, %f, %f) / (%f, %f, %f)\n", x1, y1, z1, r[0], g[0], b[0], x2, y2, z2, r[1], g[1], b[1], x3, y3, z3, r[2], g[2], b[2]);
   }

   free(buffer);
   return tl;
}

void debug(vector<Triangle> tl, Image &img)
{
	int tick = 0;
	for (Triangle t : tl){
		cout << "Before:" << endl;
		t.printVertices();
		t.sortVertices();
		cout << "After:" << endl;
		t.printVertices();
		//img.rasterize(t);
		if (tick >= 5)
		{
			break;
		}
		tick++;
		/*
		cout << "Working on Triangle: " << tick << endl;

		img.rasterize(t);
		tick++;
		if (tick == 10)
			break;
		*/

	}
}


int main()
{	
	Image img{1000, 1000};
	vector<Triangle> tl = Get3DTriangles();

	//debug(tl, img);
	for (Triangle t : tl)
		img.rasterize(t);

	img.generatePNM("proj1D_out.pnm");
}
