#include "Convert.h"
#define STR_LEN 100

int isBMP(char*);
int addName(char*, char**, unsigned int);

int main()
{
	char* originalName = (char*)malloc(sizeof(char) * STR_LEN);
	char* neoName = (char*)malloc(sizeof(char) * STR_LEN);

	/*-------------------------------------------------*/
	// 변환할 이미지 파일 이름 입력받기
	/*-------------------------------------------------*/
	
	do
	{
		printf("type vaild BMP name! \n");
		printf("the file name must end with \".bmp\" \n");
		printf("typing \'q\' or \'Q\' is Quit!\n");

		printf("File name: ");
		fgets(originalName, STR_LEN, stdin); // stdin에서 최대 99문자 + NULL을 읽어옴

		if (strcmp(originalName, "q\n") == 0 || strcmp(originalName, "Q\n") == 0)
			return 0;

		// fgets는 개행 문자까지 입력 받으므로, 개행 문자를 NULL로 바꿔줌
		size_t ln = strlen(originalName) - 1; // NULL 전의 개행을 NULL로~
		if (originalName[ln] == '\n')
			originalName[ln] = '\0';
	} while (isBMP(originalName));

	unsigned int mode = 0;
	

	/*-------------------------------------------------*/
	// 이미지 파일을 어떻게 변환할 것인가? 모드 입력받기
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
		getchar(); // stdin 버퍼 소비하기
	}

	if (mode == 13)
	{
		printf("existing.. \n");
		goto termination;
	}

	/*-------------------------------------------------*/
	// 모드에 따라서 처리결과 이미지 파일의 이름을 바꿔주기 + 본격적인 변환
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
		1 : 파일 열기 불가능
		2 : 메모리 할당 불가
		3 : 메모리 오버런, 언더런 
		4 : 치명적인 메모리 이슈
		5 : 0으로 나눌뻔함
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

	// 문자열을 하나씩 검사하면서 .bmp가 올바르게 들어있는지 검색
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

	// 오류가 있을때는 1을 반환 -> if문에서의 true
	if (stat != 4) return 1;

	else return 0;
}

int addName(char* oldName, char** newName, unsigned int mode)
// 필요에 따라 출력파일 문자열길이를 바꿀것이기 때문에, realloc 사용 -> 새로운 메모리공간 재할당
{
	/*-------------------------------------------------*/
	// 1. newName 크기를 바꾸기
	/*-------------------------------------------------*/

	char* errCode = realloc(*newName, sizeof(char) * (STR_LEN + 30));

	if (errCode == NULL)
	{
		printf("realloc Error Occured! \n");
		return 1;
	}

	*newName = errCode;
	// realloc을 하면 새로운 주소값을 반환하기에, newName을 이중포인터로 받았던것

	/*-------------------------------------------------*/
	// 2. mode에 따른 후미이름 선택
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
	// 3. oldName에서 .의 인덱스 위치를 찾는 코드
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
	// 4. newName을 만들것
	/*-------------------------------------------------*/
		
	strncpy(*newName, oldName, null_idx); // .bmp전까지만 복사
	(*newName)[null_idx] = NULL; //strcpy는 널종료문자를 추가 안함
	// 또한, strcat은 맨처음 NULL의 인덱스에 문자열을 추가함

	strcat(*newName, additional_Name);
	strcat(*newName, ".bmp");

	return 0;
}