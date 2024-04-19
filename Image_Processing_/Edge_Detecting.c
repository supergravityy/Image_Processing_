#include "Processing.h"

int edge_detecting(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errCode)
{
	/*---------------------------*/
	// 1. �������� ���� �и������� Ŀ���� �ƴϴ�
	/*---------------------------*/

	double kernel[9] = { 0,-1,0,-1,4,-1,0,-1,0 }; // ������������ ������ 0�̴� -> ������ �ֺ��� ���� �ٸ���
	// 0���� ������ ���⿡, ����ȭ �Ұ���

	/*---------------------------*/
	// 2. �������� ���
	/*---------------------------*/

	printf("\nNow edge_detecting calculation is start!\n");
	BYTE caution = 0;

	for (int h = 0; h < infoheader->height; h++)
		for (int w = 0; w < infoheader->width; w++)
			new_buffer[h * infoheader->width + w] = sharp_cal(old_buffer, kernel, h, w, infoheader->width, infoheader->height);

	return 0;
}