
#include "graphics_impl_common.h"

namespace Graphics {

void DrawRectangle(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t color, const BorderStyle& border)
{
	// �w�i�`��
	fillSolidRectangle(x, y, w, h, color);
	
	// ���ɘg����`��
	if (border.width == 0) {
		return;
	}
	// top
	fillSolidRectangle(x, y, w, border.width, border.color);
	// left
	fillSolidRectangle(x, y+border.width, border.width, h-border.width*2, border.color);
	// right
	fillSolidRectangle(x+w-border.width, y+border.width, border.width, h-border.width*2, border.color);
	// bottom
	fillSolidRectangle(x, y+h-border.width, w, border.width, border.color);
}

} // namespace Graphics
