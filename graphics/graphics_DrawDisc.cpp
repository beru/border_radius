
#include "graphics_impl_common.h"
#include <math.h>
#include <algorithm>

namespace Graphics {

inline
void blendPixel(pixel_t& pixel, uint16_t alpha)
{
	assert(alpha != 0);
	assert(pixel == 0);
	pixel = makePixel(alpha,alpha,alpha,alpha);
}

void DrawGradationCircle(int16_t cx, int16_t cy, uint16_t diameter)
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
	const int a = cx - sx;
	const int a2 = a * a;
	const int initialD = -2 * a + 1;
	for (uint16_t y=sy; y<=ey; ++y) {
		int16_t dy = cy - y;
		uint32_t dy2 = dy * dy;
		int xs = a2;
		int d1 = initialD;
		for (uint16_t x=sx; x<=ex; ++x) {
			int alpha = 255-std::min<int>(255,(((dy2+xs)*invRadius2)>>24));
			assert(alpha < 256);
			pixel_t pixel = makePixel(alpha,alpha,alpha,alpha);
			Graphics::PutPixel(x, y, pixel);
			
			xs += d1;
			d1 += 2;
		}
	}
}

} // namespace Graphics
