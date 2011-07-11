#ifndef GRAPHICS_IMPL_COMMON_H_INCLUDED__
#define GRAPHICS_IMPL_COMMON_H_INCLUDED__

#include "graphics.h"

const uint16_t WIDTH = 320;
const uint16_t HEIGHT = 240;

extern uint16_t global_lcd_framebuffer[320*240];

static inline
int getLineOffset()
{
	return 320 * sizeof(uint16_t);
}

static inline
uint16_t* getPixelPtr(uint16_t x, uint16_t y)
{
	uint16_t* ptr = &global_lcd_framebuffer[x];
	OffsetPtr(ptr, getLineOffset()*y);
	return ptr;
}

static inline
uint16_t to16BitColor(uint32_t color)
{
	uint16_t c = 0;
	c |= (color & 0xFF) >> 3;
	c |= ((color & 0xFF00) >> 10) << 5;
	c |= ((color & 0xFF0000) >> 19) << 11;
	return c;
}

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
T abs(T a)
{
	return a<0 ? -a:a;
}

#endif // #ifndef GRAPHICS_IMPL_COMMON_H_INCLUDED__
