
#include "graphics_impl_common.h"

// �P�F�h��Ԃ�����
void fillSolidRectangle(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t color)
{
	// �̈�`�F�b�N
	if (w == 0 || h == 0) {
		return;
	}
	int16_t x2 = x;
	int16_t y2 = y;
	uint16_t w2 = w;
	uint16_t h2 = h;
	// �\���͈͊O�`�F�b�N
	if (x < 0) {
		if (-x >= w) {
			return;
		}
		x2 = 0;
		w2 += x;
	}
	if (y < 0) {
		if (-y >= h) {
			return;
		}
		y2 = 0;
		h2 += y;
	}

	if (x2 >= WIDTH || y2>= HEIGHT) {
		return;
	}
	if (x2 + w2 >= WIDTH) {
		w2 = WIDTH - x2;
	}
	if (y2 + h2 >= HEIGHT) {
		h2 = HEIGHT - y2;
	}
	
	pixel_t* ptr = getPixelPtr(x2, y2);
	pixel_t sc = to_pixel_t(color);
	for (uint16_t iy=0; iy<h2; ++iy) {
		for (uint16_t ix=0; ix<w2; ++ix) {
			ptr[ix] = sc;
		}
		OffsetPtr(ptr, getLineOffset());
	}
}


