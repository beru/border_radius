
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

void DrawRadialGradient(
	int16_t cx, int16_t cy, uint16_t diameter,
	const uint8_t* table, uint8_t nShifts
	)
{
	const uint16_t radius = diameter / 2;
	const uint32_t radius2 = (diameter * diameter) / 4;
	int16_t sy = cy - radius;
	int16_t ey = sy + diameter;
	int16_t sx = cx - radius;
	int16_t ex = sx + diameter;
	sy = std::max<int>(0, sy);
	ey = std::min<int>(ey, HEIGHT);
	sx = std::max<int>(0, sx);
	ex = std::min<int>(ex, WIDTH);
	const int16_t a = cx - sx;

	// 中心から一番遠い画面の端までの距離の半径に対しての比率を出す。
	// 中心点からの画面の左端の距離と右端の距離のうち、長い方を求める。
	uint16_t dx = std::max(abs(cx - sx), abs(cx - ex));
	uint16_t dy = std::max(abs(cy - sy), abs(cy - ey));
	double lenRatio = radius / sqrt((double)dx*dx+dy*dy);
	uint8_t adjustShift1 = 0;
	uint8_t adjustShift2 = 0;
	if (lenRatio > 6.0) { // こんくらいで勘弁してやる
		adjustShift1 = 6;
		adjustShift2 = 3;
	}else if (lenRatio > 4.0) {
		adjustShift1 = 4;
		adjustShift2 = 2;
	}if (lenRatio > 2.0) {
		adjustShift1 = 2;
		adjustShift2 = 1;
	}
	const uint32_t invRadius2 = (255ull << (24+adjustShift1)) / radius2;
	const int a2 = a * a * invRadius2;
	const int initialD = (-2 * a + 1) * invRadius2;

	const uint8_t shiftBits = 32 - nShifts;
	const uint16_t mask = (1<<nShifts) - 1;
	for (uint16_t y=sy; y<=ey; ++y) {
		const int16_t dy = cy - y;
		const uint32_t dy2 = dy * dy;
		const uint32_t dy2a = dy2 * invRadius2;
		int xs = a2;
		int d1 = initialD;
		for (uint16_t x=sx; x<=ex; ++x) {
			int alpha = dy2a+xs;
			alpha >>= shiftBits;
			uint16_t idx = alpha & mask;
			alpha = table[idx] >> adjustShift2;
			Graphics::pixel_t pixel = Graphics::MakePixel(alpha,alpha,alpha,alpha);
			Graphics::PutPixel(x, y, pixel);
			
			xs += d1;
			d1 += 2 * invRadius2;
		}
	}
}

} // namespace Graphics
