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
    // 3. 샤프닝은 필연적으로 필터의 구조때문에 밝기값에 걸칠수도 있다 -> 클리핑이 필요
    /*--------------------------------*/

    int result = (int)(sum + 0.5);

    if (result < 0)
        result = 0;

    else if (result > 255)
        result = 255;

    return (BYTE)(result);
}