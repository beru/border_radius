
#include <vector>
#include <math.h>

#include "graphics.h"

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

static
void initTable(int count)
{
	float xylen45deg = sqrt(2.0)/2;	// �~45�x�ʒu�̒��S����̏c������
	
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
		float x = sqrt(xx); // y�ʒu�ɂ�����~�̐ڐ���x�ʒu
//		float x8 = xylen45deg + (radius - xylen45deg) * (xylen45deg-y) / xylen45deg; // y�ʒu�ɂ����鐳8�p�`�̕ӂ̉��ʒu
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
	
	// ���郉�C���̉~�̉���`���ۂɁA�e�[�u����2�_�Ԃ̖ʐς̍��ŋ�Ԃ̖ʐς����܂�B
	// ��Ԃ̖ʐς��璷���`�������������ɂ���ăG�b�W�̖ʐς����܂�B
	// ���̕����̃s�N�Z����L�x�����߂�ۂɁA�G�b�W�̖ʐςƁA�ꍇ�ɂ���Ă͒����`�̈ꕔ�𗘗p����B
	// ����1�s�N�Z�������̏ꍇ������΁A2�s�N�Z���Ɍׂ�ꍇ������B
	
	
}

} // namespace Graphics {


