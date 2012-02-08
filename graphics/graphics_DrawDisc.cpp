
#include "graphics_impl_common.h"
#include <math.h>
#include <algorithm>

namespace Graphics {

inline
void blendPixel(pixel_t& pixel, uint16_t alpha)
{
	assert(alpha != 0);
	assert(pixel == 0);
	pixel = MakePixel(alpha,alpha,alpha,alpha);
}

void DrawGradationCircle(
	int16_t cx, int16_t cy, uint16_t diameter,
	const uint8_t table[256]
	)
{
	const uint16_t radius = diameter / 2;
	const uint32_t radius2 = (diameter * diameter) / 4;
	const uint32_t invRadius2 = (255ull << 24) / radius2;
	int16_t sy = cy - radius;
	int16_t ey = sy + diameter;
	int16_t sx = cx - radius;
	int16_t ex = sx + diameter;
	sy = std::max<int>(0, sy);
	ey = std::min<int>(ey, HEIGHT);
	sx = std::max<int>(0, sx);
	ex = std::min<int>(ex, WIDTH);
	const int16_t a = cx - sx;
	const int a2 = a * a * invRadius2;
	const int initialD = (-2 * a + 1) * invRadius2;
	uint8_t shifts[640];
	for (uint8_t i=0; i<64; ++i) {
		shifts[i] = 22;
	}
	for (uint8_t i=0; i<16; ++i) {
		shifts[i] = 20;
	}
//	for (uint8_t i=16; i<32; ++i) {
//		shifts[i] = 20;
//	}
	
	uint8_t shifts2[23];
	for (uint8_t i=0; i<23; ++i) {
		shifts2[i] = 0;
	}
	shifts2[16] = 3;
	shifts2[20] = 1;


	for (uint16_t y=sy; y<=ey; ++y) {
		const int16_t dy = cy - y;
		const uint32_t dy2 = dy * dy;
		const uint32_t dy2a = dy2 * invRadius2;
		int xs = a2;
		int d1 = initialD;
		for (uint16_t x=sx; x<=ex; ++x) {
			int alpha = dy2a+xs;
#if 0
			alpha >>= 22;
			alpha = table[alpha & 0x3ff];
#else
			// TODO: •\ˆø‚«‚·‚é‚Ær‚­‚È‚Á‚Ä‚µ‚Ü‚¤“à‘¤‚¾‚¯•ª‚¯‚Ä•`‚­B
			// ‰~‚Ì‘å‚«‚³‚ªˆê’èˆÈ‰º‚È‚ç•s—v‚©BB
			uint64_t absSum = dy2 + (cx-x)*(cx-x);
			uint8_t shift = shifts[absSum>>14];
			alpha >>= shift;
			alpha = table[alpha & 0x3ff];
			alpha >>= shifts2[shift];
#endif
			Graphics::pixel_t pixel = Graphics::MakePixel(alpha,alpha,alpha,alpha);
			Graphics::PutPixel(x, y, pixel);
			
			xs += d1;
			d1 += 2 * invRadius2;
		}
	}
}

} // namespace Graphics
