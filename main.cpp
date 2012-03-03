
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
	
	static const uint8_t NSHIFTS = 12;
	uint16_t distanceTable[1<<NSHIFTS];
	for (int i=0; i<countof(distanceTable); ++i) {
		uint16_t alpha = sqrt((double)((uint64_t)i<<(32-NSHIFTS)));
		distanceTable[i] = alpha;
	}

#if 0
	Graphics::pixel_t colorTable[256+1];
	for (int i=0; i<256; ++i) {
		uint8_t v = 255 - i;
		colorTable[i] = Graphics::MakePixel(v,v,v,v);
	}
#else
	Graphics::pixel_t colorTable[1<<NSHIFTS];
	for (int i=0; i<countof(colorTable); ++i) {
		uint8_t v = 255 - (distanceTable[i]>>8);
		colorTable[i] = Graphics::MakePixel(v,v,v,v);
	}
#endif
	
	//for (int i=0; i<1; ++i) {
	//	Graphics::DrawDisc(240, 240, 480, -1);
	//}
	Graphics::ClippingRect clippingRect;
	clippingRect.x = 0;
	clippingRect.y = 0;
	clippingRect.w = 1920;
	clippingRect.h = 1080;
	
	Timer t;
	
#if 1
	for (int i=0; i<1; ++i) {
		Graphics::DrawRadialGradient(
			1024,512, 512*4, clippingRect,
			distanceTable, NSHIFTS,
			0, 0,
			colorTable,
			false
			);
	}
#else
	Graphics::DrawLinearGradient(
		0,0, Graphics::MakePixel(0,50,0,0),
		1024,0, Graphics::MakePixel(0,10,0,0),
		clippingRect,
		false
		);
#endif
	printf("%f\n", t.ElapsedSecond()*1000);
	
	return 0;
}

