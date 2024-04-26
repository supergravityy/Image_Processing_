#include "Processing.h"

void duplicate(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errcode)
{
	memcpy(new_buffer, old_buffer, infoheader->ImageSize);
	printf("Duplicating Image file has been completed! \n\n");
}

int bmp2txt()
{
	return 0;
}