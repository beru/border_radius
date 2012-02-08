
#include <stdio.h>

#include "graphics/graphics.h"
#include "timer.h"

#include <math.h>

int main(int argc, char* argv[])
{
	Graphics::BorderStyle bs;
	bs.width = 2;
	bs.color = 0x2FFFAF;
	
//	Graphics::DrawLinearGradient(110,10,0xAAAA00, 300,200,0x00AAAA);
#if 0
	for (int i=0; i<20; ++i) {
		Graphics::DrawRectangle(200,100-10*i, 100,100, 0x4F6F9F, bs);
	}
#endif
	
#if 0
	for (uint16_t i=1; i<240; i+=4) {
		Graphics::DrawCircle(120, 120, i, -1);
	}
#endif
//	Graphics::DrawFilledCircle(120, 120, 100, -1);
	
	uint8_t table[1024];
	for (int i=0; i<sizeof(table); ++i) {
		uint16_t alpha = sqrt((double)(i<<6));
//		alpha >>= 8;
//		assert(alpha < 256);
//		Graphics::pixel_t pixel = Graphics::MakePixel(alpha,alpha,alpha,alpha);
		table[i] = alpha;
	}
	
	Timer t;
	//for (int i=0; i<1; ++i) {
	//	Graphics::DrawDisc(240, 240, 480, -1);
	//}
	
	Graphics::DrawGradationCircle(200,200,1024*8, table);
	
	printf("%f\n", t.ElapsedSecond()*1000);
	return 0;
}

