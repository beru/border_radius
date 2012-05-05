#ifndef GRAPHICS_IMPL_COMMON_H_INCLUDED__
#define GRAPHICS_IMPL_COMMON_H_INCLUDED__

#include "graphics.h"

namespace Graphics {

static const uint16_t WIDTH = 1920;
static const uint16_t HEIGHT = 1080;

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
T min(T a, T b, T c)
{
	return min(min(a,b),c);
}

#if 0
static inline
int min(int x, int y)
{
	assert(INT_MIN <= (x - y));
	assert((x - y) <= INT_MAX);
	return y + ((x - y) & ((x - y) >> (sizeof(int) * CHAR_BIT - 1))); // min(x, y)
}

static inline
int max(int x, int y)
{
	assert(INT_MIN <= (x - y));
	assert((x - y) <= INT_MAX);
	return x - ((x - y) & ((x - y) >> (sizeof(int) * CHAR_BIT - 1))); // max(x, y)
}
#endif

template <typename T>
static inline
T max(T a, T b)
{
	return a>b ? a : b;
}

template <typename T>
static inline
T max(T a, T b, T c)
{
	return max(max(a,b),c);
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

// http://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
static inline
float InvSqrt(float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x; // get bits for floating value
	i = 0x5f375a86- (i>>1); // gives initial guess y0
	x = *(float*)&i; // convert bits back to float
	x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
	return x;
}

void fillSolidRectangle(int16_t x, int16_t y, uint16_t w, uint16_t h, pixel_t color);

}

#endif // #ifndef GRAPHICS_IMPL_COMMON_H_INCLUDED__
