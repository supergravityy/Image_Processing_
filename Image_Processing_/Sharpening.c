#include "Processing.h"

int sharpening(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errCode)
{
	/*--------------------------------*/
	// 1. �������� ���� ó���� �ٸ��� �и������� Ŀ���� �ƴϴ�
	// �������� ���� Ŀ���� ������
	/*--------------------------------*/

	double kernel[9] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };
	normalize_filter(kernel, 3, 1);

	/*--------------------------------*/
	// 2. ������ ���
	/*--------------------------------*/

	printf("\nNow sharpening calculation is start!\n");

	for (int h = 0; h < infoheader->height; h++)
		for (int w = 0; w < infoheader->width; w++)
			new_buffer[h * infoheader->width + w] = sharp_cal(old_buffer, kernel, h, w, infoheader->width, infoheader->height);

	return 0;
}