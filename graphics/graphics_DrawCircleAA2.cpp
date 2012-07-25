
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
	double pos;	// 円の端までの距離。
	double area;	// 円の面積の総和。2点間の差で面積を区間の面積を求められるので、エッジの面積を算出可能
};

std::vector<CircleLine> circleLines; // √2/2 * 256 でOK (√2 = 1.4142135623730950488016887242097)

#define M_PI 3.14159265358979 /* 円周率 */
#define rad2deg(a) ( (a) / M_PI * 180.0 ) /* rad を deg に */

static const double xylen45deg = sqrt(2.0)/2;	// 円45度位置の中心からの縦横長さ

static
void initTable(int tableRadius)
{
	circleLines.resize(tableRadius * xylen45deg + 3);
	const float radius = 1.0;
	float rr = radius * radius;
	float CIRCLE_AREA = M_PI * rr;
	static const double RADIAN_CIRCUMFERENCE = 2 * M_PI;
	for (int i=0; i<circleLines.size(); ++i) {
		double y = (double)i / tableRadius;
		double yy = y * y;
		double xx = rr - yy;
		double x = sqrt(xx); // y位置における円の接線のx位置
//		double x8 = xylen45deg + (radius - xylen45deg) * (xylen45deg-y) / xylen45deg; // y位置における正8角形の辺の横位置
		double radian = asin(y/radius);
		CircleLine& cl = circleLines[i];
		cl.pos = x;
		double sectorArea = radian / RADIAN_CIRCUMFERENCE * CIRCLE_AREA;
		double triangleArea = x*y/2;
		cl.area = sectorArea + triangleArea;
		double deg = rad2deg(radian);
		double hoge = 0;
	}
}

namespace Graphics {

static const int TABLE_RADIUS = 256;

void drawHalf(
	float cx,
	float cy,
	double diameter,
	pixel_t color,
	double direction
	)
{
	// あるラインの円の縁を描く際に、テーブルの2点間の面積の差で区間の面積が求まる。
	// 区間の面積から長方形部分を引く事によってエッジの面積が求まる。
	// 縁の部分のピクセル占有度を求める際に、エッジの面積と、場合によっては長方形の一部を利用する。
	// 縁は1ピクセルだけの場合もあれば、2ピクセルに跨る場合もある。
	
	double radius = diameter / 2.0;
	double rr = radius * radius;
	double ratioRadius = TABLE_RADIUS / radius;

	CircleLine cl = circleLines[0];
	double xScaled = cl.pos * radius;
	double prevXPlus = cx + xScaled;
	double prevXMinus = cx - xScaled;
	double prevArea = cl.area;
	for (int i=1; i<radius*xylen45deg+1; ++i) {
		double py = cy + i * direction;
		double ty = i * ratioRadius;
		double tyFrac = ty - (int)ty;
		CircleLine cl0 = circleLines[ty];
		CircleLine cl1 = circleLines[ty + 1];
		double pos = cl0.pos + (cl1.pos - cl0.pos) * tyFrac;
		double area = cl0.area + (cl1.area - cl0.area) * tyFrac;
		double xScaled = pos * radius;
		double xMinus = cx - xScaled;
		double xPlus = cx + xScaled;
		double areaDiff = area - prevArea;
		DrawHorizontalLine(xMinus+1, xPlus, py, color);
		// 0～ -45
		// X座標の整数値が異なる場合は横2pixelに跨る。
		if ((int)xPlus != (int)prevXPlus) {
			double xFrac = xPlus - (int)xPlus;
			double prevXFrac = prevXPlus - (int)prevXPlus;
			double xDiff = prevXPlus - xPlus;
			double rightArea = (areaDiff * rr - xScaled) * (prevXFrac / xDiff);
			double leftArea = (areaDiff * rr - xScaled - rightArea);
			double leftRectArea = xFrac;
			leftArea += leftRectArea;
			assert(leftArea <= 1.0);
			assert(rightArea <= 1.0);
			putPixel(xPlus, py, leftArea*255);
			putPixel(xPlus+1, py, rightArea*255);
		}else {
			double remain = areaDiff * rr - ((int)xPlus - cx);
			assert(remain <= 1.0);
			putPixel(xPlus, py, 255*remain);
		}
		// 180～225
		if ((int)xMinus != (int)prevXMinus) {
			double xFrac = 1.0 - (xMinus - (int)xMinus);
			double prevXFrac = 1.0 - (prevXMinus - (int)prevXMinus);
			double xDiff = xMinus - prevXMinus;
			double leftArea = (areaDiff * rr - xScaled) * (prevXFrac / xDiff);
			double rightArea = (areaDiff * rr - xScaled - leftArea);
			double rightRectArea = xFrac;
			rightArea += rightRectArea;
			assert(leftArea <= 1.0);
			assert(rightArea <= 1.0);
			putPixel(xMinus-1, py, leftArea*255);
			putPixel(xMinus, py, rightArea*255);
		}else {
			double remain = areaDiff * rr - (cx - (int)(xMinus+1.0));
			assert(remain <= 1.0);
			putPixel(xMinus, py, 255*remain);
		}
		prevArea = area;
		prevXPlus = xPlus;
		prevXMinus = xMinus;
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


