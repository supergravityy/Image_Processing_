#include "Processing.h"

int histo_streching(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errCode)
{
	// ��� ��Ʈ������ �̹����� �������� Ȯ���Ͽ� ��� �����Ѵ�.
	// �̹����� ������׷����� ��Ⱚ���� �ܴ��ϰ� ����������, �ּ� ��Ⱚ�� �ִ� ��Ⱚ�� ã�´�
	// �� �������� �ִ� �������� Ȯ���Ų��.

	/*-------------------------------------*/
	// 1. �� ��Ⱚ�� ����ϱ� ���� ���ڵ��� ã�´�
	/*-------------------------------------*/

	BYTE denominator;
	BYTE factor[2];
	STASTICS bmp_Data;
	double temp_arr[MAX_BRIT_VAL] = { 0 };
	find_min_max(old_buffer,infoheader,factor);
	// 1��° �ε����� �ּҰ�, 0��° �ε����� �ּҰ�

	init_ARR(old_buffer, infoheader, temp_arr, &bmp_Data);
	write_hist(temp_arr);

	printf("\n-----------------------------------------");
	printf("\nmean : %.5f, variance : %.5f", bmp_Data.mean, bmp_Data.variance);
	printf("\n-----------------------------------------");
	printf("\n-----------------------------------------\n\n\n");

	denominator = factor[1] - factor[0]; // �и� ����

	if (denominator==0) // 0���� �������� �ֱ⿡
	{
		printf("This is not picture!\n All pixel has same brit_level!!\n");
		*errCode = 5;
		return 0;
	}

	printf("\nThe denominator : %d\n", denominator);

	/*-------------------------------------*/
	// 2. ������ �ϼ��Ѵ�
	// �ȼ��� �� ��Ⱚ = (PXL_BRIT - MIN)/(DENOMINATOR)*MAX_PXL_BRIT
	// ��� �ȼ��� �����Ͽ�, �����ۿ� �Ҵ��Ѵ�
	/*-------------------------------------*/

	double scale_factor = MAX_BRIT_VAL / denominator;
	for (unsigned int idx = 0; idx < infoheader->ImageSize; idx++)
		new_buffer[idx] = (BYTE)round((old_buffer[idx] - factor[0]) * scale_factor);

	init_ARR(new_buffer, infoheader, temp_arr, &bmp_Data);
	write_hist(temp_arr);

	printf("\n-----------------------------------------");
	printf("\nmean : %.5f, variance : %.5f", bmp_Data.mean, bmp_Data.variance);
	printf("\n-----------------------------------------");
	printf("\n-----------------------------------------\n\n\n");
	// �л갪�� ���� ������ �ʴ´� -> �ֳ��ϸ� ��� �ȼ��� ��Ⱚ�鿡 ���ؼ� ���������� ������ ����Ǿ��⿡
	// ����� ������ ����, �л��� ���ǿ� ����, �л��� ������ �ʴ´�.


	return 0;
}

void find_min_max(BYTE* old_buffer, BITMAPINFOHEADER* infoheader, BYTE* factor)
{
	// 0���� �ּ�, 1���� �ִ�

	for (unsigned int idx = 0; idx < infoheader->ImageSize; idx++)
	{
		if (factor[0] > old_buffer[idx])
			factor[0] = old_buffer[idx];

		if (factor[1] < old_buffer[idx])
			factor[1] = old_buffer[idx];
	}

	printf("\n\nThe MAX britnesss : %d\nThe MIN britness : %d\n\n", factor[1], factor[0]);
	
}