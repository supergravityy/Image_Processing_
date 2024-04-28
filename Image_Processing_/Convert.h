#ifndef CONVERT_H
#define CONVERT_H

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define _USE_MATH_DEFINES
#define QUIT 14

#pragma pack(2) 
// 윈도우가 bmp 헤더를 올바르게 읽어야 하기때문에, 구조체의 멤버를 기존의 8바이트가 아닌 2바이트로 맞춤

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

int convert_TXT(char*, char*);
void write_header(BITMAPFILEHEADER*, BITMAPINFOHEADER*, BYTE*, FILE*);
void write_raw(BITMAPINFOHEADER*, BYTE*, FILE*, int*);

#pragma pack(pop)
// 혹시 이따가 본래 구조체를 써야 할 수도 있기에, 다시 본래대로 바꿔 놓는다.

#endif