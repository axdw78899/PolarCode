#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
int main() {
	int N = 8; //input
	int n = 4; //logN-1
	uint8_t A[8][4];
	uint8_t Frozen[8] = {0,0,0,1,0,1,1,1 };//Frozen bit
	int MF;//Decison Message or Frozen

	//Input message
	for (MF = 0; MF < N; MF++)
	{
		if (Frozen[MF] == 1)
			A[MF][0] = 1; //message bit
		else
			A[MF][0] = 0;//frozen bit
	}

	//Encoder
	int h = N, a, b, c;
	for (a = 0; a < n - 1; a++)
	{
		for (b = 0; b < N; b = b + (2 * N) / h)
		{
			for (c = b; c < b + N / h; c++)
			{
				A[c][a + 1] = A[c][a] ^ A[c + N / h][a];
				A[c + N / h][a + 1] = A[c + N / h][a];
			}
		}
		h /= 2;
	}
	//output
	printf("Input:\n");
	for (a = 0; a < N; a++)printf("%d ", A[a][0]);
	printf("\nOutput:\n");
	for (a = 0; a < N; a++)printf("%d ", A[a][n - 1]);

	return 0;
}
