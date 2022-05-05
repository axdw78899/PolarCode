#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(){

int N=8; 
int n=4;
int A[N][n];
int Frozen[N]={0,0,0,1,0,1,1,1,};//Frozen bit
int MF;//Decison Message or Frozen

//Input message
for(MF=0;MF<N;MF++) 
{
	if(Frozen[MF]==1)
		A[MF][0]=1; //message bit
	else 
		A[MF][0]=0;//frozen bit
}

//Encoder
int h=N,a,b,c; 
for(a=0;a<n-1;a++)
{
	for(b=0;b<N;b=b+(2*N)/h)
	{
		for(c=b;c<b+N/h;c++)
		{
			A[c][a+1]=A[c][a]^A[c+N/h][a];
			A[c+N/h][a+1]=A[c+N/h][a];
			//printf("%d %d %d \n",a,b,c);
			//printf("	A[%d][%d]=A[%d][%d]^A[%d][%d] \n",c,a+1,c,a,c+N/h,a);
			//printf("	A[%d][%d]=A[%d][%d] \n",c+N/h,a+1,c+N/h,a);
		}
	}
	h/=2;
}
//output
printf("Input¡G\n");
for(a=0;a<N;a++)printf("%d ",A[a][0]);
printf("\nOutput¡G\n");
for(a=0;a<N;a++)printf("%d ",A[a][n-1]);

return 0;
}


