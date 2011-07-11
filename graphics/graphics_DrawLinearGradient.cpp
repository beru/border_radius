#include "graphics_impl_common.h"

namespace {

uint32_t tmpBuff[WIDTH];

uint8_t getR(uint32_t c)
{
	return c & 0xFF;
}

uint8_t getG(uint32_t c)
{
	return (c >> 8) & 0xFF;
}

uint8_t getB(uint32_t c)
{
	return c >> 16;
}

uint32_t orderedDither(uint16_t x, uint16_t y, uint32_t c)
{
//	return c;
	static const uint8_t thresholds[2][2] = {
		1,3,
		4,2,
	};
	uint16_t r,g,b;
	uint8_t amp = thresholds[y%2][x%2];
	r = min(0xFF, getR(c)+amp) & 0xFC;
	g = min(0xFF, getG(c)+amp) & 0xFC;
	b = min(0xFF, getB(c)+amp) & 0xFC;
	return r | g<<8 | b<<16;
}

struct Color
{
	Color()
	{
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
	}
	
	Color(int v0, int v1, int v2)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}
	
	Color(uint32_t c)
	{
		v[0] = getR(c);
		v[1] = getG(c);
		v[2] = getB(c);
	}
	
	Color& operator -= (const Color& c) {
		v[0] -= c.v[0];
		v[1] -= c.v[1];
		v[2] -= c.v[2];
		return *this;
	}
	Color operator - (const Color& c) const {
		Color a = *this;
		return a -= c;
	}
	Color& operator += (const Color& c) {
		v[0] += c.v[0];
		v[1] += c.v[1];
		v[2] += c.v[2];
		return *this;
	}
	Color operator + (const Color& c) const {
		Color a = *this;
		return a += c;
	}
	Color& operator *= (int a) {
		v[0] *= a;
		v[1] *= a;
		v[2] *= a;
		return *this;
	}
	Color operator * (int a) const {
		Color t = *this;
		return t *= a;
	}

	Color& operator <<= (int s) {
		v[0] <<= s;
		v[1] <<= s;
		v[2] <<= s;
		return *this;
	}
	Color& operator >>= (int s) {
		v[0] >>= s;
		v[1] >>= s;
		v[2] >>= s;
		return *this;
	}
	Color operator >> (int s) const {
		Color c = *this;
		return c >>= s;
	}
		
	Color& operator /= (int denominator) {
		v[0] /= denominator;
		v[1] /= denominator;
		v[2] /= denominator;
		return *this;
	}
	Color operator / (int denominator) const {
		Color c = *this;
		return c /= denominator;
	}

	operator uint32_t () {

	}
	
	int v[3];
};


void linearGradient_Horizontal(
	int16_t x1, uint32_t v1,
	int16_t x2, uint32_t v2
	)
{
	if (x2 < x1) {
		swap(x1, x2);
		swap(v1, v2);
	}
	uint32_t* tmpl = tmpBuff;
	for (uint16_t x=0; x<=x1; ++x) {
		tmpl[x] = v1;
	}
	
	int dx = x2 - x1;
	Color va(getR(v2), getG(v2), getB(v2));
	Color v(getR(v1), getG(v1), getB(v1));
	va -= v;
	va <<= 16;
	va /= dx;
	v <<= 16;
	for (uint16_t x=x1; x<x2; ++x) {
		uint8_t r,g,b;
		r = v.v[0] >> 16;
		g = v.v[1] >> 16;
		b = v.v[2] >> 16;
		tmpl[x] = r | g<<8 | b<<16;
		v += va;
	}
	for (uint16_t x=x2; x<WIDTH; ++x) {
		tmpl[x] = v2;
	}
	
	uint16_t* pLine = getPixelPtr(0, 0);
	for (uint16_t y=0; y<HEIGHT; ++y) {
		uint16_t* buffl = pLine;
		for (uint16_t x=0; x<WIDTH; ++x) {
			buffl[x] = to16BitColor(orderedDither(x,y,tmpl[x]));
//			buffl[x] = tmpl[x];
		}
		OffsetPtr(pLine, getLineOffset());
	}
}

void linearGradient_Vertical(
	int16_t y1, uint32_t v1,
	int16_t y2, uint32_t v2
	)
{
	if (y2 < y1) {
		swap(y1, y2);
		swap(v1, v2);
	}
	uint16_t* pLine = getPixelPtr(0, 0);
	for (uint16_t y=0; y<y1; ++y) {
		for (uint16_t x=0; x<WIDTH; ++x) pLine[x] = to16BitColor(orderedDither(x,y,v1));
		OffsetPtr(pLine, getLineOffset());
	}
	
	int dy = y2 - y1;
	Color va(getR(v2), getG(v2), getB(v2));
	Color v(getR(v1), getG(v1), getB(v1));
	va -= v;
	va <<= 16;
	va /= dy;
	v <<= 16;
	int16_t ye = min(y2, (int16_t)HEIGHT);
	for (uint16_t y=y1; y<ye; ++y) {
		uint8_t r,g,b;
		r = v.v[0] >> 16;
		g = v.v[1] >> 16;
		b = v.v[2] >> 16;
		uint32_t c = r | g<<8 | b<<16;
		for (uint16_t x=0; x<WIDTH; ++x) {
			pLine[x] = to16BitColor(orderedDither(x,y,c));
		}
		OffsetPtr(pLine, getLineOffset());
		v += va;
	}
	
	for (uint16_t y=ye; y<HEIGHT; ++y) {
		for (uint16_t x=0; x<WIDTH; ++x) pLine[x] = to16BitColor(orderedDither(x,y,v2));
		OffsetPtr(pLine, getLineOffset());
	}
}

void linearGradient(
	int16_t x1, int16_t y1, uint32_t v1,
	int16_t x2, int16_t y2, uint32_t v2
	)
{
	if (x2 < x1) {
		swap(x1, x2);
		swap(y1, y2);
		swap(v1, v2);
	}
	int16_t a = x2 - x1;
	int16_t b = y2 - y1;
	int c1 = a * x1 + b * y1;
	int c2 = a * x2 + b * y2;
	Color ov1(v1);
	Color ov2(v2);
	Color vc = ov1 * c2 - ov2 * c1;
	Color dv = ov2 - ov1;
	int dc = c2 - c1;
	int idc = 0xffffff / dc;
	Color adv = dv * (int)a;
	assert(c2 >= c1);
	int c0 = 0;
	Color bdv = dv * (int)b;
	Color cdv = vc;
	uint16_t* pLine = getPixelPtr(0, 0);

	for (uint16_t y=0; y<HEIGHT; ++y) {
		Color cdv2 = cdv;
		uint16_t x = 0;
		int16_t x1e = min( (c1 - c0) / a, (int)WIDTH );
		int16_t x2e = min( WIDTH - (c0+a*WIDTH - c2) / a, (int)WIDTH);
		if (x1e > 0) {
			for (; x<x1e; ++x) {
				pLine[x] = to16BitColor(orderedDither(x,y,v1));
			}
			cdv2 += adv * (x1e+1);
		}
		for (; x<x2e-1; ++x) {
			uint8_t r,g,b;
			Color n = cdv2 * idc;
			r = n.v[0] >> 24;
			g = n.v[1] >> 24;
			b = n.v[2] >> 24;
			pLine[x] = to16BitColor(orderedDither(x,y,r | g<<8 | b<<16));
			cdv2 += adv;
		}
		for (; x<WIDTH; ++x) {
			pLine[x] = to16BitColor(orderedDither(x,y,v2));
		}
		OffsetPtr(pLine, getLineOffset());
		c0 += b;
		cdv += bdv;
	}
}

} // namespace anonymous


void DrawLinearGradient(int16_t x1, int16_t y1, uint32_t v1, int16_t x2, int16_t y2, uint32_t v2)
{
	if (x1 == x2) {
		linearGradient_Vertical(y1, v1, y2, v2);
	}else if (y1 == y2) {
		linearGradient_Horizontal(x1, v1, x2, v2);
	}else {
		linearGradient(x1, y1, v1, x2, y2, v2);
	}
}

