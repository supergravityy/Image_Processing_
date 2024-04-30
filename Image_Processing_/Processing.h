#ifndef PROCESSING_H
#define PROCESSING_H

#include "Convert.h"

#define swap(type,x,y) do { type t=(x); (x)=(y); (y)=t;} while(0)
#define MAX_BRIT_VAL 255

int mem_overrun_cnt;

void duplicate(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int blurring(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int sharpening(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int edge_detecting(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int mid_filtering(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int inverting(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int embossing(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);

typedef struct
{
	double mean;
	double variance;
	double sum;
	double pow_sum;
}STASTICS;


int histo_equalizing(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int histo_streching(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);


BYTE row_cal(BYTE* , double* ,int ,int ,int ,int );
BYTE col_cal(BYTE* , double* ,int ,int ,int ,int );
BYTE sorting(BYTE*, int, int, int, int);

void init_ARR(BYTE*, BITMAPINFOHEADER*, double*, STASTICS*);
void normalize_CDF(BYTE*, BITMAPINFOHEADER*, double*, STASTICS*);
void write_hist(double*);
void find_min_max(BYTE*, BITMAPINFOHEADER*, BYTE*);



inline int circular_wrapping(int idx, int max)
{
	if (idx < 0)
		return (max + idx) % max;
	return idx % max;
}

inline void normalize_filter(double* kernal, int size, int coeff)
{
	int scailing = (int)pow(coeff, 2);
	double sum = 0.0;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			kernal[i * size + j] /= scailing;
			sum += kernal[i * size + j];
		}
	}

	// 필터의 계수들이 제대로 작성되었는지 확인

	printf("filter coefficient :");

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			printf(" %f", *(kernal + i * size + j));

	printf("\nsum = %f\n", sum);
}

inline void draw_bar(int Num)
{
	int quotient = Num / 10;
	// 10 이하의 나머지는 무시해버린다

	for (int num = 0; num < quotient; num++)
		printf("▥");

	printf("\n");
}

inline int clipping(int pxl)
{
	if (pxl > 255)
		pxl = 255;
	else if (pxl < 0)
		pxl = 0;

	return (BYTE)(pxl);
}

inline BYTE sharp_cal(BYTE* old_buffer, double* kernel, int h, int w, int width, int height)
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

	return (BYTE)clipping(result);
}

#endif