#ifndef CONVERT_H
#define CONVERT_H

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define _USE_MATH_DEFINES
#define QUIT 13

#pragma pack(2) 
// �����찡 bmp ����� �ùٸ��� �о�� �ϱ⶧����, ����ü�� ����� ������ 4����Ʈ�� �ƴ� 2����Ʈ�� ����

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;

typedef struct
{
	WORD type;
	DWORD size;
	WORD reserved1;
	WORD reserved2;
	DWORD offset;
}BITMAPFILEHEADER;

typedef struct
{
	DWORD structSize;
	LONG width;
	LONG height;
	WORD planes;
	WORD BitPerPxl;
	DWORD compression;
	DWORD ImageSize;
	LONG xPelsPerMeter;
	LONG yPelsPerMeter;
	DWORD clrUsed;
	DWORD clrImportant;
}BITMAPINFOHEADER;

typedef struct
{
	BYTE blue;
	BYTE green;
	BYTE red;
	BYTE reserved;
}BITMAPColorPalette;

int convert_BMP(char*, char*, unsigned int);
void print_inform(BITMAPFILEHEADER*, BITMAPINFOHEADER*, BITMAPColorPalette*);
int print_data(BYTE*, DWORD, DWORD);
int mode_select(char*, char*, BITMAPINFOHEADER*, unsigned int, int*);

#pragma pack(pop)
// Ȥ�� �̵��� ���� ����ü�� ��� �� ���� �ֱ⿡, �ٽ� ������� �ٲ� ���´�.

#endif