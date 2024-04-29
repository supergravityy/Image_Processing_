#include "Processing.h"

int blurring(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errcCode)
{
	/*----------------------------------*/
	// 1. 행렬을 컨볼루션으로 계산하기에, 행과 열로 쪼개서 하면, 계산시간이 절약됨
	// 그렇기에, 행계산을 저장할 버퍼를 동적할당 해야한다.
	/*----------------------------------*/
	char* temp_buffer = (char*)malloc(infoheader->ImageSize * sizeof(BYTE));

	if (temp_buffer == NULL)
	{
		printf("Memory allocation Error! \n");
		*errcCode = 2;
		goto alloc_err;
	}

	/*----------------------------------*/
	// 2. 블러링에 필요한 가우시안 필터 만들기 3X3으로 만든다
	/*----------------------------------*/

	double kernel[9] = { 1,2,1,2,4,2,1,2,1 }; // 가우시안 2차원필터 생성
	normalize_filter(kernel, 3, 4); // 각 원소들을 16으로 나눠줘야한다.

	/*----------------------------------*/
	// 3. image ★ h_vec = temp -> 먼저 행(가로)으로 행렬계산
	/*----------------------------------*/
	
	printf("\nNow row vec calculation is start!\n");

	for (int h = 0; h < infoheader->height; h++)
		for (int w = 0; w < infoheader->width; w++)
			temp_buffer[h * infoheader->width + w] = row_cal(old_buffer, kernel, h, w, infoheader->width, infoheader->height);

	/*----------------------------------*/
	// 4. temp ★ w_vec = new -> 임시와 열(세로)로 행렬계산
	/*----------------------------------*/

	printf("\nNow col vec calculation is start!\n");

	for (int h = 0; h < infoheader->height; h++)
		for (int w = 0; w < infoheader->width; w++)
			new_buffer[h * infoheader->width + w] = col_cal(temp_buffer, kernel, h, w, infoheader->width, infoheader->height);

	/*----------------------------------*/
	// 5. 정리
	/*----------------------------------*/

	if (mem_overrun_cnt != 0)
		*errcCode = 3;

alloc_err:
	if (*errcCode == 2) return 1; // 메모리 할당 실패! 

clean_up:
	free(temp_buffer);

	return 0;
}

BYTE row_cal(BYTE* old_buffer,double* kernel,int h,int w,int width,int height)
{
    double sum = 0.0;
    double row_vec[3] = { kernel[0],kernel[1],kernel[2] };
    int wrapped_i = 0;

    for (int i = h - 1; i < h + 2; i++) // i -> 행(y축)
    {
        wrapped_i = circular_wrapping(i, height);
        sum += (double)old_buffer[wrapped_i * width + w] * row_vec[i - h + 1];

		if (sum > 255)
		{
			printf("%f at (%d, %d)\n", sum, h,w);
			printf("Warning : This processing would be incorrect!\n");
			mem_overrun_cnt++;
		}
    }

    return (BYTE)round(sum);
}

BYTE col_cal(BYTE* temp_buffer, double* kernel, int h, int w, int width, int height)
{
	double sum = 0.0;
	double N = kernel[0];
	double column_vec[3] = { kernel[0]/N , kernel[3]/N , kernel[6]/N }; // 정규화
	int wrapped_j = 0;

	for (int j = w - 1; j < w + 2; j++) // j -> 열(x축)
	{
		wrapped_j = circular_wrapping(j, width);
		sum += (double)temp_buffer[h * width + wrapped_j] * column_vec[j - w + 1];

		if (sum > 255)
		{
			printf("%f at (%d, %d)\n", sum, h, w);
			printf("Warning : This processing would be incorrect!\n");
			mem_overrun_cnt++;
		}
	}

	return (BYTE)round(sum);
}