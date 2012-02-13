#include "graphics_impl_common.h"

namespace Graphics {

uint32_t tmpBuff[WIDTH][3];

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

template <int SHIFTNUM>
uint16_t orderedDither(uint16_t x, uint16_t y, int32_t r, int32_t g, int32_t b)
{

#if 0
	static const uint8_t thresholds[2][2] = {
		1,3,
		4,2,
	};
	int32_t t = thresholds[y%2][x%2] - 2;
#elif 1
	static const int8_t thresholds[4][4] = {
		1,9,3,11,
		13,5,15,7,
		4,12,2,10,
		16,8,14,6,
	};
	int32_t t = (thresholds[y%4][x%4] - 8);
#else
	uint32_t t = 0;
#endif
	r += t << (SHIFTNUM - 1);
	g += t << (SHIFTNUM - 2);
	b += t << (SHIFTNUM - 1);
	
	r += 1 << (SHIFTNUM+2);
	g += 1 << (SHIFTNUM+1);
	b += 1 << (SHIFTNUM+2);

	r = max(0, r);
	g = max(0, g);
	b = max(0, b);
	r = min(r, (1<<SHIFTNUM+8)-1);
	g = min(g, (1<<SHIFTNUM+8)-1);
	b = min(b, (1<<SHIFTNUM+8)-1);

	r >>= SHIFTNUM+3;
	g >>= SHIFTNUM+2;
	b >>= SHIFTNUM+3;
	return r | g<<5 | b<<11;
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
	int16_t x1, pixel_t v1,
	int16_t x2, pixel_t v2
	)
{
	if (x2 < x1) {
		swap(x1, x2);
		swap(v1, v2);
	}
	uint32_t r1,g1,b1, r2,g2,b2;
	r1 = getR(v1) << 16;
	g1 = getG(v1) << 16;
	b1 = getB(v1) << 16;
	r2 = getR(v2) << 16;
	g2 = getG(v2) << 16;
	b2 = getB(v2) << 16;
	for (uint16_t x=0; x<=x1; ++x) {
		tmpBuff[x][0] = r1;
		tmpBuff[x][1] = g1;
		tmpBuff[x][2] = b1;
	}
	
	int dx = x2 - x1;
	Color va(r2, g2, b2);
	Color v(r1, g1, b1);
	va -= v;
	va /= dx;
	for (uint16_t x=x1; x<x2; ++x) {
		tmpBuff[x][0] = v.v[0];
		tmpBuff[x][1] = v.v[1];
		tmpBuff[x][2] = v.v[2];
		v += va;
	}
	for (uint16_t x=x2; x<WIDTH; ++x) {
		tmpBuff[x][0] = r2;
		tmpBuff[x][1] = g2;
		tmpBuff[x][2] = b2;
	}
	
	pixel_t* pLine = getPixelPtr(0, 0);
	for (uint16_t y=0; y<HEIGHT; ++y) {
		pixel_t* buffl = pLine;
		for (uint16_t x=0; x<WIDTH; ++x) {
			buffl[x] = orderedDither<16>(x,y,tmpBuff[x][0],tmpBuff[x][1],tmpBuff[x][2]);
		}
		OffsetPtr(pLine, getLineOffset());
	}
}

void linearGradient_Vertical(
	int16_t y1, pixel_t v1,
	int16_t y2, pixel_t v2
	)
{
	if (y2 < y1) {
		swap(y1, y2);
		swap(v1, v2);
	}
	pixel_t* pLine = getPixelPtr(0, 0);
	uint32_t r1,g1,b1, r2,g2,b2;
	r1 = getR(v1) << 16;
	g1 = getG(v1) << 16;
	b1 = getB(v1) << 16;
	r2 = getR(v2) << 16;
	g2 = getG(v2) << 16;
	b2 = getB(v2) << 16;
	for (uint16_t y=0; y<y1; ++y) {
		for (uint16_t x=0; x<WIDTH; ++x) pLine[x] = orderedDither<16>(x,y,r1,g1,b1);
		OffsetPtr(pLine, getLineOffset());
	}
	
	int dy = y2 - y1;
	Color va(r2, g2, b2);
	Color v(r1, g1, b1);
	va -= v;
	va /= dy;
	int16_t ye = min(y2, (int16_t)HEIGHT);
	for (uint16_t y=y1; y<ye; ++y) {
		for (uint16_t x=0; x<WIDTH; ++x) {
			pLine[x] = orderedDither<16>(x,y,v.v[0],v.v[1],v.v[2]);
		}
		OffsetPtr(pLine, getLineOffset());
		v += va;
	}
	
	for (uint16_t y=ye; y<HEIGHT; ++y) {
		for (uint16_t x=0; x<WIDTH; ++x) pLine[x] = orderedDither<16>(x,y,r2,g2,b2);
		OffsetPtr(pLine, getLineOffset());
	}
}

void linearGradient(
	int16_t x1, int16_t y1, pixel_t v1,
	int16_t x2, int16_t y2, pixel_t v2
	)
{
	if (x2 < x1) {
		swap(x1, x2);
		swap(y1, y2);
		swap(v1, v2);
	}
	uint32_t r1,g1,b1, r2,g2,b2;
	r1 = getR(v1) << 16;
	g1 = getG(v1) << 16;
	b1 = getB(v1) << 16;
	r2 = getR(v2) << 16;
	g2 = getG(v2) << 16;
	b2 = getB(v2) << 16;
	int a = x2 - x1;
	int b = y2 - y1;
	int c1 = a * x1 + b * y1;
	int c2 = a * x2 + b * y2;
	Color ov1(v1);
	Color ov2(v2);
	Color vc = ov1 * c2 - ov2 * c1;
	Color dv = ov2 - ov1;
	int dc = c2 - c1;
	int idc = 0x3FFFFF / dc;
	Color adv = dv * a;
	assert(c2 >= c1);
	int c0 = 0;
	Color bdv = dv * b;
	Color cdv = vc;
	pixel_t* pLine = getPixelPtr(0, 0);

	for (uint16_t y=0; y<HEIGHT; ++y) {
		Color cdv2 = cdv;
		uint16_t x = 0;
		int16_t x1e = min( (c1 - c0) / a, (int)WIDTH );
		int16_t x2e = min( WIDTH - (c0+a*WIDTH - c2) / a, (int)WIDTH);
		if (x1e > 0) {
			for (; x<x1e; ++x) {
				pLine[x] = orderedDither<16>(x,y,r1,g1,b1);
			}
			cdv2 += adv * (x1e+1);
		}
		for (; x<x2e; ++x) {
			Color n = cdv2 * idc;
			pLine[x] = orderedDither<22>(x,y,n.v[0],n.v[1],n.v[2]);
			cdv2 += adv;
		}
		for (; x<WIDTH; ++x) {
			pLine[x] = orderedDither<16>(x,y,r2,g2,b2);
		}
		OffsetPtr(pLine, getLineOffset());
		c0 += b;
		cdv += bdv;
	}
}

} // namespace anonymous

namespace Graphics {

void DrawLinearGradient(int16_t x1, int16_t y1, pixel_t v1, int16_t x2, int16_t y2, pixel_t v2)
{
	if (x1 == x2) {
		linearGradient_Vertical(y1, v1, y2, v2);
	}else if (y1 == y2) {
		linearGradient_Horizontal(x1, v1, x2, v2);
	}else {
		linearGradient(x1, y1, v1, x2, y2, v2);
	}
}

} // namespace Graphics
