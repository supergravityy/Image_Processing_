#ifndef PROCESSING_H
#define PROCESSING_H

#include "Convert.h"


#define swap(type,x,y) do{ type t=(x); (x)=(y); (y)=t;}while(0)
#define ANG2RAD(angle) ((M_PI * (double)(angle)) / 180.0)

#define MAX_BRIT_VAL 255

int mem_overrun_cnt;

// 0. 테스트용
void duplicate(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);

// 1. 개별화소 처리
int inverting(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);

// 2. 화소영역 처리
int blurring(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int sharpening(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int edge_detecting(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int mid_filtering(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int embossing(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);

// 2-1. 화소영역 처리에 필요한 행렬처리 자잘이 함수
BYTE row_cal(BYTE*, double*, int, int, int, int);
BYTE col_cal(BYTE*, double*, int, int, int, int);
BYTE sorting(BYTE*, int, int, int, int);

// 3. 기하학적 처리
int minimizing(BYTE*, BYTE**, BITMAPINFOHEADER*, BITMAPFILEHEADER*, int*);
int magnifying(BYTE*, BYTE**, BITMAPINFOHEADER*, BITMAPFILEHEADER*, int*);
int rotating(BYTE*, BYTE**, BITMAPINFOHEADER*, BITMAPFILEHEADER*, int*);

// 3-1. 기하학 처리에 필요한 자잘이 함수
int check_size_4m(int*, int*, BITMAPINFOHEADER*);
int check_size_4M(int*, int*, BITMAPINFOHEADER*);
int move_RAWdata(BYTE*, BYTE*, BITMAPINFOHEADER*, BITMAPFILEHEADER*, int, int, int);
int isInteger(double);
int rotate_RAWdata(BYTE*, BYTE*, BITMAPINFOHEADER*, double);
int finding_holes(BYTE * buffer, BITMAPINFOHEADER*);

// 4. 통계학적 처리 
int histo_equalizing(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int histo_streching(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);

// 4-1. 통계처리에 필요한 구조체
typedef struct
{
	double mean;
	double variance;
	double sum;
	double pow_sum;
}STASTICS;

// 4-2. 통계처리에 필요한 자잘이 함수
void init_ARR(BYTE*, BITMAPINFOHEADER*, double*, STASTICS*);
void normalize_CDF(BYTE*, BITMAPINFOHEADER*, double*, STASTICS*);
void write_hist(double*);
void find_min_max(BYTE*, BITMAPINFOHEADER*, BYTE*);


// 5. 루프마다 호출되는 함수는 성능을 위해 inline으로 작성됨
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

inline BYTE bilinear_interpolation(double upLeft, double upRight, double downLeft, double downRight, double xDiff, double yDiff)
{
	return (BYTE)floor((1 - yDiff) * (downLeft * xDiff + (1 - xDiff) * downRight) + yDiff * (upLeft * xDiff + (1 - xDiff) * upRight));
}

inline BYTE MYnearest_interpolate(BYTE* buffer, int curX, int curY, int Width, int Height)
{
	// 최근접 보간법이긴 하지만, 그냥 공백과 홀을 구분하기 위해,
	// 근처에 0값만 있을경우에는 공백으로 판단

	// 기존과는 다르게 다른버퍼에 옮겨쓰는것이 아닌, 현재의 버퍼에 쓰기때문에 참조방향이 제한된다

	int referX = curX + 1; int referY = curY + 1;

	// 1차원 배열이기에, +방향으로 읽게된다, 그런데 여기서, -방향의 픽셀을 참조하면, 이미지가 뭉게진다 -> 아직 건드리지 않은 +방향 픽셀 참조

	if (referX < Width && (buffer[curY * Width + referX] != 0))
		return buffer[curY * Width + referX];

	// 해당픽셀 (y,x+1) 이  최대 너비를 벗어나면, 현재 픽셀을 오른쪽 경계로 가정

	if (referY < Height && (buffer[referY * Width + curX] != 0))
		return buffer[referY * Width + curX];

	// 참조픽셀값이 홀일때 || 참조 픽셀이 경계범위 밖일때, 공백으로 판정
	return 0;
}

#endif