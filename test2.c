/* ----------------------------------------------------------
 * file: test2.c
 * author: Haofeng Yang 3190105301@zju.edu.cn
 * 		   Yuesong Wang 3190105303@zju.edu.cn
 * 		   Jiong   Li 	3190105725@zju.edu.cn
 * ----------------------------------------------------------
 * function: calculate the product of two matrices
 * description: calculating the product of two matrices
 * ----------------------------------------------------------
 * create date:   2022/4/15 1:12 pm
 * last modified: 2022/4/15 3:34 pm
 * ----------------------------------------------------------
 * history: 2022/4/15 1:12 pm create the file
 *          2022/4/15 3:34 pm finish the codes
 */

// #include<stdio.h>

int main(void) {
	int Matrix_A[25][25];
	int Matrix_B[25][25];
	int Ma,Mb,Na,Nb;

	scanf("%d %d",&Ma,&Na); // input the column and row of 1st matrix
	for(int i = 0; i < Ma; i = i + 1 ) { // input the data of 1st matrix
		for(int j = 0; j < Na; j = j + 1 ) {
			scanf("%d",&Matrix_A[i][j]);
		}
	}

	scanf("%d %d",&Mb,&Nb); // input the column and row of 2nd matrix
	for(int i = 0; i < Mb; i = i + 1) { // input the data of 2nd matrix
		for(int j = 0; j < Nb; j = j + 1) {
			scanf("%d",&Matrix_B[i][j]);
		}
	}

	if(Na != Mb) {	// if the 1st matrix's column is not equal to the 2nd's row
		printf("Incompatible Dimensions\n"); // output error message
		return 0;
	}

	int Mc = Ma;
	int Nc = Nb;
	int sum ;
	for(int i = 0; i < Mc; i = i + 1) {
		for(int j = 0; j < Nc; j = j + 1) { //calculate every element of the target matrix
			sum = 0;
			for(int k = 0; k < Na; k = k + 1){
				sum = sum + Matrix_A[i][k]*Matrix_B[k][j];
			}
			printf("%10d",sum); // output the result
		}
		printf("\n");
	}

	return 0;
}
