#include "Processing.h"

int embossing(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errCode)
{
	/*------------------------------*/
	// 1. ������ ȿ�� ���� �и������� Ŀ���� �� �� ����
	/*------------------------------*/

	double kernel[9] = { -1,-1,0,0,0,0,0,1,1 };
	// �ȼ��� �밢�� �������� ���̸� ����Ѵ�, ���� ������ 0
	// �߰����� �ȼ�ó���� �Բ���� �밢�� �������� ���� ��µ��� ȿ���� �ش�
	
	/*------------------------------*/
	// 2. ������ ó�� ���
	/*------------------------------*/

	printf("\nNow embossing calculation is start!\n");

	int temp;
	for (int h = 0; h < infoheader->height; h++)
	{
		for (int w = 0; w < infoheader->width; w++)
		{
			temp = 128 + sharp_cal(old_buffer, kernel, h, w, infoheader->width, infoheader->height);
			new_buffer[h * infoheader->width + w] = clipping(temp);
		}
	}
	
	// �ش� Ŀ���� ���� ������ 0�̱⿡, ���� �����ð� ��Եȴ�. �̸� �����ϱ� ����, 128�� �����ش�.

	return 0;
}