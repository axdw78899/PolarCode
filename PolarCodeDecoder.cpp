#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
#include<math.h>
//Setting
const uint8_t N = 8;
const uint8_t n = 4;
uint8_t A[N][n], ML_decision[N][n];
uint8_t Frozen[N] = { 0,0,0,1,0,1,1,1 };
float Decoder[N][n];
//Function: add noise
float add_gassrand(float EbNo);
float gaussrand();
//Function: ML CNOT decision
float f(float Lin1, float Lin2);
float g(float Lin1, float Lin2, uint8_t b);

//main
int main() {
	srand((unsigned)time(NULL));
	float EbNodB;
	EbNodB = 4.0; //EbNO = 10^(EbNOdB/10);

	int MF;//Decison Message or Frozen
	for (MF = 0; MF < N; MF++)
	{
		if (Frozen[MF])
			A[MF][0] = 1;
		else
			A[MF][0] = 0;
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
	//BPSK
	for (a = 0; a < N; a++)
		Decoder[a][n - 1] = A[a][n - 1] ? -1.0 : 1.0;
	//add noise
	add_gassrand(EbNodB);
	//Decoder

	int j, i, u = N;
	//Decoder :f function 
	for (j = n - 1; j > 0; j--)
	{
		u /= 2;
		for (i = 0; i < u; i++) {
			Decoder[i][j - 1] = f(Decoder[i][j], Decoder[i + u][j]);
		}
	}

	for(i = 0;i < N-3; i=i+4)
	{
		if (Frozen[i] == 0)
			ML_decision[i][0] = 0;
		else
			ML_decision[i][0] = Decoder[i][0] > 0.0 ? 0 : 1;
		//Decoder :g function  
		Decoder[i + 1][0] = g(Decoder[i][1], Decoder[i + 1][1], ML_decision[i][0]);


		//bit 2 (i=1+1=2)
		if (Frozen[i + 1] == 0)
			ML_decision[i + 1][0] = 0;
		else
			ML_decision[i + 1][0] = Decoder[i + 1][0] > 0.0 ? 0 : 1;
		//bit 1 & bit 2 do CNOT gate
		//gain line 2's ML_decision
		ML_decision[i][1] = ML_decision[i][0] ^ ML_decision[i + 1][0];
		ML_decision[i + 1][1] = ML_decision[i + 1][0];

		//line 2
		//ML_decision & Decoder do g funtion
		Decoder[i + 2][1] = g(Decoder[i][2], Decoder[i + 2][2], ML_decision[i][1]);
		Decoder[i + 3][1] = g(Decoder[i + 1][2], Decoder[i + 3][2], ML_decision[i + 1][1]);
		//back line 1 do f funtion
		Decoder[i + 2][0] = f(Decoder[i + 2][1], Decoder[i + 3][1]);
		//bit 3 (i=1+2=3)
		if (Frozen[i + 2] == 0)
			ML_decision[i + 2][0] = 0;
		else
			ML_decision[i + 2][0] = Decoder[i + 2][0] > 0.0 ? 0 : 1;

		Decoder[i + 3][0] = g(Decoder[i + 2][1], Decoder[i + 3][1], ML_decision[i + 2][0]);
		//bit 4 (i=1+3=4)
		if (Frozen[i + 3] == 0)
			ML_decision[i + 3][0] = 0;
		else
			ML_decision[i + 3][0] = Decoder[i + 3][0] > 0.0 ? 0 : 1;

		ML_decision[i + 2][1] = ML_decision[i + 2][0] ^ ML_decision[i + 3][0] ? 1 : 0;
		ML_decision[i + 3][1] = ML_decision[i + 3][0];

	}



	//output
	printf("Input: ");
	for (a = 0; a < N; a++)
		printf("%d ", A[a][0]);
	printf("\noutput:");
	for (a = 0; a < N; a++)
		printf("%d ", ML_decision[a][0]);
	//result
	int w = 0;
	for (int i = 0; i < N; i++)
	{
		if (A[i][0] != ML_decision[i][0])
			w++;
	}
	if (w != 0)
		printf("\nresult：wrong\n");
	else
		printf("\nresult：right\n");
	return 0;
}

//random
float gaussrand() { //Box-Muller
	static float U, V;	//2 uniform distribution U&V  make 1 normal distribution
	static int phase = 0;
	float X;            //X=R*cos( Theta ) or R*sin( Theta ),Theta
	//R=sqrt(-2 * ln(U))
	if (phase == 0)
	{
		U = rand() / (RAND_MAX + 1.0);
		V = rand() / (RAND_MAX + 1.0);
		X = sqrt(-2.0 * log(U)) * sin(2.0 * 3.1415926 * V);
	}
	else {
		X = sqrt(-2.0 * log(U)) * cos(2.0 * 3.1415926 * V);
	}
	phase = 1 - phase;
	return X;
}

float add_gassrand(float EbNo) {
	int i;
	float Sigma2;//noise
	float Sigma;//noise 
	float Rate = (N / 2) / (float)N;//rate
	Sigma2 = (float)1 / (2 * Rate * pow(10, (EbNo / 10.0)));//方差
	Sigma = sqrtf(Sigma2);//標準差 
	for (i = 0; i < N; i++)
	{
		Decoder[i][n - 1] = 2 * (Decoder[i][n - 1] + gaussrand() * Sigma) / Sigma2;
	}
	return 0;
}

//CNOT function
float f(float Lin1, float Lin2)//f
{
	float Lout, s, min;
	int sign;
	s = Lin1 * Lin2;
	if (s > 0)
		sign = 1;
	else
		sign = -1;

	min = fabs(Lin1) <= fabs(Lin2) ? fabs(Lin1) : fabs(Lin2);
	Lout = sign * min;
	return Lout;
}

float g(float Lin1, float Lin2, uint8_t b)//g
{
	return (1 - 2 * b) * Lin1 + Lin2;
}
