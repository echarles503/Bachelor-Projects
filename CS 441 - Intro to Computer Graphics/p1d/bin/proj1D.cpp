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

enum Plane {X, Y, Z};

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
	Triangle() : vertices{{0, 0, -1}, {0, 0, -1}, {0, 0, -1}}, colors{{0,0,0},{0,0,0},{0,0,0}}
	{

	}
	
	Triangle(std::vector<Vec3> &_vertices, std::vector<RGB> &_colors)
	: vertices{_vertices}, colors{_colors} {}

	~Triangle() = default;

	// Public fxns

	void printVertices()
	{
		for (int i = 0; i < 3; i++)
		{
			cout << "Vtx" << i << "= ";
			cout << vertices[i].X << " " << vertices[i].Y << " " << vertices[i].Z << " = ";
			cout << colors[i].R << " " << colors[i].G << " " << colors[i].B << endl;
		}
	}

	void swapVertices(int a, int b)
	{
		std::swap(vertices[a], vertices[b]);
		std::swap(colors[a][0], colors[b][0]);
		std::swap(colors[a][1], colors[b][1]);
		std::swap(colors[a][2], colors[b][2]);
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

	double zslope (int zy1, int zy2)
	{
		return (vertices[zy2].Z - vertices[zy1].Z) / (vertices[zy2].Y - vertices[zy1].Y);
	}

	double scanline(int vtx, int y, double m)
	{
		double x1 = vertices[vtx].X;
		double y1 = vertices[vtx].Y;

		return x1 + (y - y1) * m;
	}

	double zbuffer(int idx, int y, double m)
	{

		return vertices[idx].Z + (y - vertices[idx].Y) * m;
	}

	double interpolateColor(int vtx1, int vtx2, int c, int y)
	{
		// 2 vtxs, color index c, row/col value xy, plane to grab vertex array
		double c1 = colors[vtx1][c];
		double c2 = colors[vtx2][c];
		double y1 = vertices[vtx1].Y;
		double y2 = vertices[vtx2].Y;

		return c1 + (y - y1) * ( c2 - c1 ) / ( y2 - y1 );
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

		// Calculate edges

		// Calculate xy slope for each edge
		double m1 = t.slope(0, 1);
		double m2 = t.slope(0, 2);
		double m3 = t.slope(1, 2);

		// Calculate zy slope for each edge
		double zm1 = t.zslope(0, 1);
		double zm2 = t.zslope(0, 2);
		double zm3 = t.zslope(1, 2);
		//printf("z1: %lf, z2: %lf, z3: %lf");

		int minRow = C441(t.vertices[0].Y);
		int maxRow = F441(t.vertices[2].Y);

		for (int y = minRow; y <= maxRow; y++)
		{
			// Check if scanline is below midpoint
			if (y < t.vertices[1].Y)
			{
				//printf("Rasterizing along row: %d\n", y);
				// Calculate scanline ends
				double sx1 = t.scanline(0, y, m1);
				double sx2 = t.scanline(0, y, m2);

				// Determine left/right scanline ends
				double xL = sx1;
				double xR = sx2;
				
				// Interpolate scanline ends
				double zL = t.zbuffer(0, y, zm1);
				double zR = t.zbuffer(0, y, zm2);

				//interpolateColor(int vtx1, int vtx2, int c, int xy, Plane plane)
				double rL = t.interpolateColor(0, 1, 0, y);
				double rR = t.interpolateColor(0, 2, 0, y);

				double gL = t.interpolateColor(0, 1, 1, y);
				double gR = t.interpolateColor(0, 2, 1, y);

				double bL = t.interpolateColor(0, 1, 2, y);
				double bR = t.interpolateColor(0, 2, 2, y);

				// Determine left and right ends
				if (sx2 < sx1){
					std::swap(xL, xR);
					std::swap(zL, zR);
					std::swap(rL, rR);
					std::swap(gL, gR);
					std::swap(bL, bR);
				}

				// Round endpoints to integers
				int minCol = (int)C441(xL);
				int maxCol = (int)F441(xR);

				for (int x = minCol; x <= maxCol; x++)
				{
			//		printf("Rasterizing along column: %d row: %d\n", x, y);
			//		printf("LeftEnd: %lf rL: %lf RightEnd: %lf rR: %lf\n", xL, rL, xR, rR);

					// Calculate interpolated-z for each pixel in scanline
					double sz = (zR - zL) / (xR - xL);
					double z = zL + (x - xL) * sz;

					/* Barycentric coordinate interpolation method
					double u = ( (Y[1] - Y[2]) * (x - X[2]) + (X[2] - X[1]) * (y - Y[2]) ) / 
							   ( (Y[1] - Y[2]) * (X[0] - X[2]) + (X[2] - X[1]) * (Y[0] - Y[2]) );

					double v = ( (Y[2] - Y[0]) * (x - X[2]) + (X[0] - X[2]) * (y - Y[2]) ) / 
					           ( (Y[1] - Y[2]) * (X[0] - X[2]) + (X[2] - X[1]) * (Y[0] - Y[2]) );

					double w = 1 - u - v;
					
					//(r, g, b) = (ur1 + vr2 + wr3, ug1 + vg2 + wg3, ub1 + vb2 + w*b3)

					//double i_r = u * t.getColor(0,0) + v * t.getColor(1,0) + w * t.getColor(2,0);
					//double i_g = u * t.getColor(0,1) + v * t.getColor(1,1) + w * t.getColor(2,1); 
					//double i_b = u * t.getColor(0,2) + v * t.getColor(1,2) + w * t.getColor(2,2); 
					*/

					double i_r = rR + (x - xR) * ((rL - rR) / (xL - xR));
					double i_g = gR + (x - xR) * ((gL - gR) / (xL - xR));
					double i_b = bR + (x - xR) * ((bL - bR) / (xL - xR));

					// Convert rgb values to byte integers
					uint8_t r = (uint8_t)C441(i_r * 255);
					uint8_t g = (uint8_t)C441(i_g * 255);
					uint8_t b = (uint8_t)C441(i_b * 255);

			//		printf("(%d, %d) z: %lf   r: %lf g: %lf b: %lf \n", x, y, z, i_r, i_g, i_b);

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
				
				double sx1 = t.scanline(0, y, m2);
				double sx2 = t.scanline(1, y, m3);
				
				double xL = sx1;
				double xR = sx2;

				double zL = t.zbuffer(0, y, zm2);
				double zR = t.zbuffer(1, y, zm3);

				//interpolateColor(int vtx1, int vtx2, int c, int xy, Plane plane)
				double rL = t.interpolateColor(0, 2, 0, y);
				double rR = t.interpolateColor(1, 2, 0, y);

				double gL = t.interpolateColor(0, 2, 1, y);
				double gR = t.interpolateColor(1, 2, 1, y);

				double bL = t.interpolateColor(0, 2, 2, y);
				double bR = t.interpolateColor(1, 2, 2, y);

				// Determine left and right z-end
				if (sx2 < sx1){
					std::swap(xL, xR);
					std::swap(zL, zR);
					std::swap(rL, rR);
					std::swap(gL, gR);
					std::swap(bL, bR);
				}
				

				int minCol = (int)C441(xL);
				int maxCol = (int)F441(xR);

				for (int x = minCol; x <= maxCol; x++)
				{
			//		printf("Rasterizing along column: %d row: %d\n", x, y);
			//		printf("LeftEnd: %lf rL: %lf RightEnd: %lf rR: %lf\n", xL, rL, xR, rR);

					// Calculate interpolated-z for each pixel in scanline
					double sz = (zR - zL) / (xR - xL);
					double z = zL + (x - xL) * sz;

					/* Barycentric coordinate interpolation method
					double u = ( (Y[1] - Y[2]) * (x - X[2]) + (X[2] - X[1]) * (y - Y[2]) ) / 
							   ( (Y[1] - Y[2]) * (X[0] - X[2]) + (X[2] - X[1]) * (Y[0] - Y[2]) );

					double v = ( (Y[2] - Y[0]) * (x - X[2]) + (X[0] - X[2]) * (y - Y[2]) ) / 
					           ( (Y[1] - Y[2]) * (X[0] - X[2]) + (X[2] - X[1]) * (Y[0] - Y[2]) );

					double w = 1 - u - v;
					
					//(r, g, b) = (ur1 + vr2 + wr3, ug1 + vg2 + wg3, ub1 + vb2 + w*b3)

					//double i_r = u * t.getColor(0,0) + v * t.getColor(1,0) + w * t.getColor(2,0);
					//double i_g = u * t.getColor(0,1) + v * t.getColor(1,1) + w * t.getColor(2,1); 
					//double i_b = u * t.getColor(0,2) + v * t.getColor(1,2) + w * t.getColor(2,2); 
					*/

					// Calculate interpolated-rgb for each pixel in scanline
					double i_r = rL + (x - xL) * ((rR - rL) / (xR - xL));
					double i_g = gL + (x - xL) * ((gR - gL) / (xR - xL));
					double i_b = bL + (x - xL) * ((bR - bL) / (xR - xL));

					// Convert rgb values to byte integers
					uint8_t r = (uint8_t)C441(i_r * 255);
					uint8_t g = (uint8_t)C441(i_g * 255);
					uint8_t b = (uint8_t)C441(i_b * 255);

			//		printf("(%d, %d) z: %lf   r: %lf g: %lf b: %lf \n", x, y, z, i_r, i_g, i_b);
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

       /*
       tl->triangles[i].X[0] = x1;
       tl->triangles[i].X[1] = x2;
       tl->triangles[i].X[2] = x3;
       tl->triangles[i].Y[0] = y1;
       tl->triangles[i].Y[1] = y2;
       tl->triangles[i].Y[2] = y3;
       tl->triangles[i].Z[0] = z1;
       tl->triangles[i].Z[1] = z2;
       tl->triangles[i].Z[2] = z3;
       */

      	std::vector<RGB> colors
		{
			{RGB(r[0], g[0], b[0])},
			{RGB(r[1], g[1], b[1])},
			{RGB(r[2], g[2], b[2])}
		};

     	std::vector<Vec3> vertices
   		{ 
			{Vec3(x1, y1, z1)},
			{Vec3(x2, y2, z2)},
			{Vec3(x3, y3, z3)} 
		};

       /*
       tl->triangles[i].color[0][0] = r[0];
       tl->triangles[i].color[0][1] = g[0];
       tl->triangles[i].color[0][2] = b[0];
       tl->triangles[i].color[1][0] = r[1];
       tl->triangles[i].color[1][1] = g[1];
       tl->triangles[i].color[1][2] = b[1];
       tl->triangles[i].color[2][0] = r[2];
       tl->triangles[i].color[2][1] = g[2];
       tl->triangles[i].color[2][2] = b[2];
       */

      	t = Triangle(vertices, colors);
       

       tl.push_back(t);
       ////printf("Read triangle (%f, %f, %f) / (%f, %f, %f), (%f, %f, %f) / (%f, %f, %f), (%f, %f, %f) / (%f, %f, %f)\n", x1, y1, z1, r[0], g[0], b[0], x2, y2, z2, r[1], g[1], b[1], x3, y3, z3, r[2], g[2], b[2]);
   }

   free(buffer);
   return tl;
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
