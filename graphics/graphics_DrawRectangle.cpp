
#include "graphics_impl_common.h"

void DrawRectangle(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t color, const BorderStyle& border)
{
	// 領域チェック
	if (w == 0 || h == 0) {
		return;
	}
	int16_t x2 = x;
	int16_t y2 = y;
	uint16_t w2 = w;
	uint16_t h2 = h;
	// 表示範囲外チェック
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
	
	uint16_t* dest = &global_lcd_framebuffer[WIDTH*y2+x2];
	uint16_t* line = dest;
	uint32_t c16 = to16BitColor(color);
	// まず背景を描画する
	for (uint16_t iy=0; iy<h2; ++iy) {
		for (uint16_t ix=0; ix<w2; ++ix) {
			line[ix] = c16;
		}
		OffsetPtr(line, getLineOffset());
	}
	
	// 次に枠線を描画する
	if (border.width == 0) {
		return;
	}
	uint32_t bc16 = to16BitColor(border.color);
	if (bc16 == c16) {
		return;
	}
	int xOffset = x2 - x;
	int yOffset = y2 - y;
	// top
	line = dest;
	for (uint16_t iy=0; iy<border.width - yOffset; ++iy) {
		for (uint16_t ix=0; ix<w2; ++ix) {
			line[ix] = bc16;
		}
		OffsetPtr(line, getLineOffset());
	}
	// left
	if (border.width > xOffset) {
		line = dest;
		OffsetPtr(line, getLineOffset()*(border.width - yOffset));
		for (uint16_t iy=0; iy<h2-border.width*2; ++iy) {
			for (uint16_t ix=0; ix<border.width-xOffset; ++ix) {
				line[ix] = bc16;
			}
			OffsetPtr(line, getLineOffset());
		}
	}
	// right
	line = dest;
	OffsetPtr(line, getLineOffset()*(border.width - yOffset));
	for (uint16_t iy=0; iy<h2-border.width*2; ++iy) {
		for (uint16_t ix=w2-border.width; ix<w2; ++ix) {
			line[ix] = bc16;
		}
		OffsetPtr(line, getLineOffset());
	}
	// bottom
	for (uint16_t iy=0; iy<border.width; ++iy) {
		for (uint16_t ix=0; ix<w2; ++ix) {
			line[ix] = bc16;
		}
		OffsetPtr(line, getLineOffset());
	}
	
}

