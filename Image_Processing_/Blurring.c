#include "Processing.h"

int blurring(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errcCode)
{
	/*----------------------------------*/
	// 1. ����� ����������� ����ϱ⿡, ��� ���� �ɰ��� �ϸ�, ���ð��� �����
	// �׷��⿡, ������ ������ ���۸� �����Ҵ� �ؾ��Ѵ�.
	/*----------------------------------*/
	char* temp_buffer = (char*)malloc(infoheader->ImageSize * sizeof(BYTE));

	if (temp_buffer == NULL)
	{
		printf("Memory allocation Error! \n");
		*errcCode = 2;
		goto alloc_err;
	}

	/*----------------------------------*/
	// 2. ������ �ʿ��� ����þ� ���� ����� 3X3���� �����
	/*----------------------------------*/

	double kernel[9] = { 1,2,1,2,4,2,1,2,1 }; // ����þ� 2�������� ����
	normalize_filter(kernel, 3, 4); // �� ���ҵ��� 16���� ��������Ѵ�.

	/*----------------------------------*/
	// 3. image �� h_vec = temp -> ���� ��(����)���� ��İ��
	/*----------------------------------*/
	
	printf("\nNow row vec calculation is start!\n");

	for (int h = 0; h < infoheader->height; h++)
		for (int w = 0; w < infoheader->width; w++)
			temp_buffer[h * infoheader->width + w] = row_cal(old_buffer, kernel, h, w, infoheader->width, infoheader->height);

	/*----------------------------------*/
	// 4. temp �� w_vec = new -> �ӽÿ� ��(����)�� ��İ��
	/*----------------------------------*/

	printf("\nNow col vec calculation is start!\n");

	for (int h = 0; h < infoheader->height; h++)
		for (int w = 0; w < infoheader->width; w++)
			new_buffer[h * infoheader->width + w] = col_cal(temp_buffer, kernel, h, w, infoheader->width, infoheader->height);

	/*----------------------------------*/
	// 5. ����
	/*----------------------------------*/

	if (mem_overrun_cnt != 0)
		*errcCode = 3;

alloc_err:
	if (*errcCode == 2) return 1; // �޸� �Ҵ� ����! 

clean_up:
	free(temp_buffer);

	return 0;
}

BYTE row_cal(BYTE* old_buffer,double* kernel,int h,int w,int width,int height)
{
    double sum = 0.0;
    double row_vec[3] = { kernel[0],kernel[1],kernel[2] };
    int wrapped_i = 0;

    for (int i = h - 1; i < h + 2; i++) // i -> ��(y��)
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
	double column_vec[3] = { kernel[0]/N , kernel[3]/N , kernel[6]/N }; // ����ȭ
	int wrapped_j = 0;

	for (int j = w - 1; j < w + 2; j++) // j -> ��(x��)
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