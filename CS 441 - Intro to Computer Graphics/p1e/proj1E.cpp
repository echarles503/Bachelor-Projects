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

	void Print(void){
		printf("%lf %lf %lf\n", X, Y, Z);
		//std::cout << X << " " << Y << " " << Z << endl;
	}

	double operator[](int i) const 
	{	
		switch(i) {
			case 0: return X;
			case 1: return Y;
			case 2: return Z;

			default: 
				throw std::out_of_range("Index out of range");
		}
	}

	double& operator[](int i) 
	{	
		switch(i) {
			case 0: return X;
			case 1: return Y;
			case 2: return Z;

			default: 
				throw std::out_of_range("Index out of range");
		}
	}

	Vec3 operator-(const Vec3 &other)
	{
		return Vec3(X - other.X, Y - other.Y, Z - other.Z);
	}

	Vec3 operator^(const Vec3 &other)
	{
		double x = Y * other.Z - Z * other.Y;
		double y = Z * other.X - X * other.Z;
		double z = X * other.Y - Y * other.X;
		return Vec3(x,y,z);
	}

	double operator*(const Vec3 &other) const
	{
		return X*other.X + Y*other.Y + Z*other.Z;
	}

	void normalize()
	{
		double magnitude = sqrt(X*X + Y*Y + Z*Z);
		X = X / magnitude;
		Y = Y / magnitude;
		Z = Z / magnitude;
	}
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

vector<double> CrossProduct(vector<double> &u, vector<double> &v)
{
	return {u[1]*v[2] - u[2]*v[1], u[2]*v[0] - u[0]*v[2], u[0]*v[1] - u[1]*v[0]};
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
	std::vector<Vec3> normals;

	// Constructors
	Triangle() 
	: 
	vertices{{0,0,-1},{0,0,-1},{0,0,-1}}, 
	colors{{0,0,0},{0,0,0},{0,0,0}}, 
	normals{{0,0,0},{0,0,0},{0,0,0}}
	{ }
	
	Triangle(std::vector<Vec3> &_vertices, std::vector<RGB> &_colors, std::vector<Vec3> &_normals)
	: 
	vertices{_vertices},
	colors{_colors}, 
	normals{_normals} 
	{ }

	~Triangle() { }

	void printVertices()
	{
		for (int i = 0; i < 3; i++)
		{
			cout << "Vtx" << i << " " << endl;
			cout << vertices[i].X << " " << vertices[i].Y << " " << vertices[i].Z << endl;
			cout << colors[i].R << " " << colors[i].G << " " << colors[i].B << endl;
			cout << normals[i].X << " " << normals[i].Y << " " << normals[i].Z << endl;
		}
	}

	void swapVertices(int a, int b)
	{
		std::swap(vertices[a], vertices[b]);
		std::swap(colors[a], colors[b]);
	}

	void sortVertices()
	{
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

				// Determine left/right scanline ends, round to integers
				int minCol = (int)C441(std::min(sx1, sx2));
				int maxCol = (int)F441(std::max(sx1, sx2));

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

				// Determine left/right scanline ends, round to integers
				int minCol = (int)C441(std::min(sx1, sx2));
				int maxCol = (int)F441(std::max(sx1, sx2));

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


class Matrix
{

public:
	vector<vector<double>> A;  // A[i][j] means row i, column j

    Matrix() : A{{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}} {}

    Matrix(vector<vector<double>> &_a) : A{_a} {}

    Matrix(vector<Vec3> &_a) 
    : A{{_a[0].X, _a[0].Y, _a[0].Z, 0},
    	{_a[1].X, _a[1].Y, _a[1].Z, 0},
    	{_a[2].X, _a[2].Y, _a[2].Z, 0},
    	{_a[3].X, _a[3].Y, _a[3].Z, 1}} 
    {}

    //void            TransformPoint(const double *ptIn, double *ptOut);
    //static Matrix   ComposeMatrices(const Matrix &, const Matrix &);
    //void            Print(ostream &o);


    void Print(ostream &o)
	{
	    for (int i = 0 ; i < 4 ; i++)
	    {
	        char str[256];
	        sprintf(str, "(%.7f %.7f %.7f %.7f)\n", A[i][0], A[i][1], A[i][2], A[i][3]);
	        o << str;
	    }
	}

	static Matrix ComposeMatrices(const Matrix &M1, const Matrix &M2)
	{
	    Matrix rv;
	    for (int i = 0 ; i < 4 ; i++)
	        for (int j = 0 ; j < 4 ; j++)
	        {
	            rv.A[i][j] = 0;
	            for (int k = 0 ; k < 4 ; k++)
	                rv.A[i][j] += M1.A[i][k]*M2.A[k][j];
	        }

	    return rv;
	}

	void TransformPoint(const double *ptIn, double *ptOut)
	{
	    ptOut[0] = ptIn[0]*A[0][0]
	             + ptIn[1]*A[1][0]
	             + ptIn[2]*A[2][0]
	             + ptIn[3]*A[3][0];
	    ptOut[1] = ptIn[0]*A[0][1]
	             + ptIn[1]*A[1][1]
	             + ptIn[2]*A[2][1]
	             + ptIn[3]*A[3][1];
	    ptOut[2] = ptIn[0]*A[0][2]
	             + ptIn[1]*A[1][2]
	             + ptIn[2]*A[2][2]
	             + ptIn[3]*A[3][2];
	    ptOut[3] = ptIn[0]*A[0][3]
	             + ptIn[1]*A[1][3]
	             + ptIn[2]*A[2][3]
	             + ptIn[3]*A[3][3];
	}

};


class Camera
{
  public:
    double          near, far;
    double          angle;
    Vec3 			position;
    Vec3 			focus;
    Vec3			up;

    Matrix ViewTransform(void) 
    {	
    	double a = 1/tan(angle/2);
    	double b = (far+near)/(far-near);
    	double c = (2*far*near)/(far-near);
    	
    	vector<vector<double>> entries
    	{
  			{a, 0, 0, 0},
			{0, a, 0, 0},
			{0, 0, b, -1},
			{0, 0, c, 0}
		};
    	return Matrix(entries);
    }    

    Matrix CameraTransform(void)
    {
	    vector<Vec3> cartesian
	    {
  			Vec3(1, 0, 0),
			Vec3(0, 1, 0),
			Vec3(0, 0, 1),
			Vec3(0, 0, 0)
		};

		Vec3 w = position - focus;
		Vec3 u = up ^ w;
		Vec3 v = w ^ u;
		u.normalize(); v.normalize(); w.normalize();

		Vec3 t = Vec3(0,0,0) - position;

		vector<Vec3> e = {
			Vec3(u.X, v.X, w.X),
			Vec3(u.Y, v.Y, w.Y),
			Vec3(u.Z, v.Z, w.Z),
			Vec3(u*t,v*t,w*t)
		};

		/* Note:
		 *
		 * `^` -> cross product operator
		 * `*` -> dot product operator
		 */

		/*
		e[0][0] = ((cartesian[0] ^ v) * w ) / ((u ^ v) * w);
		e[0][1] = ((u ^ cartesian[0]) * w ) / ((u ^ v) * w);
		e[0][2] = ((u ^ v) * cartesian[0] ) / ((u ^ v) * w);

		e[1][0] = ((cartesian[1] ^ v) * w) / ((u ^ v) * w);
		e[1][1] = ((u ^ cartesian[1]) * w) / ((u ^ v) * w);
		e[1][2] = ((u ^ v) * cartesian[1]) / ((u ^ v) * w);

		e[2][0] = ((cartesian[2] ^ v) * w) / ((u ^ v) * w);
		e[2][1] = ((u ^ cartesian[2]) * w) / ((u ^ v) * w);
		e[2][2] = ((u ^ v) * cartesian[2]) / ((u ^ v) * w);
		*/
		
	    return Matrix(e);
    }

    Matrix DeviceTransform(void) 
    {
    	double n = 1000, m = 1000;

    	vector<vector<double>> entries
    	{
  			{n/2, 0,   0, 0},
			{0,   m/2, 0, 0},
			{0,   0 ,  1, 0},
			{n/2, m/2, 0, 1}
		};

		return Matrix(entries);
    }

    Matrix ComposeTransforms()
    {
    	Matrix m = Matrix::ComposeMatrices(CameraTransform(), ViewTransform());
    	return Matrix::ComposeMatrices(m, DeviceTransform());
    }

};


double SineParameterize(int curFrame, int nFrames, int ramp)
{
    int nNonRamp = nFrames-2*ramp;
    double height = 1./(nNonRamp + 4*ramp/M_PI);
    if (curFrame < ramp)
    {
        double factor = 2*height*ramp/M_PI;
        double eval = cos(M_PI/2*((double)curFrame)/ramp);
        return (1.-eval)*factor;
    }
    else if (curFrame > nFrames-ramp)
    {
        int amount_left = nFrames-curFrame;
        double factor = 2*height*ramp/M_PI;
        double eval = cos(M_PI/2*((double)amount_left/ramp));
        return 1. - (1-eval)*factor;
    }
    double amount_in_quad = ((double)curFrame-ramp);
    double quad_part = amount_in_quad*height;
    double curve_part = height*(2*ramp)/M_PI;
    return quad_part+curve_part;
}

Camera
GetCamera(int frame, int nframes)
{
    double t = SineParameterize(frame, nframes, nframes/10);
    Camera c;
    c.near = 5;
    c.far = 200;
    c.angle = M_PI/6;
    c.position[0] = 40*sin(2*M_PI*t);
    c.position[1] = 40*cos(2*M_PI*t);
    c.position[2] = 40;
    c.focus[0] = 0;
    c.focus[1] = 0;
    c.focus[2] = 0;
    c.up[0] = 0;
    c.up[1] = 1;
    c.up[2] = 0;
    return c;
}


char *
Read3Numbers(char *tmp, double *v1, double *v2, double *v3)
{
    *v1 = atof(tmp);
    while (*tmp != ' ')
       tmp++;
    tmp++; /* space */
    *v2 = atof(tmp);
    while (*tmp != ' ')
       tmp++;
    tmp++; /* space */
    *v3 = atof(tmp);
    while (*tmp != ' ' && *tmp != '\n')
       tmp++;
    return tmp;
}

vector<Triangle>
Get3DTriangles()
{
   FILE *f = fopen("ws_tris.txt", "r");
   if (f == NULL)
   {
       fprintf(stderr, "You must place the ws_tris.txt file in the current directory.\n");
       exit(EXIT_FAILURE);
   }
   fseek(f, 0, SEEK_END);
   int numBytes = ftell(f);
   fseek(f, 0, SEEK_SET);
   if (numBytes != 3892295)
   {
       fprintf(stderr, "Your ws_tris.txt file is corrupted.  It should be 3892295 bytes, but you have %d.\n", numBytes);
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
 
   if (numTriangles != 14702)
   {
       fprintf(stderr, "Issue with reading file -- can't establish number of triangles.\n");
       exit(EXIT_FAILURE);
   }

   vector<Triangle> tl;
   for (int i = 0 ; i < numTriangles ; i++)
   {
   		Triangle t;
   		double x1, x2, x3, y1, y2, y3, z1, z2, z3;
   		double r1, r2, r3, g1, g2, g3, b1, b2, b3;
   		double n1, n2, n3, n4, n5, n6, n7, n8, n9;

   		tmp = Read3Numbers(tmp, &x1, &y1, &z1);
      	tmp += 3; /* space+slash+space */
      	tmp = Read3Numbers(tmp, &r1, &g1, &b1);
      	tmp += 3; /* space+slash+space */
      	tmp = Read3Numbers(tmp, &n1, &n2, &n3);
      	tmp++;    /* newline */

   		tmp = Read3Numbers(tmp, &x2, &y2, &z2);
      	tmp += 3; /* space+slash+space */
      	tmp = Read3Numbers(tmp, &r2, &g2, &b2);
      	tmp += 3; /* space+slash+space */
      	tmp = Read3Numbers(tmp, &n4, &n5, &n6);
      	tmp++;    /* newline */

   		tmp = Read3Numbers(tmp, &x3, &y3, &z3);
      	tmp += 3; /* space+slash+space */
      	tmp = Read3Numbers(tmp, &r3, &g3, &b3);
      	tmp += 3; /* space+slash+space */
      	tmp = Read3Numbers(tmp, &n7, &n8, &n9);
      	tmp++;    /* newline */

      	std::vector<Vec3> vertices
   		{ 
			{Vec3(x1, y1, z1)},
			{Vec3(x2, y2, z2)},
			{Vec3(x3, y3, z3)} 
		};

		std::vector<RGB> colors
		{
			{RGB(r1, g1, b1)},
			{RGB(r2, g2, b2)},
			{RGB(r3, g3, b3)}
		};

		std::vector<Vec3> normals
   		{ 
			{Vec3(n1, n2, n3)},
			{Vec3(n4, n5, n6)},
			{Vec3(n7, n8, n9)} 
		};

		t = Triangle(vertices, colors, normals);

		tl.push_back(t);
	}
   free(buffer);
   return tl;
}

void printFrame(int frame, string filename)
{

	Image img{1000, 1000};
	vector<Triangle> tl = Get3DTriangles();
	
	Camera cam = GetCamera(frame, 1000);
	Matrix m = cam.ComposeTransforms();

	for (Triangle t : tl)
	{
		for (int i = 0; i < 3; i++)
		{
			Vec3 vtx = t.vertices[i];
			double pts[4] = {vtx.X, vtx.Y, vtx.Z, 1};
			double outPts[4];
			m.TransformPoint(pts, outPts);
			t.vertices[i] = Vec3(outPts[0]/outPts[3], outPts[1]/outPts[3], outPts[2]/outPts[3]);
		}
		img.rasterize(t);
	}
	img.generatePNM(filename);

}

int main()
{		
	printFrame(0, "proj1E_frame0000.pnm");
	printFrame(250, "proj1E_frame0250.pnm");
	printFrame(500, "proj1E_frame0500.pnm");
	printFrame(750, "proj1E_frame0750.pnm");
}
