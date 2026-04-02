#define N 11
#include <stdio.h>
#include <stdlib.h>

int main() {
	
	int a[N][N] = { 0 };
	
	a[N/2][0] = 1;
	
	int col = 0;
	int row = N / 2;
	for (int i = 2;i <= N * N;i++){
		col--;
		row++;
		if(col<0) {
			col = N - 1;
		}
		if (row == N)
		{
			row = 0;
		}
		if (a[row][col] == 0) {
			a[row][col] = i;
		}
		else {
			if (col == N-1) {
				col = -1;
			}
			if (row == 0)
			{
				row = N;
			}
			col+=2 ;
			row--;
			a[row][col] = i;
		}
	}
	
	for (int j = 0; j < N;j++) {
		for (int i = 0; i < N;i++) {
			printf(" %d ", a[i][j]);
		}
		printf("\n");
	}
}

