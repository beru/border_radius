
#include <vector>
#include <math.h>

#include "graphics.h"
#include "graphics_impl_common.h"

/*

上から45度の位置まで1ライン毎に円の端の位置と面積を記録していく。

固定小数点版の最適化した実装では、単純計算で求まる部分を除いて記録する事によって数値の桁を下げて小数点以下の精度をより高められる。
円に内接する正8角形からの差分を記録する。(正16角形の方が良いか？)

*/

struct CircleLine
{
	float pos;	// 円の端までの距離。
	float area;	// 円の面積の総和。2点間の差で面積を区間の面積を求められるので、エッジの面積を算出可能
};

std::vector<CircleLine> circleLines; // √2/2 * 256 でOK (√2 = 1.4142135623730950488016887242097)

#define M_PI 3.14159265358979 /* 円周率 */
#define rad2deg(a) ( (a) / M_PI * 180.0 ) /* rad を deg に */

static const float xylen45deg = sqrt(2.0)/2;	// 円45度位置の中心からの縦横長さ

static
void initTable(int tableRadius)
{
	circleLines.resize(tableRadius * xylen45deg + 3);
	const float radius = 1.0;
	float rr = radius * radius;
	float CIRCLE_AREA = M_PI * rr;
	static const float RADIAN_CIRCUMFERENCE = 2 * M_PI;
	for (int i=0; i<circleLines.size(); ++i) {
		float y = (float)i / tableRadius;
		float yy = y * y;
		float xx = rr - yy;
		float x = sqrt(xx); // y位置における円の接線のx位置
		float radian = asin(y/radius);
		CircleLine& cl = circleLines[i];
		cl.pos = x;
		float sectorArea = radian / RADIAN_CIRCUMFERENCE * CIRCLE_AREA;
		float triangleArea = x*y/2;
		cl.area = sectorArea + triangleArea;
		float deg = rad2deg(radian);
		float hoge = 0;
	}
}

namespace Graphics {

static const int TABLE_RADIUS = 256;

void drawHalf(
	float cx,
	float cy,
	float diameter,
	pixel_t color,
	float direction
	)
{
	// あるラインの円の縁を描く際に、テーブルの2点間の面積の差で区間の面積が求まる。
	// 区間の面積から長方形部分を引く事によってエッジの面積が求まる。
	// 縁の部分のピクセル占有度を求める際に、エッジの面積と、場合によっては長方形の一部を利用する。
	// 縁は1ピクセルだけの場合もあれば、2ピクセルに跨る場合もある。
	float radius = diameter / 2.0;
	float rr = radius * radius;
	float ratioRadius = TABLE_RADIUS / radius;
	
	CircleLine cl = circleLines[0];
	float prevPos = cl.pos * radius;
	float prevArea = cl.area;
	float py = cy;
	float ty = 0;
	for (uint32_t i=1; i<radius*xylen45deg; ++i) {
		py += direction;
		ty += ratioRadius;
		float tyFrac = frac(ty);
		CircleLine cl0 = circleLines[ty];
		CircleLine cl1 = circleLines[ty + 1];
		CircleLine clDiff;
		clDiff.pos = cl1.pos - cl0.pos;
		clDiff.area = cl1.area - cl0.area;
		float pos = (cl0.pos + clDiff.pos * tyFrac) * radius;
		float area = (cl0.area + clDiff.area * tyFrac) * rr;
		float areaDiff = area - prevArea;
		float curvedPart = areaDiff - pos;
		float prescaledCurvedPart = curvedPart / (prevPos - pos);
		// X座標の整数値が異なる場合は横2pixelに跨る。
		// 左側 180°～225°
		float xMinus = cx - pos;
		float prevXMinus = cx - prevPos;
		if ((int)xMinus != (int)prevXMinus) {
			float leftArea = prescaledCurvedPart * (1.0f - frac(prevXMinus));
			float rightArea = curvedPart - leftArea;
			float rightRectArea = 1.0f - frac(xMinus);
			rightArea += rightRectArea;
			assert(leftArea <= 1.0f);
			assert(rightArea <= 1.0f);
			putPixel(xMinus-1, py, leftArea*255.0f);
			putPixel(xMinus, py, rightArea*255.0f);
		}else {
			float remain = areaDiff - (cx - (int)(xMinus+1.0f));
			assert(remain <= 1.0);
			putPixel(xMinus, py, 255.0f*remain);
		}
		// 中間線
		float xPlus = cx + pos;
		DrawHorizontalLine(xMinus+1.0f, xPlus, py, color);
		// 右側 0°～ -45°
		float prevXPlus = cx + prevPos;
		if ((int)xPlus != (int)prevXPlus) {
			float rightArea = prescaledCurvedPart * frac(prevXPlus);
			float leftArea = curvedPart - rightArea;
			float leftRectArea = frac(xPlus);
			leftArea += leftRectArea;
			assert(leftArea <= 1.0);
			assert(rightArea <= 1.0);
			putPixel(xPlus, py, leftArea*255.0f);
			putPixel(xPlus+1, py, rightArea*255.0f);
		}else {
			float remain = areaDiff - ((int)xPlus - cx);
			assert(remain <= 1.0);
			putPixel(xPlus, py, 255.0f*remain);
		}
		prevPos = pos;
		prevArea = area;
	}
}

void DrawFilledCircleAA2(float cx, float cy, float diameter, pixel_t color)
{
	static bool inited;
	if (!inited) {
		inited = true;
		initTable(TABLE_RADIUS);
	}
	
	// 下側
	drawHalf(cx, cy, diameter, color, 1.0);
	// 上側
	drawHalf(cx, cy+1, diameter, color, -1.0);
}

} // namespace Graphics {


