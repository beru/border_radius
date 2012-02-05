
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
	const pixel_t* ctable, uint16_t tlen
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
	for (uint16_t y=sy; y<=ey; ++y) {
		const int16_t dy = cy - y;
		const uint32_t dy2 = dy * dy * invRadius2;
		int xs = a2;
		int d1 = initialD;
		for (uint16_t x=sx; x<=ex; ++x) {
			int alpha = (dy2+xs) >> 24;
			assert(alpha < tlen);
			Graphics::PutPixel(x, y, ctable[alpha]);
			
			xs += d1;
			d1 += 2 * invRadius2;
		}
	}
}

} // namespace Graphics
