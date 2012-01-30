
#include "graphics_impl_common.h"
#include <math.h>
#include <algorithm>

namespace Graphics {

inline
void blendPixel(pixel_t& pixel, uint32_t color, uint16_t opacity)
{
	assert(opacity != 0);
	assert(pixel == 0);
	pixel = to_pixel_t(opacity | (opacity<<8) | (opacity<<16));
}

// TODO: ¶ã‚Ì–Ê‚ÌAlphaî•ñ‚¾‚¯‚ðŒvŽZ‚µAŒã‚Å•`‰æŽž‚É‚»‚ÌBlendingî•ñ‚ð—˜—p‚·‚éB

// Antialiased, Filled apart from center pixel:
// original : http://mines.lumpylumpy.com/Electronics/Computers/Software/Cpp/Graphics/Bitmap/Drawing/CircleStencil.php
void Disc(int16_t xCenter, int16_t yCenter, int16_t diameter, uint32_t color)
{
	if (diameter == 0) return;
	const int32_t radius = diameter / 2;
	const int32_t radius2 = (diameter * diameter) / 4;
	const uint16_t threshold = radius2 >> 8;
	const int32_t invRadius2 = (0xFFFF << 8) / radius2;
	static const uint8_t FIXEDPOINT_SHIFTS = 16;
	
	uint16_t y = radius - 1;
	pixel_t* lineYMinus = getPixelPtr(xCenter, yCenter-y -1);
	pixel_t* lineYCenter = getPixelPtr(xCenter, yCenter);
	pixel_t* lineYPlus = getPixelPtr(xCenter, yCenter+y +1);
	const int lineOffset = getLineOffset();
	for (; y; --y) {
		const uint16_t y2 = y * y;
		OffsetPtr(lineYMinus, lineOffset);
		OffsetPtr(lineYPlus, -lineOffset);
		// x = 0
		{
			const uint16_t r2 = y2;
			const int32_t diff = radius2 - r2;
			if (diff <= threshold) {
				continue;
			}
			const uint16_t opacity = (diff * invRadius2) >> FIXEDPOINT_SHIFTS;
			blendPixel(lineYMinus[0], color, opacity);
			blendPixel(lineYCenter[-y], color, opacity);
//			blendPixel(lineYCenter[+y], color, opacity);
//			blendPixel(lineYPlus[0], color, opacity);
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
		const uint32_t diff2Add = 2 * invRadius2; // •Ï“®—Ê‚ª2‚¸‚Â‘‚¦‚Ä‚¢‚­
		uint32_t diff2 = invRadius2 + diff2Add; // 2‰ñ–Ú‚Í3
		pixel_t* lineXMinus = lineYCenter;
		pixel_t* lineXPlus = lineYCenter;
		for (uint16_t x=1; x<ex; ++x) {
			const uint32_t opacity = multipliedDiff >> FIXEDPOINT_SHIFTS;
			multipliedDiff -= diff2;
			diff2 += diff2Add;
			blendPixel(lineYMinus[-x], color, opacity);
//			blendPixel(lineYMinus[+x], color, opacity);
//			blendPixel(lineYPlus[-x], color, opacity);
//			blendPixel(lineYPlus[+x], color, opacity);
			OffsetPtr(lineXMinus, -lineOffset);
			OffsetPtr(lineXPlus, +lineOffset);
			blendPixel(lineXMinus[-y], color, opacity);
//			blendPixel(lineXMinus[+y], color, opacity);
//			blendPixel(lineXPlus[-y], color, opacity);
//			blendPixel(lineXPlus[+y], color, opacity);
		}
		// x == y
		{
			const int32_t r2 = y2 + y2; //x*x + y2;
			const int32_t diff = radius2 - r2;
			if (diff <= threshold) {
				continue;
			}
			const uint16_t opacity = (diff * invRadius2) >> FIXEDPOINT_SHIFTS;
			blendPixel(lineYMinus[-y], color, opacity);	// upper left
//			blendPixel(lineYMinus[+y], color, opacity); // upper right
//			blendPixel(lineYPlus[-y], color, opacity); // bottom left
//			blendPixel(lineYPlus[+y], color, opacity); // bottom right
		}
	}
}

void DrawDisc(int16_t cx, int16_t cy, uint16_t diameter, uint32_t color)
{
	Disc(cx, cy, diameter, color);
//	
	
}


} // namespace Graphics
