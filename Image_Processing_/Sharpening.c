#include "Processing.h"

int sharpening(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errCode)
{
	/*--------------------------------*/
	// 1. 샤프닝은 블러링 처리와 다르게 분리가능한 커널이 아니다
	// 샤프닝을 위한 커널을 만들자
	/*--------------------------------*/

	double kernel[9] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };
	normalize_filter(kernel, 3, 1);

	/*--------------------------------*/
	// 2. 샤프닝 계산
	/*--------------------------------*/

	printf("\nNow sharpening calculation is start!\n");

	for (int h = 0; h < infoheader->height; h++)
		for (int w = 0; w < infoheader->width; w++)
			new_buffer[h * infoheader->width + w] = sharp_cal(old_buffer, kernel, h, w, infoheader->width, infoheader->height);

	return 0;
}