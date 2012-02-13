#ifndef GRAPHICS_IMPL_COMMON_H_INCLUDED__
#define GRAPHICS_IMPL_COMMON_H_INCLUDED__

#include "graphics.h"

namespace Graphics {

static const uint16_t WIDTH = 1024;
static const uint16_t HEIGHT = 1024;

extern pixel_t global_lcd_framebuffer[HEIGHT][WIDTH];

static inline
int getLineOffset()
{
	return WIDTH * sizeof(pixel_t);
}

static inline
pixel_t* getPixelPtr(uint16_t x, uint16_t y)
{
	return &global_lcd_framebuffer[y][x];
}

static inline
pixel_t to_pixel_t(uint32_t color)
{
#if 1
#if 1
	return color & 0xFF;
#else
	uint16_t c = 0;
	c |= (color & 0xFF) >> 3;
	c |= ((color & 0xFF00) >> 10) << 5;
	c |= ((color & 0xFF0000) >> 19) << 11;
	return c;
#endif
#else
	return color;
#endif
}

static inline
void putPixel(uint16_t x, uint16_t y, pixel_t color)
{
	*getPixelPtr(x, y) = color;
}

//static inline
//void putPixel(uint16_t x, uint16_t y, uint8_t color)
//{
//	*getPixelPtr(x, y) = to_pixel_t(color);
//}

template <typename T>
static inline
void swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

template <typename T>
static inline
T min(T a, T b)
{
	return a<b ? a : b;
}

template <typename T>
static inline
T max(T a, T b)
{
	return a>b ? a : b;
}

template <typename T>
static inline
T clamp(T v, T minV, T maxV)
{
	return min(max(v,minV), maxV);
}

template <typename T>
static inline
T abs(T a)
{
	return a<0 ? -a:a;
}

void fillSolidRectangle(int16_t x, int16_t y, uint16_t w, uint16_t h, pixel_t color);

}

#endif // #ifndef GRAPHICS_IMPL_COMMON_H_INCLUDED__
