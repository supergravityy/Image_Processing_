#include "Processing.h"

int magnifying(BYTE* old_buffer, BYTE** new_buffer, BITMAPINFOHEADER* infoheader, BITMAPFILEHEADER* fileheader, int* errCode)
{
	// �缱�� ������
	/*------------------------------------------------------*/
	// 1. ���� ũ��� Ȯ���� ������
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
	// 2. �Է¹��� ������ ���缭, ������ ũ��� ������ ���μ��� �ʺ� ����Ѵ�.
	/*------------------------------------------------------*/

	newSize = newHeight * newWidth;

	printf("The new width : %d , The new height : %d, Finally new Imagesize : %u\n", newWidth, newHeight, newSize);

	/*---------------------------------------------*/
	// 3. �ش� ��갪�� ����� �־� �ùٸ��� �����Ѵ�
	/*---------------------------------------------*/

	int oldWidth = infoheader->width;
	int oldHeight = infoheader->height;

	infoheader->width = newWidth;
	infoheader->height = newHeight;
	infoheader->ImageSize = newSize;
	fileheader->size = infoheader->ImageSize + fileheader->offset;

	/*---------------------------------------------*/
	// 4. �ش� ��갪�� ���缭 new_buffer�� ũ�⸦ ���Ҵ� ���ش�.
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
	// 5. ���� �̹����� �� �̹����� ���� ���ش�.
	/*---------------------------------------------*/

	// ���� �̹����� �� �ȼ���ġ�� ���� �� �̹��������� �����Ǵ� ��ġ�� ���
	// ���� ��ġ�� �ֺ� 4���� �ȼ����� ã��
	// �� 4���� �ȼ��� ����ġ�� ����Ͽ� �� �ȼ��� ��Ⱚ�� ���
	// ���� ������ �� �̹��� ���ۿ� ����

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

	// X�������� Ȯ��

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

	// Y�������� Ȯ��

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
	// �缱���������� ���� ���� ���� ��ǥ�� (���� ����)
}