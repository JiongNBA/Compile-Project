#include<stdio.h>

int main() {
	int Matrix_A[25][25];
	int Matrix_B[25][25];
	int Ma,Mb,Na,Nb;
	scanf("%d %d",&Ma,&Na);
	for(int i = 0; i < Ma; ++i) {
		for(int j = 0; j < Na; ++j) {
			scanf("%d",&Matrix_A[i][j]);
		}
	}
	scanf("%d %d",&Mb,&Nb);
	if(Na != Mb) {
	printf("Incompatible Dimensions/n");
	return 0;
	}
	for(int i = 0; i < Mb; ++i) {
		for(int j = 0; j < Nb; ++j) {
			scanf("%d",&Matrix_B[i][j]);
		}
	}
	int Mc = Ma;
	int Nc = Nb;
	int sum ;
	for(int i = 0; i < Mc; ++i) {
		for(int j = 0; j < Nc; ++j) {
			sum = 0;
			for(int k = 0; k < Na; ++k){
			sum = sum + Matrix_A[i][k]*Matrix_B[k][j];
			}
			printf("%10d",sum);
		}
		printf("\n");
	}
return 0;

}
