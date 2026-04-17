#include <stdio.h>
#include <stdlib.h>

#include "sparseMatrix.h"

int main() {
	sparseMatrix* sm1 = createSparseMatrix(5, 5);
	addElementSparseMatrix(sm1, 2, 1, 10);
	addElementSparseMatrix(sm1, 3, 3, 15);
	addElementSparseMatrix(sm1, 4, 1, 3);

	printSparseMatrix(sm1);

	sparseMatrix* sm2 = createSparseMatrix(5, 5);
	addElementSparseMatrix(sm2, 2, 1, 10);
	addElementSparseMatrix(sm2, 3, 4, 15);
	addElementSparseMatrix(sm2, 4, 1, 3);

	printSparseMatrix(sm2);

	sparseMatrix* sm3 = addSparseMatrix(sm1, sm2);
	printf("add 직후\n");

	printSparseMatrix(sm3);
	printf("print 직후\n");
}