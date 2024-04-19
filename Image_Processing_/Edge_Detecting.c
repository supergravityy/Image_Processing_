#include "Processing.h"

int edge_detecting(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errCode)
{
	/*---------------------------*/
	// 1. 엣지검출 역시 분리가능한 커널이 아니다
	/*---------------------------*/

	double kernel[9] = { 0,-1,0,-1,4,-1,0,-1,0 }; // 엣지디텍팅은 총합이 0이다 -> 엣지는 주변과 색이 다르다
	// 0으로 나눌수 없기에, 정규화 불가능

	/*---------------------------*/
	// 2. 엣지검출 계산
	/*---------------------------*/

	printf("\nNow edge_detecting calculation is start!\n");
	BYTE caution = 0;

	for (int h = 0; h < infoheader->height; h++)
		for (int w = 0; w < infoheader->width; w++)
			new_buffer[h * infoheader->width + w] = sharp_cal(old_buffer, kernel, h, w, infoheader->width, infoheader->height);

	return 0;
}