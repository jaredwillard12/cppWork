#include <stdio.h>
#include <math.h>
#include <vector>

int main(int argc, char* argv[]){
	double x[100];
	double y[100];
	double intervals = 100;
#pragma omp parallel
	{
	int i=0;
	#pragma omp for 
	for (i=intervals/2; i>0; i--){
		int index = intervals/2-i;
		x[index]=double(-1.0/100.0)*i;
	}
}
#pragma omp parallel
	{
	int i=0;
	#pragma omp for 
	for (i=0; i<intervals/2; i++){
		x[50+i]=double(1.0/100.0)*i;
	}
}
#pragma omp parallel
	{
	int i=0;
	#pragma omp for 
	for (i=0; i<intervals; i++){
		y[i]=(sqrt(1-pow(x[i],2)));
	}
}
	double area=0;
	for (int i=0; i<intervals; i++){
		area += (1/intervals * y[i]);
	}
	printf("%lf",area);
}
