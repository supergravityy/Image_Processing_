#ifndef PROCESSING_H
#define PROCESSING_H

#include "Convert.h"


#define swap(type,x,y) do{ type t=(x); (x)=(y); (y)=t;}while(0)
#define ANG2RAD(angle) ((M_PI * (double)(angle)) / 180.0)

#define MAX_BRIT_VAL 255
#define PXL_BRIT_NUM 256

int mem_overrun_cnt;

// 0. �׽�Ʈ��
void duplicate(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);

// 1. ����ȭ�� ó��
int inverting(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);

// 2. ȭ�ҿ��� ó��
int blurring(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int sharpening(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int edge_detecting(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int mid_filtering(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int embossing(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);

// 2-1. ȭ�ҿ��� ó���� �ʿ��� ���ó�� ������ �Լ�
BYTE row_cal(BYTE*, double*, int, int, int, int);
BYTE col_cal(BYTE*, double*, int, int, int, int);
BYTE sorting(BYTE*, int, int, int, int);

// 3. �������� ó��
int minimizing(BYTE*, BYTE**, BITMAPINFOHEADER*, BITMAPFILEHEADER*, int*);
int magnifying(BYTE*, BYTE**, BITMAPINFOHEADER*, BITMAPFILEHEADER*, int*);
int rotating(BYTE*, BYTE**, BITMAPINFOHEADER*, BITMAPFILEHEADER*, int*);

// 3-1. ������ ó���� �ʿ��� ������ �Լ�
int check_size_4m(int*, int*, BITMAPINFOHEADER*);
int check_size_4M(int*, int*, BITMAPINFOHEADER*);
int move_RAWdata(BYTE*, BYTE*, BITMAPINFOHEADER*, BITMAPFILEHEADER*, int, int, int);
int isInteger(double);
int rotate_RAWdata(BYTE*, BYTE*, BITMAPINFOHEADER*, double);
int finding_holes(BYTE * buffer, BITMAPINFOHEADER*);

// 4. ������� ó�� 
int histo_equalizing(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int histo_streching(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);
int binarizating(BYTE*, BYTE*, BITMAPINFOHEADER*, int*);

// 4-1. ���ó���� �ʿ��� ����ü
typedef struct
{
	double mean;
	double variance;
	double sum;
	double pow_sum;
}STASTICS;

// 4-2. ���ó���� �ʿ��� ������ �Լ�
void init_ARR(BYTE*, BITMAPINFOHEADER*, double*, STASTICS*);
void normalize_CDF(BYTE*, BITMAPINFOHEADER*, double*, STASTICS*);
void write_hist(double*);
void find_min_max(BYTE*, BITMAPINFOHEADER*, BYTE*);
void init_Stastics(STASTICS* );
double within_class_Var(double*, int, int, int, int, STASTICS*);
int find_valid_Val(char, double*);
int Otsu_final(BYTE*, BYTE*, BYTE, BITMAPINFOHEADER*);



// 5. �������� ȣ��Ǵ� �Լ��� ������ ���� inline���� �ۼ���
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

	// ������ ������� ����� �ۼ��Ǿ����� Ȯ��

	printf("filter coefficient :");

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			printf(" %f", *(kernal + i * size + j));

	printf("\nsum = %f\n", sum);
}

inline void draw_bar(int Num)
{
	int quotient = Num / 10;
	// 10 ������ �������� �����ع�����

	for (int num = 0; num < quotient; num++)
		printf("��");

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
	// 3. �������� �ʿ������� ������ ���������� ��Ⱚ�� ��ĥ���� �ִ� -> Ŭ������ �ʿ�
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
	// �ֱ��� �������̱� ������, �׳� ����� Ȧ�� �����ϱ� ����,
	// ��ó�� 0���� ������쿡�� �������� �Ǵ�

	// �������� �ٸ��� �ٸ����ۿ� �Űܾ��°��� �ƴ�, ������ ���ۿ� ���⶧���� ���������� ���ѵȴ�

	int referX = curX + 1; int referY = curY + 1;

	// 1���� �迭�̱⿡, +�������� �аԵȴ�, �׷��� ���⼭, -������ �ȼ��� �����ϸ�, �̹����� �������� -> ���� �ǵ帮�� ���� +���� �ȼ� ����

	if (referX < Width && (buffer[curY * Width + referX] != 0))
		return buffer[curY * Width + referX];

	// �ش��ȼ� (y,x+1) ��  �ִ� �ʺ� �����, ���� �ȼ��� ������ ���� ����

	if (referY < Height && (buffer[referY * Width + curX] != 0))
		return buffer[referY * Width + curX];

	// �����ȼ����� Ȧ�϶� || ���� �ȼ��� ������ ���϶�, �������� ����
	return 0;
}

#endif