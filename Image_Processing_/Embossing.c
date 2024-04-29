#include "Processing.h"

int embossing(BYTE* old_buffer, BYTE* new_buffer, BITMAPINFOHEADER* infoheader, int* errCode)
{
	/*------------------------------*/
	// 1. 엠보싱 효과 역시 분리가능한 커널이 될 수 없다
	/*------------------------------*/

	double kernel[9] = { -1,-1,0,0,0,0,0,1,1 };
	// 픽셀의 대각선 방향으로 차이를 계산한다, 역시 총합은 0
	// 추가적인 픽셀처리와 함께라면 대각선 방향으로 빛이 드는듯한 효과를 준다
	
	/*------------------------------*/
	// 2. 엠보싱 처리 계산
	/*------------------------------*/

	printf("\nNow embossing calculation is start!\n");

	int temp;
	for (int h = 0; h < infoheader->height; h++)
	{
		for (int w = 0; w < infoheader->width; w++)
		{
			temp = 128 + sharp_cal(old_buffer, kernel, h, w, infoheader->width, infoheader->height);
			new_buffer[h * infoheader->width + w] = clipping(temp);
		}
	}
	
	// 해당 커널은 현재 총합이 0이기에, 엣지 디텍팅과 닮게된다. 이를 방지하기 위해, 128을 더해준다.

	return 0;
}