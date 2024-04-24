#include "Processing.h"

#define MAX_BRIT_VAL 256

int histo_equalizing(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errCode)
{
	/*---------------------------------------*/
	// 1. ������׷� ��Ȱȭ�� �ʿ��� �ʱ�ȭ ����
	/*---------------------------------------*/

	STASTICS bmp_Data; // ��谪�� Ȯ���ϱ����� ����ü �ʱ�ȭ

	double TEMP_ARR[MAX_BRIT_VAL] = { 0 }; 
	// ������׷� ������ ���� �迭 ����, �ʱ�ȭ
	// �� �迭�� �ε�����ȣ ��ü�� 0~255������ ��Ⱚ�� �Ȱ���.


	/*---------------------------------------*/
	// 2. �̹����� �ȼ����� �迭�� �ε��� ��ȣ�� ���� ���⿡, �󵵼��� �����Ѵ�.
	// ���ġ�� ���ϰ�, ��ü�ȼ��� ��, ��Ⱚ�� ���� ������׷��� ���
	/*---------------------------------------*/

	init_ARR(old_buffer, infoheader, TEMP_ARR, &bmp_Data);

	printf("\n-----------------------------------------");
	printf("\nmean : %.5f, variance : %.5f", bmp_Data.mean, bmp_Data.variance);
	printf("\n-----------------------------------------");
	printf("\n-----------------------------------------\n\n\n");

	write_hist(TEMP_ARR);

	printf("\n\n\n");
	for (int brit = 0; brit < MAX_BRIT_VAL; brit++)
		printf("(%d) %d   ", brit, (int)TEMP_ARR[brit]);

	/*---------------------------------------*/
	// 3. ��ü ��Ⱚ���� �����, ��ü �̹������� Ư�� ��Ⱚ�� ���� Ȯ���� ���Ѵ�. (����ȭ ����)
	// �׸���, �ش� Ȯ���� ��ü ��Ⱚ�� ���ؼ�, �ٽ� ��Ⱚ�� ���� ������ ������ش�.
	/*---------------------------------------*/

	normalize_CDF(old_buffer, infoheader, TEMP_ARR, &bmp_Data);

	for (int idx = 0; idx < infoheader->ImageSize; idx++)
		new_buffer[idx] = (BYTE)TEMP_ARR[old_buffer[idx]];
	// �� ���� ������ �ȼ��� ��Ⱚ�� ������ ������׷��� �־������, �� ���� ��Ⱚ�� ������ �ȴ�
	// �� �ش� ����(Ȯ��*�ִ��Ⱚ)�� ó���� ��Ⱚ���� �����ϰ�, ���ο� ���ۿ� �־������.

	init_ARR(new_buffer, infoheader, TEMP_ARR, &bmp_Data);

	printf("\n-----------------------------------------");
	printf("\nmean : %.5f, variance : %.5f", bmp_Data.mean, bmp_Data.variance);
	printf("\n-----------------------------------------");
	printf("\n-----------------------------------------\n\n\n");

	write_hist(TEMP_ARR);

	printf("\n\n\n");
	for (int brit = 0; brit < MAX_BRIT_VAL; brit++)
		printf("(%d) %d   ", brit, (int)TEMP_ARR[brit]);

	printf("\n");



	return 0;
}

void init_ARR(BYTE* buffer, BITMAPINFOHEADER* infoheader, double* temp_arr, STASTICS* data)
{
	// ��Ⱚ�� ���� �󵵼� �����ϱ�

	data->mean = 0; data->pow_sum =0; data->sum = 0; data->variance = 0;

	for (int idx = 0; idx < infoheader->ImageSize; idx++)
	{
		for (int brit = 0; brit < MAX_BRIT_VAL; brit++)
		{
			if (buffer[idx] == brit)
			{
				temp_arr[brit]++;
				data->sum += buffer[idx];
				data->pow_sum += pow((double)buffer[idx], 2);
				break; // �ȼ��� ��Ⱚ�� ã������, �پ�Ѿ������
			}
		}
	}

	data->mean = data->sum / infoheader->ImageSize;
	data->variance = ((double)data->pow_sum / (double)infoheader->ImageSize) - pow(data->mean, 2);
	// ������׷��� ��հ� �л��� ���ؼ� ���� ����
}

void normalize_CDF(BYTE* old_buffer, BITMAPINFOHEADER* infoheader, double* temp_arr, STASTICS* data)
{
	// ��Ⱚ�� ���� �󵵼��� �����Ǿ�����, ��ü �ȼ����� ������, 
	// �̹������� Ư�� ��Ⱚ�� �̾������� ��ü �ȼ� ��, �� ��Ⱚ�� ���� Ȯ���� ������
	// ����, �ִ� ��Ⱚ�� ���Ͽ�, ���ο� ��Ⱚ���� ������ Ȯ���Ѵ�

	double scale_factor = (double)MAX_BRIT_VAL / (double)infoheader->ImageSize;
	int temp;
	double sum = 0;

	for (int brit = 0; brit < MAX_BRIT_VAL; brit++)
	{
		sum += temp_arr[brit];
		temp = (int)round(sum * scale_factor); // ����ȭ ��, �ڵ��ݿø�
		printf("(%d) %d, ", brit, temp);
		temp_arr[brit] = temp; // �ٽ� ������ ������׷� �󵵼��� �־����
	}
	// ���� ������׷� ��Ȱȭ�� �Ϸ�Ǿ���.

	printf("\n\n");
}

void write_hist(double* temp_arr)
{
	static char status = 0;
	int count = 0;

	if (status == 0)
	{
		printf("\n\n---This is brightness value histogram of original---\n\n");
		status++;
	}
	else
		printf("\n\n---This is brightness value histogram of modified---\n\n");

	for (int brit = 0; brit < MAX_BRIT_VAL; brit++)
	{
		count = (int)temp_arr[brit];
		if (brit >= 0 && brit < 10)
		{
			printf("%d   | ", brit);
			draw_bar(count);
		}

		else if (brit >= 10 && brit < 100)
		{
			printf("%d  | ", brit);
			draw_bar(count);
		}

		else
		{
			printf("%d | ", brit);
			draw_bar(count);
		}
	}

	printf("\n\n");
}

inline void draw_bar(int Num)
{
	int quotient = Num / 10;
	// 10 ������ �������� �����ع�����

	for (int num = 0; num < quotient; num++)
		printf("��");

	printf("\n");
}