#include "graphics_impl_common.h"
#include <math.h>
#include <algorithm>

#include <vector>

namespace {

float lengths[1024];
float areas[1024];
double sum = 0;
double deg45Len = sqrt(2.0) / 2;

// 45度までを同じ高さで等分した際の、長さと面積を算出して表に入れる。
void calcTable()
{
	for (int i=1; i<=countof(lengths); ++i) {
		double y = (deg45Len * i) / countof(lengths);
		double x = 1 - sqrt(1 - y*y);
		sum += x;
		lengths[i-1] = x;
		areas[i-1] = sum;
	}
}

} // namespace

namespace Graphics {

void DrawFilledCircleAA(float cx, float cy, float diameter, pixel_t color)
{
	float radius = diameter / 2.0;
	float radius2 = radius * radius;
	float ty = cy - radius;
	float by = cy + radius;
	float lx = cx - radius;
	float rx = cx + radius;
	
	int ity = floor(ty);
	int iby = ceil(by);
	int ilx = floor(lx);
	int irx = ceil(rx);
	
	// 円の中央の横線の上側の画素境界との接点を求める
	float y0 = floor(cy);
	float dy = cy - y0;
	float dy2 = dy * dy;
	float dx = sqrt(radius2 - dy2);	// 中央との距離
	float lx0 = cx - dx;			// 絶対座標に変換
	float rx0 = cx + dx;			// 円の対象性
	// 円の中央の横線の下側の画素境界との接点を求める
	float y1 = y0 + 1.0;
	dy += 1.0;
	dy2 = dy * dy;
	dx = sqrt(radius2 - dy2);
	lx = cx - dx;
	rx = cx + dx;
	ilx = floor(lx);
	irx = floor(rx);
	while (y1 <= iby) {
		// 1画素下にずらして、横線の下側の画素境界との接点を求める
		float olx = lx;
		float orx = rx;
		float oilx = ilx;
		float oirx = irx;
		y1 += 1.0;
		dy += 1.0;
		dy2 = dy * dy;
		dx = sqrt(radius2 - dy2);
		lx = cx - dx;
		rx = cx + dx;
		ilx = floor(lx);
		irx = floor(rx);
		
		dx = lx - olx;
		if (ilx - oilx) {
			// 横2画素に渡って全部塗りつぶされていない領域がある
			// 三角形の面積の比率、
			// 長さの比で高さが出せる
			float dx0 = ilx - olx;			// 左側の画素中にはみ出ている三角形の横幅
			float ratio = dx0 / dx;			// 左側の画素中にはみ出ている三角形の比率
			float la = (dx0 * ratio) / 2.0;	// 左側の画素中の三角形の面積
			float ra = (dx / 2) - la + (1.0 - (lx - ilx));	// 右側の画素中の三角形の面積
			putPixel(oilx, y1, la*255);
			putPixel(oilx+1, y1, ra*255);
			DrawHorizontalLine(oilx+2, irx, y1, 255);
		}else {
			// 切り捨てて同じ画素にある場合は横2画素にまたがっていない。
			float a = (ilx + 1.0 - lx) + dx / 2.0;
			putPixel(oilx, y1, a*255);
			DrawHorizontalLine(oilx+1, irx, y1, 255);
		}
		
		dx = orx - rx;
		if (oirx - irx) {
			float dx0 = orx - oirx;			// 右側の画素中にはみ出ている三角形の横幅
			float ratio = dx0 / dx;			// 右側の画素中にはみ出ている三角形の比率
			float ra = (dx0 * ratio) / 2.0;	// 右側の画素中の三角形の面積
			float la = (dx / 2) - ra + (rx - irx);	// 左側の画素中の三角形の面積
			putPixel(irx, y1, la*255);
			putPixel(irx+1, y1, ra*255);
		}else {
			float a = (rx - irx) + dx / 2.0;
			putPixel(irx, y1, a*255);
		}
		
	}
	
}


} // namespace Graphics

