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

BYTE sharp_cal(BYTE* old_buffer, double* kernel, int h, int w, int width, int height)
{
    double sum = 0;
    int wrapped_i = 0; int wrapped_j = 0;

    for (int i = h - 1; i < h + 2; i++)
    {
        wrapped_i = circular_wrapping(i, height);
        
        for (int j = w - 1; j < w + 2; j++)
        {
            wrapped_j = circular_wrapping(j, width);
            sum += (double)old_buffer[wrapped_i * width + wrapped_j] * kernel[(i - h + 1) * 3 + (j - w + 1)];
        }
    }

    /*--------------------------------*/
    // 3. �������� �ʿ������� ������ ���������� ��Ⱚ�� ��ĥ���� �ִ� -> Ŭ������ �ʿ�
    /*--------------------------------*/

    int result = (int)(sum + 0.5);

    if (result < 0)
        result = 0;

    else if (result > 255)
        result = 255;

    return (BYTE)(result);
}