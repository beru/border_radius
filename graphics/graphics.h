#ifndef GRAPHICS_H_INCLUDED__
#define GRAPHICS_H_INCLUDED__

namespace Graphics {

void PutPixel(uint16_t x, uint16_t y, uint32_t color);
void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);

void DrawHorizontalLine(int16_t x1, int16_t x2, int16_t y2, uint32_t color);
void DrawVerticalLine(int16_t x, int16_t y1, int16_t y2, uint32_t color);
void DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint32_t color);
void DrawLinearGradient(int16_t x1, int16_t y1, uint32_t v1, int16_t x2, int16_t y2, uint32_t v2);

struct BorderStyle
{
	uint32_t color;
	uint8_t width;
	uint8_t radius;
};

void DrawRectangle(int16_t x, int16_t y, uint16_t w, uint16_t h, uint32_t color, const BorderStyle& border);

void DrawCircle(int16_t cx, int16_t cy, uint16_t diameter, uint32_t color);
void DrawFilledCircle(int16_t cx, int16_t cy, uint16_t diameter, uint32_t color);
void DrawGradationCircle(int16_t cx, int16_t cy, uint16_t diameter);

} // namespace Graphics

#endif // ifndef GRAPHICS_H_INCLUDED__
