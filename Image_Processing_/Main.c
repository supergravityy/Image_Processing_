#include "Convert.h"
#define STR_LEN 100

int isBMP(char*);
int addName(char*, char**, unsigned int);

int main()
{
	char* originalName = (char*)malloc(sizeof(char) * STR_LEN);
	char* neoName = (char*)malloc(sizeof(char) * STR_LEN);

	/*-------------------------------------------------*/
	// ��ȯ�� �̹��� ���� �̸� �Է¹ޱ�
	/*-------------------------------------------------*/
	
	do
	{
		printf("type vaild BMP name! \n");
		printf("the file name must end with \".bmp\" \n");
		printf("typing \'q\' or \'Q\' is Quit!\n");

		printf("File name: ");
		fgets(originalName, STR_LEN, stdin); // stdin���� �ִ� 99���� + NULL�� �о��

		if (strcmp(originalName, "q\n") == 0 || strcmp(originalName, "Q\n") == 0)
			return 0;

		// fgets�� ���� ���ڱ��� �Է� �����Ƿ�, ���� ���ڸ� NULL�� �ٲ���
		size_t ln = strlen(originalName) - 1; // NULL ���� ������ NULL��~
		if (originalName[ln] == '\n')
			originalName[ln] = '\0';
	} while (isBMP(originalName));

	unsigned int mode = 0;
	

	/*-------------------------------------------------*/
	// �̹��� ������ ��� ��ȯ�� ���ΰ�? ��� �Է¹ޱ�
	/*-------------------------------------------------*/
	while (mode < 1 || mode > 12)
	{
		printf("1. blurring\n");
		printf("2. sharpening\n");
		printf("3. midean filtering\n");
		printf("4. edge detection\n");
		printf("5. rotating\n");
		printf("6. minimizing\n");
		printf("7. magnifying\n");
		printf("8. histogram equalizing\n");
		printf("9. histogram stretching\n");
		printf("10. embossing\n");
		printf("11. binarization\n");
		printf("12. invert\n");
		printf("13. Quit\n");

		printf("type the mode you want : ");
		scanf("%u", &mode);
		getchar(); // stdin ���� �Һ��ϱ�
	}

	if (mode == 13)
	{
		printf("existing.. \n");
		goto termination;
	}

	/*-------------------------------------------------*/
	// ��忡 ���� ó����� �̹��� ������ �̸��� �ٲ��ֱ� + �������� ��ȯ
	/*-------------------------------------------------*/
	if (addName(originalName, &neoName, mode))
	{
		printf("Name changing progress has been terminated with Occuring Error! \n");
		goto termination;
	}

	/*printf("Name change progress has been completed! \n");*/
	printf("%s\n", neoName);

	int errcode = convert_BMP(originalName, neoName, mode);

	if(errcode)
	{
		printf("Fatal error occured! => %d \n\n", errcode);
		goto termination;
		/* 
		1 : ���� ���� �Ұ���
		2 : �޸� �Ҵ� �Ұ�
		3 : �޸� ������, ����� 
		4 : ġ������ �޸� �̽�
		*/
	}

termination:

	free(originalName);
	free(neoName);

	return 0;
}

int isBMP(char* BMP_Name)
{
	int stat = 0;

	// ���ڿ��� �ϳ��� �˻��ϸ鼭 .bmp�� �ùٸ��� ����ִ��� �˻�
	for (int i = 0; i < strlen(BMP_Name); i++)
	{
		if (BMP_Name[i] == '.')
			stat++;
		else if (stat == 1 && BMP_Name[i] == 'b')
			stat++;
		else if (stat == 2 && BMP_Name[i] == 'm')
			stat++;
		else if (stat == 3 && BMP_Name[i] == 'p')
			stat++;
	}

	// ������ �������� 1�� ��ȯ -> if�������� true
	if (stat != 4) return 1;

	else return 0;
}

int addName(char* oldName, char** newName, unsigned int mode)
// �ʿ信 ���� ������� ���ڿ����̸� �ٲܰ��̱� ������, realloc ��� -> ���ο� �޸𸮰��� ���Ҵ�
{
	/*-------------------------------------------------*/
	// 1. newName ũ�⸦ �ٲٱ�
	/*-------------------------------------------------*/

	char* errCode = realloc(*newName, sizeof(char) * (STR_LEN + 30));

	if (errCode == NULL)
	{
		printf("realloc Error Occured! \n");
		return 1;
	}

	*newName = errCode;
	// realloc�� �ϸ� ���ο� �ּҰ��� ��ȯ�ϱ⿡, newName�� ���������ͷ� �޾Ҵ���

	/*-------------------------------------------------*/
	// 2. mode�� ���� �Ĺ��̸� ����
	/*-------------------------------------------------*/

	char additional_Name[30];

	switch (mode)
	{
	case 1:
		strcpy(additional_Name, "_blurred");
		break;
	case 2:
		strcpy(additional_Name, "_sharppend");
		break;
	case 3:
		strcpy(additional_Name, "_mid_filtered");
		break;
	case 4:
		strcpy(additional_Name, "_edge_detected");
		break;
	case 5:
		strcpy(additional_Name, "_rotated");
		break;
	case 6:
		strcpy(additional_Name, "minimized");
		break;
	case 7:
		strcpy(additional_Name, "_magnified");
		break;
	case 8:
		strcpy(additional_Name, "_equalized");
		break;
	case 9:
		strcpy(additional_Name, "_stretched");
		break;
	case 10:
		strcpy(additional_Name, "_embossing");
		break;
	case 11:
		strcpy(additional_Name, "_binarizated");
		break;
	case 12:
		strcpy(additional_Name, "_inverted");
		break;

	default:
		printf("mode is invaild! \n");
		return 1;
	}

	/*-------------------------------------------------*/
	// 3. oldName���� .�� �ε��� ��ġ�� ã�� �ڵ�
	/*-------------------------------------------------*/

	int null_idx = strlen(oldName);

	for (int i = 0; i < strlen(oldName); i++)
	{
		if (oldName[i] == '.')
		{
			null_idx = i;
			break;
		}
	}

	/*-------------------------------------------------*/
	// 4. newName�� �����
	/*-------------------------------------------------*/
		
	strncpy(*newName, oldName, null_idx); // .bmp�������� ����
	(*newName)[null_idx] = NULL; //strcpy�� �����Ṯ�ڸ� �߰� ����
	// ����, strcat�� ��ó�� NULL�� �ε����� ���ڿ��� �߰���

	strcat(*newName, additional_Name);
	strcat(*newName, ".bmp");

	return 0;
}