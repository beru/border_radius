
#include <vector>
#include <math.h>

#include "graphics.h"
#include "graphics_impl_common.h"

/*

�ォ��45�x�̈ʒu�܂�1���C�����ɉ~�̒[�̈ʒu�Ɩʐς��L�^���Ă����B

�Œ菬���_�ł̍œK�����������ł́A�P���v�Z�ŋ��܂镔���������ċL�^���鎖�ɂ���Đ��l�̌��������ď����_�ȉ��̐��x����荂�߂���B
�~�ɓ��ڂ��鐳8�p�`����̍������L�^����B(��16�p�`�̕����ǂ����H)

*/

struct CircleLine
{
	double pos;	// �~�̒[�܂ł̋����B
	double area;	// �~�̖ʐς̑��a�B2�_�Ԃ̍��Ŗʐς���Ԃ̖ʐς����߂���̂ŁA�G�b�W�̖ʐς��Z�o�\
};

std::vector<CircleLine> circleLines; // ��2/2 * 256 ��OK (��2 = 1.4142135623730950488016887242097)

#define M_PI 3.14159265358979 /* �~���� */
#define rad2deg(a) ( (a) / M_PI * 180.0 ) /* rad �� deg �� */

static const double xylen45deg = sqrt(2.0)/2;	// �~45�x�ʒu�̒��S����̏c������

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
		double x = sqrt(xx); // y�ʒu�ɂ�����~�̐ڐ���x�ʒu
//		double x8 = xylen45deg + (radius - xylen45deg) * (xylen45deg-y) / xylen45deg; // y�ʒu�ɂ����鐳8�p�`�̕ӂ̉��ʒu
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
	double radius = diameter / 2.0;
	double rr = radius * radius;
	double ratioRadius = TABLE_RADIUS / radius;

	CircleLine prevCl = circleLines[0];
	double prevXPlus = cx + prevCl.pos * radius;
	double prevXMinus = cx - prevCl.pos * radius;
	for (int i=1; i<radius*xylen45deg+1; ++i) {
		double y0 = cy + i * direction;
		double i2 = i * ratioRadius;
		CircleLine cl0 = circleLines[i2];
		CircleLine cl1 = circleLines[i2 + 1];
		CircleLine cl;
		double i2Frac = i2 - (int)i2;
		cl.pos = cl0.pos + (cl1.pos - cl0.pos) * i2Frac;
		cl.area = cl0.area + (cl1.area - cl0.area) * i2Frac;
		double xScaled = cl.pos * radius;
		double xMinus = cx - xScaled;
		double xPlus = cx + xScaled;
		DrawHorizontalLine(xMinus+1, xPlus, y0, color);
		double areaDiff = cl.area - prevCl.area;
		// 0�` -45
		// X���W�̐����l���قȂ�ꍇ�͉�2pixel�Ɍׂ�B
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
			putPixel(xPlus, y0, leftArea*255);
			putPixel(xPlus+1, y0, rightArea*255);
		}else {
			double remain = areaDiff * rr - ((int)xPlus - cx);
			assert(remain <= 1.0);
			putPixel(xPlus, y0, 255*remain);
		}
		// 180�`225
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
			putPixel(xMinus-1, y0, leftArea*255);
			putPixel(xMinus, y0, rightArea*255);
		}else {
			double remain = areaDiff * rr - (cx - (int)(xMinus+1.0));
			assert(remain <= 1.0);
			putPixel(xMinus, y0, 255*remain);
		}
		prevCl = cl;
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
	
	// ���郉�C���̉~�̉���`���ۂɁA�e�[�u����2�_�Ԃ̖ʐς̍��ŋ�Ԃ̖ʐς����܂�B
	// ��Ԃ̖ʐς��璷���`�������������ɂ���ăG�b�W�̖ʐς����܂�B
	// ���̕����̃s�N�Z����L�x�����߂�ۂɁA�G�b�W�̖ʐςƁA�ꍇ�ɂ���Ă͒����`�̈ꕔ�𗘗p����B
	// ����1�s�N�Z�������̏ꍇ������΁A2�s�N�Z���Ɍׂ�ꍇ������B
	
	// ����
	drawHalf(cx, cy, diameter, color, 1.0);
	// �㑤
	drawHalf(cx, cy+1, diameter, color, -1.0);
}

} // namespace Graphics {


