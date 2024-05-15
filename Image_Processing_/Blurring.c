#include "Processing.h"

int blurring(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errcCode)
{
	// 순서도는 가우시안 커널을 기준으로 작성되었습니다! 

	/*------------------------------------*/
	// 1. 사용자에게 원하는 커널을 선택받는다
	/*------------------------------------*/

	int option;

	while (1)
	{
		printf("1. Average kernel\n");
		printf("2. Custom gaussian kernel\n");

		printf("\nChoose your kernel! : ");

		scanf("%d", &option);
		putchar('\n');

		if (option > 0 && option < 3)
			break;
	}

	int kernel_size = 0;
	int formula_seed = 0;

	/*------------------------------------*/
	// 2. 커널 크기를 입력받는다
	// 커널크기는 특유의 영역처리 때문에, 홀수의 제곱배이기 때문에, x에 들어갈 수를 입력받는다
	/*------------------------------------*/

	while (1) // 커널크기를 수열이라고 하면  일반항은 (2x-1)^2 이다!
	{
		printf("\nThis is how the kernel size general term is configured : (2x-1)^2\n");
		printf("To suppress other possible errors, limit the kernel size to 100 or less\n");
		printf("And kernel size must bigger than 1\n");
		printf("Please input your kernel size to X! : ");

		if (scanf("%d", &formula_seed) != 1)
		{
			// 소수같은 것을 입력했을경우, 입력 버퍼를 비우고 다시 시도
			printf("Invalid input. Please enter a valid integer.\n");
			while (getchar() != '\n'); // fflush(stdin) 을 해도 무한루프가 계속되기에 어쩔수 없이 전부 비워준다.
			continue;
		}

		kernel_size = (int)floor(pow(2 * formula_seed - 1, 2));
		printf("\nYour kernel size is (2 * %d - 1) ^2 = %d\n", formula_seed, kernel_size);

		if (kernel_size > 1 && kernel_size < 100)
		{
			printf("Your kernel size is valid!\n");
			break;
		}

		else
			printf("Invalid kernel size! Please retry!\n\n\n ");
	}

	int Sizeside = (int)floor(sqrt(kernel_size));

	/*------------------------------------*/
	// 3. 원하는 커널을 생성시킨다
	/*------------------------------------*/

	double* kernel = NULL;
	BYTE* temp_buffer = NULL;
	double* vector_buffer = NULL;

	switch (option)
	{
	case 1:
		kernel = gen_AVG_kernel(kernel_size);
		break;

	case 2:
		kernel = gen_GAU_kernel(kernel_size);
		break;
	}

	if (kernel == NULL)
	{
		printf("memory allocation failed! at making kernel process!\n");
		goto release;
	}

	/*------------------------------------*/
	// 6. 커널이 제대로 완성되어있는지 확인한다.
	/*------------------------------------*/

	double checkSum = 0;
	putchar('\n');
	putchar('\n');

	// 총합이 1인지 확인

	for (int i = 0; i < kernel_size; i++)
	{
		if (i % Sizeside == 0)
			putchar('\n');
		printf("%lf ", kernel[i]);
		checkSum += kernel[i];
	}

	if (checkSum == 1)
	{
		printf("\n\nThe sum of the kernels is 1!\nIt won't affect the overall pixel brightness in any way!\n");
		printf("Overall, your kernel is valid for blurring kernel!\n\n");
	}

	else
	{
		printf("\n\nThe sum of the kernels is NOT 1!\n");
		printf("Beware that memory overruns can occur during computation! \n\n");
		// 간혹가다가 총합이 1이 안될때도 있다. 이는 부동소수점 때문에 발생한 것이고, 약간의 전체적으로 픽셀값이 어두워질 수도 있지만
		// 우리눈으로보기엔 별로 티 안난다.
	}

	/*------------------------------------*/
	// 7. 행렬이 y축과 x축에 대칭인지 확인후 드디어 블러링 연산
	/*------------------------------------*/

	int Symmetric = 0;
	int result = 0;

	if (Symmetric = check_symmetry(kernel, kernel_size)) 
	// y축과 x축에 대칭이면, 분리가능한 커널로 볼 수있다.
	// 또한, 2차원 영역을 컨볼루션 하게 되면, y축과 x축에 대칭이어야 하기에 이또한 무시가 가능하다
	{
		printf("\n\nThe kernel is symmetric about both x and y axes\n");
		printf("As a result, your kernel is a detachable kernel!\n");
		printf("When calculating kernels, the time cost is reduced by 33\%! Compared to regular calculation\n\n");

		// 행과 열로 쪼개서 연산 (어차피 컨볼루션이라 쪼개기가 가능하다)

		temp_buffer = (BYTE*)malloc(infoheader->ImageSize * sizeof(BYTE));

		if (temp_buffer == NULL)
		{
			printf("Memory allocation Error! at generatimg tempBuffer\n");
			goto release;
		}

		vector_buffer = (double*)malloc(Sizeside * sizeof(double)); // 벡터 연산에 필요한 벡터버퍼 생성

		if (vector_buffer == NULL)
		{
			printf("Memory allocation Error! at generatimg vectorBuffer\n");
			goto release;
		}

		/*------------------------------------*/
		// 8. 올드버퍼 ★ 행벡터(x축) = 임시버퍼
		/*------------------------------------*/

		//system("cls");

		int idx;
		printf("Row Vector of kernel!\n");

		for (idx = 0; idx < Sizeside; idx++) // 커널의 행벡터(x축) 나열
		{
			vector_buffer[idx] = kernel[idx * Sizeside];
			printf("%lf ", vector_buffer[idx]);
		}

		printf("\nNow row vec calculation is start!\n\n"); // 가로부터 행렬계산

		for (int Y = 0; Y < infoheader->height; Y++)
		{
			for (int X = 0; X < infoheader->width; X++)
			{
				result = row_cal(old_buffer, vector_buffer, X, Y, infoheader, Sizeside);
				temp_buffer[Y * infoheader->width + X] = (BYTE)result;
				//printf("%d ", (int)result);
			}
		}

		/*------------------------------------*/
		// 9. 임시버퍼 ★ 열벡터(y축) = 뉴버퍼
		/*------------------------------------*/

		//system("cls");

		printf("Column Vector of kernel!\n");
		double N = kernel[0];
		for (idx = 0; idx < Sizeside; idx++) // 커널의 행벡터(x축)
		{
			vector_buffer[idx] = kernel[idx * Sizeside] / N; // 정규화
			//printf("%lf ", vector_buffer[idx]);
		}

		printf("\nNow col vec calculation is start!\n"); // 그 후에 세로로 행렬계산

		for (int Y = 0; Y < infoheader->height; Y++)
		{
			for (int X = 0; X < infoheader->width; X++)
			{
				result = col_cal(temp_buffer, vector_buffer, X, Y, infoheader, Sizeside);
				new_buffer[Y * infoheader->width + X] = (BYTE)result;
				//printf("%d ", (int)result);
			}
		}
	}

	else // 분리가 불가능하면
	{
		printf("\n\nThe kernel is NOT symmetric about both x and y axes\n");
		printf("As a result, your kernel is NOT a detachable kernel!\n");

		flipping(kernel, Sizeside);
		// 컨볼루션이기에 -기호는 y축과 x축에 대칭일것이다.

		// 행벡터와 열벡터로 쪼갤수가 없기에, 일반연산
		for (int Y = 0; Y < infoheader->height; Y++)
			for (int X = 0; X < infoheader->width; X++)
				new_buffer[Y * infoheader->width + X] = regular_cal(old_buffer, kernel, X, Y, infoheader, Sizeside);
	}

	/*------------------------------------*/
	// 10. 정리하기
	/*------------------------------------*/

release:

	if (kernel != NULL) free(kernel);
	if (temp_buffer != NULL) free(temp_buffer);
	if (vector_buffer != NULL) free(vector_buffer);
	return;
}

void flipping(double* kernel, int Sizeside)
{
	int Size = (int)pow(Sizeside, 2);
	int Mid = Size / 2 + 1;
	int idx;

	for (idx = 0; idx <= Mid; idx++) // y축과 x축에 대해 flip 연산
		swap(BYTE, kernel[idx], kernel[(Size - 1) - idx]);

	return;
}

double* gen_AVG_kernel(int size)
{
	// 여기서, 커널을 동적할당한다
	double* AVG_kernel = (double*)malloc(sizeof(double) * size);
	double sum = 0;
	int idx;

	for (idx = 0; idx < size; idx++)
	{
		AVG_kernel[idx] = 1;
		sum += AVG_kernel[idx];
	}


	for (idx = 0; idx < size; idx++)
		AVG_kernel[idx] = 1 / sum;


	// 평균필터 완성!

	return AVG_kernel;
}

double* gen_GAU_kernel(int size)
{
	/*------------------------------------*/
	// 4. 커널의 분포를 결정하는 시그마 값을 입력
	/*------------------------------------*/

	// 표준편차는 커널의 폭을 제어하여, 스무딩 강도를 조절한다.
	// 표준편차는 말그대로, 각 측정값과 해당 그룹의 평균과의 떨어진것의 평균이다
	// 첨도와 어느정도 관계가 있을 줄 알았지만, 전혀 관계가 없다.

	int side = (int)floor(sqrt(size));

	double* GAU_kernel = (double*)malloc(sizeof(double) * size);

	if (GAU_kernel == NULL)
		goto exit;

	printf("You selected Gaussian kernel!\n");
	printf("To create a custom Gaussian kernel, you need to enter an additional sigma value!\n");

	double sigma = 1;

	while (1)
	{
		printf("\n\nThe default value for sigma is 1!\n");
		printf("If you raise the sigma value, the smoothing ability becomes stronger.\n");
		printf("And if you lower it, the smoothing becomes weaker\n");
		printf("Input your sigma Value (sigma > 0) : ");

		if (scanf("%lf", &sigma) != 1 || sigma <= 0)
		{
			printf("Invalid sigma value. Please enter a positive number.\n");
			while (getchar() != '\n'); // fflush(stdin) 으로 안되서 하나하나 비워줘야한다;;;;
			continue;
		}
		else
			break;
	}

	/*------------------------------------*/
	// 5. 2차원 가우시안 분포 필터 만들기
	/*------------------------------------*/

	int Radius = side / 2;
	double sum = 0;

	int Y, X;
	int idx;

	// Y와X값은 평균에 대칭임을 보이기 위해 일부러 -반경 ~ +반경 까지
	// 어차피 인덱스 계산할때, 반경을 더해주긴한다.
	for (Y = -Radius; Y <= Radius; Y++)
	{
		for (X = -Radius; X <= Radius; X++)
		{
			int idx = (Y + Radius) * side + (X + Radius);
			GAU_kernel[idx] = exp(-(X * X + Y * Y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
			sum += GAU_kernel[idx];
		}
	}

	for (idx = 0; idx < size; idx++)
		GAU_kernel[idx] /= sum;

	// 가우시안 필터 완성!

exit:
	return GAU_kernel;
}

int check_symmetry(double* kernel, int size)
{
	int side = (int)floor(sqrt(size));
	int Symmetric = 1;
	// 딱, y축&x축에 대칭인지를 파악하기 위해선 그냥, 맨 처음과 맨 끝부터 1씩 증가시킨게 똑같으면 된다.

	for (int i = 0; i < side; i++)
	{
		for (int j = 0; j < side; j++)
		{
			if (kernel[i * side + j] != kernel[(side - 1 - i) * side + (side - 1 - j)])
			{
				Symmetric = 0;
				break;
			}
		}
		if (!Symmetric)
			break;
	}

	return Symmetric;
}

BYTE row_cal(BYTE* old_buffer, double* row_vec, int curX, int curY, BITMAPINFOHEADER* infoheader, int Sizeside)
{
	double sum = 0;
	int wrapped_i = 0;

	int Height = infoheader->height;
	int Width = infoheader->width;
	int Radius = Sizeside / 2;
	int vector_idx, buffer_idx;

	for (int i = curY - Radius; i <= curY + Radius; i++) // i -> 행(y축)
	{
		wrapped_i = circular_wrapping(i, Height);
		buffer_idx = wrapped_i * Width + curX;
		vector_idx = i - curY + Radius;
		sum += (double)old_buffer[buffer_idx] * row_vec[vector_idx];
		//printf("buffer idx = %d, vector idx = %d\t", buffer_idx, vector_idx);
	}

	return (BYTE)round(sum);
}


BYTE col_cal(BYTE* temp_buffer, double* col_vec, int curX, int curY, BITMAPINFOHEADER* infoheader, int Sizeside)
{
	double sum = 0;
	int wrapped_j = 0;

	int Height = infoheader->height;
	int Width = infoheader->width;
	int Radius = Sizeside / 2;
	int vector_idx, buffer_idx;

	for (int j = curX - Radius; j <= curX + Radius; j++) // j -> 열(x축)
	{
		wrapped_j = circular_wrapping(j, Width);
		buffer_idx = curY * Width + wrapped_j;
		vector_idx = j - curX + Radius;
		sum += (double)temp_buffer[buffer_idx] * col_vec[vector_idx];
		//printf("buffer idx = %d, vector idx = %d\t", buffer_idx, vector_idx);
	}

	return (BYTE)round(sum);
}

BYTE regular_cal(BYTE* old_buffer, double* kernel, int curX, int curY, BITMAPINFOHEADER* infoheader, int Sizeside)
{
	double sum = 0.0;

	int wrapped_j = 0; int wrapped_i = 0;

	int Height = infoheader->height;
	int Width = infoheader->width;
	int Radius = Sizeside / 2;
	int kernel_idx;

	for (int i = curY - Radius; i <= curY + Radius; i++)
	{
		wrapped_i = circular_wrapping(i, Height);

		for (int j = curX - Radius; j <= curX + Radius; j++)
		{
			wrapped_j = circular_wrapping(j, Width);
			kernel_idx = (i - curY + Radius) * Sizeside + (j - curX + Radius);
			sum += (double)old_buffer[wrapped_i * Width + wrapped_j] * kernel[kernel_idx];
		}
	}

	return (BYTE)round(sum);
}