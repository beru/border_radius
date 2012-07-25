
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
	float pos;	// �~�̒[�܂ł̋����B
	float area;	// �~�̖ʐς̑��a�B2�_�Ԃ̍��Ŗʐς���Ԃ̖ʐς����߂���̂ŁA�G�b�W�̖ʐς��Z�o�\
};

std::vector<CircleLine> circleLines; // ��2/2 * 256 ��OK (��2 = 1.4142135623730950488016887242097)

#define M_PI 3.14159265358979 /* �~���� */
#define rad2deg(a) ( (a) / M_PI * 180.0 ) /* rad �� deg �� */

static const float xylen45deg = sqrt(2.0)/2;	// �~45�x�ʒu�̒��S����̏c������

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
		float x = sqrt(xx); // y�ʒu�ɂ�����~�̐ڐ���x�ʒu
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
	// ���郉�C���̉~�̉���`���ۂɁA�e�[�u����2�_�Ԃ̖ʐς̍��ŋ�Ԃ̖ʐς����܂�B
	// ��Ԃ̖ʐς��璷���`�������������ɂ���ăG�b�W�̖ʐς����܂�B
	// ���̕����̃s�N�Z����L�x�����߂�ۂɁA�G�b�W�̖ʐςƁA�ꍇ�ɂ���Ă͒����`�̈ꕔ�𗘗p����B
	// ����1�s�N�Z�������̏ꍇ������΁A2�s�N�Z���Ɍׂ�ꍇ������B
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
		// X���W�̐����l���قȂ�ꍇ�͉�2pixel�Ɍׂ�B
		// ���� 180���`225��
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
		// ���Ԑ�
		float xPlus = cx + pos;
		DrawHorizontalLine(xMinus+1.0f, xPlus, py, color);
		// �E�� 0���` -45��
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
	
	// ����
	drawHalf(cx, cy, diameter, color, 1.0);
	// �㑤
	drawHalf(cx, cy+1, diameter, color, -1.0);
}

} // namespace Graphics {


