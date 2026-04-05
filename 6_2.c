#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>


int main() {

	int N = 0;
	scanf("%d", &N);
	if (N % 2 != 1) {
		printf("not odd number");
		return 0;

	}
	int** Nptr=NULL;
	Nptr = (int*)malloc((N * N) * sizeof(int));
	for (int i = 0; i < N; i++) {
		Nptr[i] = (int*)malloc((N * N) * sizeof(int));
	}
	
	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			Nptr[i][j] =  0 ;
		}
		
	}



	int col = 0;
	int row = N / 2;
	Nptr[row][col] = 1;
	for (int i = 2; i <= N * N; i++) {
		col--;
		row++;
		if (col < 0) {
			col = N - 1;
		}
		if (row == N)
		{
			row = 0;
		}
		if (Nptr[row][col] ==0 ) {
			Nptr[row][col] = i;
		}
		else {
			if (col == N - 1) {
				col = -1;
			}
			if (row == 0)
			{
				row = N;
			}
			col += 2;
			row--;
			Nptr[row][col] = i;
		}
	}

	for (int j = 0; j < N; j++) {
		for (int i = 0; i < N; i++) {
			printf(" %d ", Nptr[i][j]);
		}
		printf("\n");
	}
}
