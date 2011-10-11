
#include "graphics_impl_common.h"

namespace Graphics {

// http://www.softist.com/programming/drawcircle/drawcircle.htm
void drawCircle1(int16_t cx, int16_t cy, uint16_t r, uint32_t color)
{
	#define SHIFT 7
	int xx = r << SHIFT;
	int yy = 0;
	int x = 0;
	int y = 0;
	while (yy < xx) {
		x = xx >> SHIFT;
		y = yy >> SHIFT;
		putPixel(cx+x, cy+y, color);
		putPixel(cx-x, cy-y, color);
		putPixel(cx-x, cy+y, color);
		putPixel(cx+x, cy-y, color);
		putPixel(cx+y, cy+x, color);
		putPixel(cx-y, cy-x, color);
		putPixel(cx-y, cy+x, color);
		putPixel(cx+y, cy-x, color);
		yy += xx >> SHIFT;
		xx -= yy >> SHIFT;
	}
}

// http://fussy.web.fc2.com/algo/algo2-1.htm
void drawCircle2(int16_t cx, int16_t cy, uint16_t r, uint32_t color)
{
	int x = r;
	int y = 0;
	int F = -2 * r + 3;
	while (x >= y) {
		putPixel(cx + x, cy + y, color);
		putPixel(cx - x, cy + y, color);
		putPixel(cx + x, cy - y, color);
		putPixel(cx - x, cy - y, color);
		putPixel(cx + y, cy + x, color);
		putPixel(cx - y, cy + x, color);
		putPixel(cx + y, cy - x, color);
		putPixel(cx - y, cy - x, color);
		if (F >= 0) {
			x--;
			F -= 4 * x;
		}
		y++;
		F += 4 * y + 2;
	}
}

// http://d.hatena.ne.jp/zariganitosh/20100318/1269006632

void drawCircle3(int16_t cx, int16_t cy, uint16_t r, uint32_t color)
{
	int x = r;
	int y = 0;
	int f = -2 * r + 2;
	while (x > y) {
		if (f > -x) {
			f += -2 * x + 1;
			--x;
		}
		f += 2 * y + 1;
		++y;

	}
}


void DrawCircle(int16_t cx, int16_t cy, uint16_t r, uint32_t color)
{
	drawCircle2(cx, cy, r, color);
}

} // namespace Graphics


