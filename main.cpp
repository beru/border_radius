
#include <stdio.h>

#include "graphics/graphics.h"

int main(int argc, char* argv[])
{
	Graphics::BorderStyle bs;
	bs.width = 2;
	bs.color = 0x2FFFAF;

	Graphics::DrawLinearGradient(110,10,0xAAAA00, 300,200,0x00AAAA);
	
#if 1
	for (int i=0; i<20; ++i) {
		Graphics::DrawRectangle(200,100-10*i, 100,100, 0x4F6F9F, bs);
	}
#endif

	return 0;
}

