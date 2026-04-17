#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sparseMatrix.h"

int main() {
	srand(time(NULL));
	sparseMatrix* sm1 = createSparseMatrix(10, 10);
	int nonEmptyCount = 0;
	while(nonEmptyCount < 20){
		int row = rand() % 10;
		int col = rand() % 10;
		int value = rand() % 100 + 1;
		int isDuplicate = 0;
		for (int i = 0; i < sm1->al->size; i++) {
			if (sm1->al->data[i].row == row && sm1->al->data[i].col == col) {
				isDuplicate = 1;
				break;
			}
		}

		if (isDuplicate) {
			continue; // 중복이면 다시 뽑기
		}
		addElementSparseMatrix(sm1, row, col, value);
		nonEmptyCount++;

	}


	printSparseMatrix(sm1);
	printf("\n");

	printf("전치행렬입니다\n");
	sparseMatrix* sm2 = transposeSparseMatrix(sm1);

	printSparseMatrix(sm2);

	destroySparseMatrix(sm1);
	destroySparseMatrix(sm2);
}
