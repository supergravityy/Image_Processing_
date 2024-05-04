#include "Processing.h"

int magnifying(BYTE* old_buffer, BYTE** new_buffer, BITMAPINFOHEADER* infoheader, BITMAPFILEHEADER* fileheader, int* errCode)
{
	// 양선형 보간법
	/*------------------------------------------------------*/
	// 1. 얼마의 크기로 확대할 것인지
	/*------------------------------------------------------*/

	int newWidth; int newHeight; DWORD newSize;

	printf("How much do you want to increase?\n\n");

	do
	{
		printf("The original BMP file's information : width -> %d, height -> %d\n", infoheader->width, infoheader->height);
		printf("The new size must bigger than original size and must be positive number!\n\n");
		printf("Input new width : ");
		scanf("%d", &newWidth);
		fflush(stdin);
		printf("\nInput new height : ");
		scanf("%d", &newHeight);
		fflush(stdin);
		puts("\n\n");
	} while (check_size_4M(&newWidth, &newHeight, infoheader));

	/*------------------------------------------------------*/
	// 2. 입력받은 정보에 맞춰서, 파일의 크기와 파일의 가로세로 너비를 계산한다.
	/*------------------------------------------------------*/

	newSize = newHeight * newWidth;

	printf("The new width : %d , The new height : %d, Finally new Imagesize : %u\n", newWidth, newHeight, newSize);

	/*---------------------------------------------*/
	// 3. 해당 계산값을 헤더로 넣어 올바르게 수정한다
	/*---------------------------------------------*/

	int oldWidth = infoheader->width;
	int oldHeight = infoheader->height;

	infoheader->width = newWidth;
	infoheader->height = newHeight;
	infoheader->ImageSize = newSize;
	fileheader->size = infoheader->ImageSize + fileheader->offset;

	/*---------------------------------------------*/
	// 4. 해당 계산값에 맞춰서 new_buffer의 크기를 재할당 해준다.
	/*---------------------------------------------*/

	*new_buffer = NULL;
	BYTE* new_addr = realloc(*new_buffer, sizeof(char) * newSize);

	if (new_addr == NULL)
	{
		printf("realloc Error occured!\n");
		*errCode = 2;
		return 0;
	}

	*new_buffer = new_addr;

	/*---------------------------------------------*/
	// 5. 기존 이미지를 새 이미지에 매핑 해준다.
	/*---------------------------------------------*/

	// 원본 이미지의 각 픽셀위치에 대해 새 이미지에서의 대응되는 위치를 계산
	// 계산된 위치의 주변 4개의 픽셀값을 찾기
	// 이 4개의 픽셀과 가중치를 사용하여 새 픽셀의 밝기값을 계산
	// 계산된 색상값을 새 이미지 버퍼에 적용

	double xRatio = (double)oldWidth/ (double)newWidth;
	double yRatio = (double)oldHeight / (double)newHeight;

	int intX, intY;
	double floatX, floatY;
	BYTE top, bottom;

	BYTE* temp_buffer = (BYTE*)malloc(sizeof(BYTE) * newSize);

	if (temp_buffer == NULL)
	{
		printf("Cannot memory section allocate!\n");
		*errCode = 2;
		return 0;
	}

	// X축으로의 확장

	for (int oldY = 0; oldY < oldHeight; oldY++)
	{
		for (int newX = 0; newX < newWidth; newX++)
		{
			floatX = newX * xRatio;
			intX = (int)floor(floatX);

			if (intX + 1 < oldWidth)
			{
				bottom = old_buffer[oldY * oldWidth + intX];
				top = old_buffer[oldY * oldWidth + intX + 1];
				temp_buffer[oldY * newWidth + newX] = bottom + (BYTE)(abs(top - bottom) * (floatX - intX));
			}

			else
				temp_buffer[oldY * newWidth + newX] = old_buffer[oldY * oldWidth + intX];
		}
	}

	// Y축으로의 확장

	for (int newY = 0; newY < newHeight; newY++)
	{
		for (int newX = 0; newX < newWidth; newX++)
		{
			floatY = newY * yRatio;
			intY = (int)floor(floatY);

			if (intY + 1 < oldHeight)
			{
				bottom = temp_buffer[(intY * newWidth) + newX];
				top = temp_buffer[(intY + 1) * newWidth + newX];
				new_addr[newY * newWidth + newX] = bottom + (BYTE)(abs(top - bottom) * (floatY - intY));
			}

			else
				new_addr[newY * newWidth + newX] = temp_buffer[intY * newWidth + newX];
		}
	}

	free(temp_buffer);

	return 0;
}

int check_size_4M(int* newWidth, int* newHeight, BITMAPINFOHEADER* infoheader)
{
	int width_check = 1; int height_check = 1;


	return width_check || height_check;
}

BYTE Bilinear_Interploate(BYTE top_Left, BYTE top_Right, BYTE bottom_Left, BYTE bottom_Right, double xDiff, double yDiff)
{
	double valueTOP = top_Left + xDiff*(top_Right - top_Left);
	double valueBOTTOM = bottom_Left + xDiff*(bottom_Right - bottom_Left);
	
	return (BYTE)(valueTOP + yDiff * (valueBOTTOM - valueTOP));
	// 양선형보간법에 의한 최종 매핑 좌표값 (기존 버퍼)
}