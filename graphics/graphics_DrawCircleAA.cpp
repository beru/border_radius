#include "graphics_impl_common.h"
#include <math.h>
#include <algorithm>

#include <vector>

namespace {

float lengths[1024];
float areas[1024];
double sum = 0;
double deg45Len = sqrt(2.0) / 2;

// 45�x�܂ł𓯂������œ��������ۂ́A�����Ɩʐς��Z�o���ĕ\�ɓ����B
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
	
	// �~�̒����̉����̏㑤�̉�f���E�Ƃ̐ړ_�����߂�
	float y0 = floor(cy);
	float dy = cy - y0;
	float dy2 = dy * dy;
	float dx = sqrt(radius2 - dy2);	// �����Ƃ̋���
	float lx0 = cx - dx;			// ��΍��W�ɕϊ�
	float rx0 = cx + dx;			// �~�̑Ώې�
	// �~�̒����̉����̉����̉�f���E�Ƃ̐ړ_�����߂�
	float y1 = y0 + 1.0;
	dy += 1.0;
	dy2 = dy * dy;
	dx = sqrt(radius2 - dy2);
	lx = cx - dx;
	rx = cx + dx;
	ilx = floor(lx);
	irx = floor(rx);
	while (y1 <= iby) {
		// 1��f���ɂ��炵�āA�����̉����̉�f���E�Ƃ̐ړ_�����߂�
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
			// ��2��f�ɓn���đS���h��Ԃ���Ă��Ȃ��̈悪����
			// �O�p�`�̖ʐς̔䗦�A
			// �����̔�ō������o����
			float dx0 = ilx - olx;			// �����̉�f���ɂ͂ݏo�Ă���O�p�`�̉���
			float ratio = dx0 / dx;			// �����̉�f���ɂ͂ݏo�Ă���O�p�`�̔䗦
			float la = (dx0 * ratio) / 2.0;	// �����̉�f���̎O�p�`�̖ʐ�
			float ra = (dx / 2) - la + (1.0 - (lx - ilx));	// �E���̉�f���̎O�p�`�̖ʐ�
			putPixel(oilx, y1, la*255);
			putPixel(oilx+1, y1, ra*255);
			DrawHorizontalLine(oilx+2, irx, y1, 255);
		}else {
			// �؂�̂Ăē�����f�ɂ���ꍇ�͉�2��f�ɂ܂������Ă��Ȃ��B
			float a = (ilx + 1.0 - lx) + dx / 2.0;
			putPixel(oilx, y1, a*255);
			DrawHorizontalLine(oilx+1, irx, y1, 255);
		}
		
		dx = orx - rx;
		if (oirx - irx) {
			float dx0 = orx - oirx;			// �E���̉�f���ɂ͂ݏo�Ă���O�p�`�̉���
			float ratio = dx0 / dx;			// �E���̉�f���ɂ͂ݏo�Ă���O�p�`�̔䗦
			float ra = (dx0 * ratio) / 2.0;	// �E���̉�f���̎O�p�`�̖ʐ�
			float la = (dx / 2) - ra + (rx - irx);	// �����̉�f���̎O�p�`�̖ʐ�
			putPixel(irx, y1, la*255);
			putPixel(irx+1, y1, ra*255);
		}else {
			float a = (rx - irx) + dx / 2.0;
			putPixel(irx, y1, a*255);
		}
		
	}
	
}

} // namespace Graphics

