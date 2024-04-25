#include "pixel.h"
#include "image.h"
#include <string>
#include <fstream>

using namespace std;

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
}colors;


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