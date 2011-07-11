#include "graphics_impl_common.h"

uint16_t global_lcd_framebuffer[320*240];

void PutPixel(uint16_t x, uint16_t y, uint32_t color)
{
	*getPixelPtr(x, y) = to16BitColor(color);
}

void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color)
{
	uint16_t* ptr = getPixelPtr(x, y);
	uint16_t sc = to16BitColor(color);
	for (uint16_t y=0; y<h; ++y) {
		for (uint16_t x=0; x<w; ++x) {
			ptr[x] = sc;
		}
		OffsetPtr(ptr, getLineOffset());
	}
}

void DrawHorizontalLine(uint16_t x1, uint16_t x2, uint16_t y, uint32_t color)
{
	if (x1 > x2) {
		uint16_t tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	uint16_t* ptr = getPixelPtr(x1, y);
	uint16_t sc = to16BitColor(color);
	for (uint16_t x=x1; x<x2; ++x) {
		*ptr = sc;
		++ptr;
	}
}

void DrawVerticalLine(uint16_t x, uint16_t y1, uint16_t y2, uint32_t color)
{
	if (y1 > y2) {
		uint16_t tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	uint16_t* ptr = getPixelPtr(x, y1);
	uint16_t sc = to16BitColor(color);
	for (uint16_t y=y1; y<y2; ++y) {
		*ptr = sc;
		OffsetPtr(ptr, getLineOffset());
	}
}

void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color)
{
	if (x1 == x2) {
		DrawVerticalLine(x1, y1, y2, color);
	}else if (y1 == y2) {
		DrawHorizontalLine(x1, x2, y2, color);
	}else {
		uint16_t dx = abs(x2 - x1);
		uint16_t dy = abs(y2 - y1);
		int sx = (x1 < x2) ? 1 : -1;
		int sy = (y1 < y2) ? 1 : -1;
		uint16_t* ptr = getPixelPtr(x1, y1);
		sy *= getLineOffset();
		int dx2 = dx * 2;
		int dy2 = dy * 2;
		int E;
		uint16_t sc = to16BitColor(color);
		// ŒX‚«‚ª1ˆÈ‰º‚Ìê‡
		if (dy <= dx) {
			E = -dx;
			for (size_t i=0; i<=dx; ++i) {
				*ptr = sc;
				ptr += sx;
				E += dy2;
				if (0 <= E) {
					OffsetPtr(ptr, sy);
					E -= dx2;
				}
			}
		// ŒX‚«‚ª1‚æ‚è‘å‚«‚¢ê‡
		}else {
			E = -dy;
			for (size_t i=0; i<=dy; ++i) {
				*ptr = sc;
				OffsetPtr(ptr, sy);
				E += dx2;
				if (0 <= E) {
					ptr += sx;
					E -= dy2;
				}
			}
		}
	}
}

