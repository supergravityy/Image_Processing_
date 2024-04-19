#include "Convert.h"
#include "Processing.h"



int convert_BMP(char* oldName, char* newName, unsigned int mode)
{
	int errorcode = 0;
/*
* 1. ���Ͽ��� �Ұ���
* 2. �޸� �Ҵ� �Ұ���
* 3. �޸� ��� �̽�
* 4. ġ������ �޸� �̽�
*/

	/*---------------------------------------*/
	/* 1. ���Ͽ��� + ����б� + ���ϰ˻� */
	/*---------------------------------------*/

	if (mode >= QUIT || mode < 1) return 1;

	FILE* newBMP;
	FILE* oldBMP;
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	BITMAPColorPalette RGB[256];

	oldBMP = fopen(oldName, "rb");
	newBMP = fopen(newName, "wb");

	if (oldBMP == NULL)
	{
		printf("Can not open file!\n\n");
		errorcode = 1;
		goto close;
	}

	fread((char*)&fileheader, sizeof(BITMAPFILEHEADER), 1, oldBMP);
	fread((char*)&infoheader, sizeof(BITMAPINFOHEADER), 1, oldBMP);
	fread((char*)RGB, sizeof(RGB), 1, oldBMP);

	print_inform(&fileheader, &infoheader, RGB);

	if(infoheader.BitPerPxl != 8)
	{
		printf("Image file is not black & white!\n\n");
		errorcode = 1;
		goto close;
	}

	/*---------------------------------------*/
	/* 2. ��� �Է� + ���� ���� + �̹��� �б� */
	/*---------------------------------------*/

	fwrite((char*)&fileheader, sizeof(BITMAPFILEHEADER), 1, newBMP);
	fwrite((char*)&infoheader, sizeof(BITMAPINFOHEADER), 1, newBMP);
	fwrite((char*)RGB, sizeof(RGB), 1, newBMP);

	BYTE* old_buffer = (BYTE*)malloc(infoheader.ImageSize * sizeof(BYTE));
	BYTE* new_buffer = (BYTE*)malloc(infoheader.ImageSize * sizeof(BYTE));

	if (old_buffer == NULL || new_buffer == NULL)
	{
		printf("Cannot allocate memory section!\n\n");
		errorcode = 2;
		goto close;
	}

	fread(old_buffer, infoheader.ImageSize, 1, oldBMP);

	int result = mode_select(old_buffer, new_buffer, &infoheader, mode, &errorcode);

	if(result)
	{
		if (result == 3)
		{
			goto ignore;
			printf("Image Processing has been completed!\nBut it would be incorrect! \n");
		}
			
		printf("Processing Error Occured!\n\n"); // �����ڵ�� �ش� �Լ����� �ٲ��.
		goto clean_up;
	}


	printf("Image Processing has been completed! successfully\n");
ignore:
	fwrite(new_buffer, infoheader.ImageSize, 1, newBMP);

	/*---------------------------------------*/
	/* 3. �����ϱ� */
	/*---------------------------------------*/

clean_up:
	free(old_buffer);
	free(new_buffer);
close:
	fclose(oldBMP);
	fclose(newBMP);

	return errorcode; // �����۵��� false�� ��ȯ
}




void print_inform(BITMAPFILEHEADER* fileHeader, BITMAPINFOHEADER* infoHeader, BITMAPColorPalette* rgb)
{
	printf("file Hedaer\n");

	printf("File Type : %d, Size : %lu, Reserved1 : %d, Reserved2 : %d, Offset : %lu \n",
		fileHeader->type, fileHeader->size, fileHeader->reserved1, fileHeader->reserved2, fileHeader->offset);

	printf("info Header\n");

	printf("Compression : %lu, SizeImage : %lu, XPelsPerMeter : %ld, YPelsPerMeter : %ld, ClrUsed : %lu, ClrImportant : %lu, width : %d, height : %d \n",
		infoHeader->compression, infoHeader->ImageSize, infoHeader->xPelsPerMeter, infoHeader->yPelsPerMeter, infoHeader->clrUsed, infoHeader->clrImportant, infoHeader->width, infoHeader->height);

	printf("Palette info\n");

	for (int i = 0; i < 256; i++)
	{
		printf("Number(%d) Blue: %d, Green: %d, Red: %d, Reserved: %d\n",
			i, rgb->blue, rgb->green, rgb->red, rgb->reserved);
	}
}

int print_data(BYTE* buffer_inverted, DWORD pedded_width, DWORD height)
{
	int real_size = 0;
	printf("\nRGB data info\n");

	for (size_t y = 0; y < height; y++)
	{
		for (rsize_t x = 0; x < pedded_width; x++)
		{
			printf("%X ", *(buffer_inverted + y * pedded_width + x));
			real_size++;
		}
		printf("\n");
	}

	return real_size;
}

int mode_select(char* old_buffer, char* new_buffer, BITMAPINFOHEADER* infoheader, unsigned int mode, int* errCode)
{
	switch (mode)
	{
	case 1:
		//duplicate(old_buffer, new_buffer, infoheader, errCode);
		blurring(old_buffer, new_buffer, infoheader, errCode);
		break;
	case 2:
		sharpening(old_buffer, new_buffer, infoheader, errCode);
		break;
	case 3:
		mid_filtering(old_buffer, new_buffer, infoheader, errCode);
		break;
	case 4:
		edge_detecting(old_buffer, new_buffer, infoheader, errCode);
		break;
	case 5:
		// ȸ��
		break;
	case 6:
		// ���
		break;
	case 7:
		// Ȯ��
		break;
	case 8:
		histo_equalizing(old_buffer, new_buffer, infoheader, errCode);
		break;
	case 9:
		// ��Ʈ��Ī
		break;
	case 10:
		// ������
		break;
	case 11:
		// ����ȭ
		break;
	case 12:
		inverting(old_buffer, new_buffer, infoheader, errCode);
		break;
	}

	return *errCode;
}