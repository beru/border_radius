
#include <vector>
#include <math.h>

#include "graphics.h"

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

static
void initTable(int count)
{
	float xylen45deg = sqrt(2.0)/2;	// 円45度位置の中心からの縦横長さ
	
	circleLines.resize(count * xylen45deg + 3);
	const float radius = 1.0;
	float rr = radius * radius;
	float CIRCLE_AREA = M_PI * rr;
	float areaSum = 0;
	static const double RADIAN_CIRCUMFERENCE = 2 * M_PI;
	for (int i=0; i<circleLines.size(); ++i) {
		float y = (float)i / count;
		float yy = y * y;
		float xx = rr - yy;
		float x = sqrt(xx); // y位置における円の接線のx位置
//		float x8 = xylen45deg + (radius - xylen45deg) * (xylen45deg-y) / xylen45deg; // y位置における正8角形の辺の横位置
		float radian = asin(y/radius);
		CircleLine& cl = circleLines[i];
		cl.pos = x;
		float sectorArea = radian / RADIAN_CIRCUMFERENCE * CIRCLE_AREA;
		float triangleArea = x*y/2;
		float area = sectorArea + triangleArea;
		areaSum += area;
		cl.area = areaSum;
		float deg = rad2deg(radian);
		float hoge = 0;
	}
}

namespace Graphics {

void DrawFilledCircleAA2(float cx, float cy, float diameter, pixel_t color)
{
	static bool inited;
	if (!inited) {
		inited = true;
		initTable(256);
	}
	
	// あるラインの円の縁を描く際に、テーブルの2点間の面積の差で区間の面積が求まる。
	// 区間の面積から長方形部分を引く事によってエッジの面積が求まる。
	// 縁の部分のピクセル占有度を求める際に、エッジの面積と、場合によっては長方形の一部を利用する。
	// 縁は1ピクセルだけの場合もあれば、2ピクセルに跨る場合もある。
	
	
}

} // namespace Graphics {


