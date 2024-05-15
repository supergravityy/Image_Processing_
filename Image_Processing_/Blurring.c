#include "Processing.h"

int blurring(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errcCode)
{
	// �������� ����þ� Ŀ���� �������� �ۼ��Ǿ����ϴ�! 

	/*------------------------------------*/
	// 1. ����ڿ��� ���ϴ� Ŀ���� ���ù޴´�
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
	// 2. Ŀ�� ũ�⸦ �Է¹޴´�
	// Ŀ��ũ��� Ư���� ����ó�� ������, Ȧ���� �������̱� ������, x�� �� ���� �Է¹޴´�
	/*------------------------------------*/

	while (1) // Ŀ��ũ�⸦ �����̶�� �ϸ�  �Ϲ����� (2x-1)^2 �̴�!
	{
		printf("\nThis is how the kernel size general term is configured : (2x-1)^2\n");
		printf("To suppress other possible errors, limit the kernel size to 100 or less\n");
		printf("And kernel size must bigger than 1\n");
		printf("Please input your kernel size to X! : ");

		if (scanf("%d", &formula_seed) != 1)
		{
			// �Ҽ����� ���� �Է��������, �Է� ���۸� ���� �ٽ� �õ�
			printf("Invalid input. Please enter a valid integer.\n");
			while (getchar() != '\n'); // fflush(stdin) �� �ص� ���ѷ����� ��ӵǱ⿡ ��¿�� ���� ���� ����ش�.
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
	// 3. ���ϴ� Ŀ���� ������Ų��
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
	// 6. Ŀ���� ����� �ϼ��Ǿ��ִ��� Ȯ���Ѵ�.
	/*------------------------------------*/

	double checkSum = 0;
	putchar('\n');
	putchar('\n');

	// ������ 1���� Ȯ��

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
		// ��Ȥ���ٰ� ������ 1�� �ȵɶ��� �ִ�. �̴� �ε��Ҽ��� ������ �߻��� ���̰�, �ణ�� ��ü������ �ȼ����� ��ο��� ���� ������
		// �츮�����κ��⿣ ���� Ƽ �ȳ���.
	}

	/*------------------------------------*/
	// 7. ����� y��� x�࿡ ��Ī���� Ȯ���� ���� ���� ����
	/*------------------------------------*/

	int Symmetric = 0;
	int result = 0;

	if (Symmetric = check_symmetry(kernel, kernel_size)) 
	// y��� x�࿡ ��Ī�̸�, �и������� Ŀ�η� �� ���ִ�.
	// ����, 2���� ������ ������� �ϰ� �Ǹ�, y��� x�࿡ ��Ī�̾�� �ϱ⿡ �̶��� ���ð� �����ϴ�
	{
		printf("\n\nThe kernel is symmetric about both x and y axes\n");
		printf("As a result, your kernel is a detachable kernel!\n");
		printf("When calculating kernels, the time cost is reduced by 33\%! Compared to regular calculation\n\n");

		// ��� ���� �ɰ��� ���� (������ ��������̶� �ɰ��Ⱑ �����ϴ�)

		temp_buffer = (BYTE*)malloc(infoheader->ImageSize * sizeof(BYTE));

		if (temp_buffer == NULL)
		{
			printf("Memory allocation Error! at generatimg tempBuffer\n");
			goto release;
		}

		vector_buffer = (double*)malloc(Sizeside * sizeof(double)); // ���� ���꿡 �ʿ��� ���͹��� ����

		if (vector_buffer == NULL)
		{
			printf("Memory allocation Error! at generatimg vectorBuffer\n");
			goto release;
		}

		/*------------------------------------*/
		// 8. �õ���� �� �຤��(x��) = �ӽù���
		/*------------------------------------*/

		//system("cls");

		int idx;
		printf("Row Vector of kernel!\n");

		for (idx = 0; idx < Sizeside; idx++) // Ŀ���� �຤��(x��) ����
		{
			vector_buffer[idx] = kernel[idx * Sizeside];
			printf("%lf ", vector_buffer[idx]);
		}

		printf("\nNow row vec calculation is start!\n\n"); // ���κ��� ��İ��

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
		// 9. �ӽù��� �� ������(y��) = ������
		/*------------------------------------*/

		//system("cls");

		printf("Column Vector of kernel!\n");
		double N = kernel[0];
		for (idx = 0; idx < Sizeside; idx++) // Ŀ���� �຤��(x��)
		{
			vector_buffer[idx] = kernel[idx * Sizeside] / N; // ����ȭ
			//printf("%lf ", vector_buffer[idx]);
		}

		printf("\nNow col vec calculation is start!\n"); // �� �Ŀ� ���η� ��İ��

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

	else // �и��� �Ұ����ϸ�
	{
		printf("\n\nThe kernel is NOT symmetric about both x and y axes\n");
		printf("As a result, your kernel is NOT a detachable kernel!\n");

		flipping(kernel, Sizeside);
		// ��������̱⿡ -��ȣ�� y��� x�࿡ ��Ī�ϰ��̴�.

		// �຤�Ϳ� �����ͷ� �ɰ����� ���⿡, �Ϲݿ���
		for (int Y = 0; Y < infoheader->height; Y++)
			for (int X = 0; X < infoheader->width; X++)
				new_buffer[Y * infoheader->width + X] = regular_cal(old_buffer, kernel, X, Y, infoheader, Sizeside);
	}

	/*------------------------------------*/
	// 10. �����ϱ�
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

	for (idx = 0; idx <= Mid; idx++) // y��� x�࿡ ���� flip ����
		swap(BYTE, kernel[idx], kernel[(Size - 1) - idx]);

	return;
}

double* gen_AVG_kernel(int size)
{
	// ���⼭, Ŀ���� �����Ҵ��Ѵ�
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


	// ������� �ϼ�!

	return AVG_kernel;
}

double* gen_GAU_kernel(int size)
{
	/*------------------------------------*/
	// 4. Ŀ���� ������ �����ϴ� �ñ׸� ���� �Է�
	/*------------------------------------*/

	// ǥ�������� Ŀ���� ���� �����Ͽ�, ������ ������ �����Ѵ�.
	// ǥ�������� ���״��, �� �������� �ش� �׷��� ��հ��� ���������� ����̴�
	// ÷���� ������� ���谡 ���� �� �˾�����, ���� ���谡 ����.

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
			while (getchar() != '\n'); // fflush(stdin) ���� �ȵǼ� �ϳ��ϳ� �������Ѵ�;;;;
			continue;
		}
		else
			break;
	}

	/*------------------------------------*/
	// 5. 2���� ����þ� ���� ���� �����
	/*------------------------------------*/

	int Radius = side / 2;
	double sum = 0;

	int Y, X;
	int idx;

	// Y��X���� ��տ� ��Ī���� ���̱� ���� �Ϻη� -�ݰ� ~ +�ݰ� ����
	// ������ �ε��� ����Ҷ�, �ݰ��� �����ֱ��Ѵ�.
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

	// ����þ� ���� �ϼ�!

exit:
	return GAU_kernel;
}

int check_symmetry(double* kernel, int size)
{
	int side = (int)floor(sqrt(size));
	int Symmetric = 1;
	// ��, y��&x�࿡ ��Ī������ �ľ��ϱ� ���ؼ� �׳�, �� ó���� �� ������ 1�� ������Ų�� �Ȱ����� �ȴ�.

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

	for (int i = curY - Radius; i <= curY + Radius; i++) // i -> ��(y��)
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

	for (int j = curX - Radius; j <= curX + Radius; j++) // j -> ��(x��)
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