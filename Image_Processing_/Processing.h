#ifndef PROCESSING_H
#define PROCESSING_H

#include "Convert.h"

#define swap(type,x,y) do { type t=(x); (x)=(y); (y)=t;} while(0)

int mem_overrun_cnt;

void duplicate(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int blurring(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int sharpening(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int edge_detecting(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int mid_filtering(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int inverting(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);

typedef struct
{
	double mean;
	double variance;
	double sum;
	double pow_sum;
}STASTICS;


int histo_equalizing(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
void init_ARR(BYTE*, BITMAPINFOHEADER*, BYTE*, STASTICS*);
void normalize_CDF(BYTE*, BITMAPINFOHEADER*, BYTE*, STASTICS*);
void write_hist(double*);
inline void draw_bar(int);

int histo_streching(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);

BYTE row_cal(BYTE* , double* ,int ,int ,int ,int );
BYTE col_cal(BYTE* , double* ,int ,int ,int ,int );
BYTE sharp_cal(BYTE* , double* ,int ,int ,int ,int );
BYTE sorting(BYTE*, int, int, int, int);



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

#endif