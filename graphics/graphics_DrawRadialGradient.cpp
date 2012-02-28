
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
	const ClippingRect& clippingRect,
	const uint16_t* distanceTable, uint8_t distanceTableShifts,
	const pixel_t* pixelTable, uint8_t pixelTableShits,
	const pixel_t* colorTable,
	bool dithering
	)
{
	if (diameter < 2) {
		return;
	}
	const uint16_t radius = diameter / 2;
	const uint32_t radius2 = (diameter * diameter) / 4;
	int32_t sy = cy - radius;
	int32_t ey = sy + diameter;
	int32_t sx = cx - radius;
	int32_t ex = sx + diameter;

	sy = max(sy, (int32_t)clippingRect.y);
	sy = max(sy, 0);
	
	ey = min(ey, clippingRect.y+clippingRect.h);
	ey = min(ey, (int32_t)HEIGHT);
	
	sx = max(sx, (int32_t)clippingRect.x);
	sx = max(sx, 0);

	ex = min(ex, clippingRect.x+clippingRect.w);
	ex = min(ex, (int32_t)WIDTH);
	
	// 中心から描画領域の端までの距離の半径に対しての比率を出す。
	// 中心点からの描画領域の左端の距離と右端の距離のうち、長い方を求める。
	uint16_t dx = std::max(abs(cx - sx), abs(cx - ex));
	uint16_t dy = std::max(abs(cy - sy), abs(cy - ey));
	uint16_t lenRatio2 = radius2 / (dx*dx+dy*dy);
	uint8_t adjustShift1 = 0;
	uint8_t adjustShift2 = 0;
	if (lenRatio2 >= 8*8) {
		adjustShift1 = 8;
		adjustShift2 = 4;
	}else if (lenRatio2 >= 6*6) {
		adjustShift1 = 6;
		adjustShift2 = 3;
	}else if (lenRatio2 >= 4*4) {
		adjustShift1 = 4;
		adjustShift2 = 2;
	}else if (lenRatio2 >= 2*2) {
		adjustShift1 = 2;
		adjustShift2 = 1;
	}
	adjustShift2 += 8;
	const uint32_t invRadius2 = (255ull << (24+adjustShift1)) / radius2;
	// ordered dithering table 4x4
	uint16_t thresholds[4][4] = {
		1,9,3,11,
		13,5,15,7,
		4,12,2,10,
		16,8,14,6,
	};
	uint16_t* pt = (uint16_t*)thresholds;
	for (uint8_t i=0; i<16; ++i) {
		pt[i] <<= adjustShift2 - 3;
	}
	
	const uint8_t shiftBits = (32 - distanceTableShifts);
	const uint16_t mask = (1<<distanceTableShifts) - 1;
	for (uint16_t y=sy; y<=ey; ++y) {
		const int16_t dy = cy - y;
		const uint32_t dy2 = dy * dy;
		const uint32_t dy2a = dy2 * invRadius2;
		
		int16_t dx = sqrt((double)radius2 - dy2);// + 0.5;
		const int16_t sx2 = max<int16_t>(sx, cx-dx);
		const int16_t ex2 = min<int16_t>(ex, cx+dx);
		
		dx = cx - sx2;
		int32_t xs = dx * dx * invRadius2;
		int32_t d1 = (dx * -2 + 1) * invRadius2;
		int16_t x = sx2;
		uint32_t sum = dy2a + xs;
		
		// TODO: 縁の描画はピクセル占有率等も考慮して行う
		
		if (dithering) {
			const uint16_t* pThreshold = thresholds[y&3u];
			for (; x<=ex2; ++x) {
				uint32_t alpha = (distanceTable[sum >> shiftBits] + pThreshold[x&3u]) >> adjustShift2;
				Graphics::pixel_t pixel = colorTable[alpha];
				Graphics::PutPixel(x, y, pixel);
				sum += d1;
				d1 += 2 * invRadius2;
			}
		}else {
			for (; x<=ex2; ++x) {
				uint32_t alpha = distanceTable[sum >> shiftBits] >> adjustShift2;
				Graphics::pixel_t pixel = colorTable[alpha];
				Graphics::PutPixel(x, y, pixel);
				sum += d1;
				d1 += 2 * invRadius2;
			}
		}
	}
}

} // namespace Graphics
