
#include <stdio.h>

#include "graphics/graphics.h"

int main(int argc, char* argv[])
{
	BorderStyle bs;
	bs.width = 5;
	bs.color = 0xFFFFFF;
	DrawRectangle(100,100, 100,100, 0x4F6F9F, bs);
	
	
	return 0;
}

