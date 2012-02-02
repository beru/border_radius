
#include "graphics_impl_common.h"
#include <math.h>
#include <algorithm>

namespace Graphics {

inline
void blendPixel(pixel_t& pixel, uint16_t alpha)
{
	assert(alpha != 0);
	assert(pixel == 0);
	pixel = alpha;
}

// TODO: 左上の面のAlpha情報だけを計算し、後で描画時にそのBlending情報を利用する。
// 左右の反転は読み出しポインタをデクリメントすればOK
// 上下の反転は読み出しラインを変えればOK

// Antialiased, Filled apart from center pixel:
// original : http://mines.lumpylumpy.com/Electronics/Computers/Software/Cpp/Graphics/Bitmap/Drawing/CircleStencil.php
void Disc(
	uint8_t* buff,
	int16_t diameter
	)
{
	if (diameter == 0) {
		return;
	}
	const int32_t radius = diameter / 2;
	assert(radius < 512);
	const int32_t radius2 = (diameter * diameter) / 4;
	const uint16_t threshold = radius2 >> 8;
	const int32_t invRadius2 = (0xFFFF << 8) / radius2;
	static const uint8_t FIXEDPOINT_SHIFTS = 16;
	
	uint16_t y = radius - 1;
	uint8_t* lineYMinus = buff;
	for (; y; --y) {
		const uint16_t y2 = y * y;
		// x = 0
		{
			const uint16_t r2 = y2;
			const int32_t diff = radius2 - r2;
			const uint16_t opacity = (diff * invRadius2) >> FIXEDPOINT_SHIFTS;
			blendPixel(lineYMinus[0], opacity);	// N
		}
		const int32_t remain = radius2 - y2;
		const uint16_t remainRoot = sqrt((double)remain);
		uint16_t ex = std::min(y, remainRoot);
		int32_t diff = remain - (ex-1)*(ex-1);
		while (diff <= threshold) {
			--ex;
			diff = remain - (ex-1)*(ex-1);
		}
		uint32_t multipliedDiff = (remain - 1) * invRadius2;
		const uint32_t diff2Add = 2 * invRadius2; // 変動量が2ずつ増えていく
		uint32_t diff2 = invRadius2 + diff2Add; // 2回目は3
		for (uint16_t x=1; x<ex; ++x) {
			const uint32_t opacity = multipliedDiff >> FIXEDPOINT_SHIFTS;
			multipliedDiff -= diff2;
			diff2 += diff2Add;
			blendPixel(lineYMinus[+x], opacity);	// NNW
		}
		// x == y
		{
			const int32_t r2 = y2 + y2; //x*x + y2;
			const int32_t diff = radius2 - r2;
			if (diff > threshold) {
				const uint16_t opacity = (diff * invRadius2) >> FIXEDPOINT_SHIFTS;
				blendPixel(lineYMinus[+y], opacity);	// NW
			}
		}
//		lineYMinus += y;
		lineYMinus += getLineOffset();
	}
}

void DrawDisc(int16_t cx, int16_t cy, uint16_t diameter, uint32_t color)
{
	
	Disc(getPixelPtr(cx, cy), diameter);
//	
	
}

void DrawGradationCircle(int16_t cx, int16_t cy, uint16_t diameter)
{
	const uint16_t radius = diameter / 2;
	const uint32_t radius2 = (diameter * diameter) / 4;
	const uint64_t invRadius2 = (255ull << 24) / radius2;
	int sy = cy - radius;
	int ey = sy + diameter;
	int sx = cx - radius;
	int ex = sx + diameter;
	sy = std::max<int>(0, sy);
	ey = std::min<int>(ey, HEIGHT);
	sx = std::max<int>(0, sx);
	ex = std::min<int>(ex, WIDTH);
	for (int y=sy; y<=ey; ++y) {
		int16_t dy = cy - y;
		uint32_t dy2 = dy * dy;
		int a = cx - sx;
		int a2 = a * a;
		int d1 = -2 * a + 1;
		int xs = a2;
		for (int x=sx; x<=ex; ++x) {
			Graphics::PutPixel(x, y, 255-std::min<int>(255,(((dy2+xs)*invRadius2)>>24)));
			xs += d1;
			d1 += 2;
		}
	}
}

} // namespace Graphics
