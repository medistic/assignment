#include <stdio.h>
#include <stdlib.h>

#include "sparseMatrix.h"

sparseMatrix* createSparseMatrix(int row, int col) {
	sparseMatrix* sm = (sparseMatrix*)malloc(sizeof(sparseMatrix));
	sm->row = row;
	sm->col = col;	
	sm->al = createArrayList(row * col);
	return sm;
}

int addElementSparseMatrix(sparseMatrix* sm, int row, int col, int value) {
	int size;
	size = sizeArrayList(sm -> al);

	return insertArrayList(sm->al, size,
		(elementArrayList) {row, col, value});
}

int printSparseMatrix(sparseMatrix* sm) {
	printf("희소행렬입니다\n");

	printArrayList(sm->al);
	return 1;
}

sparseMatrix* transposeSparseMatrix(sparseMatrix* sm) {
	sparseMatrix* smTransposed = createSparseMatrix(sm->col, sm->row);
	int size = sizeArrayList(sm->al);

	for(int i = 0; i < size; i++) {
		elementArrayList element = getItemArrayList(sm->al, i);
		insertArrayList(smTransposed->al, i, (elementArrayList) {element.col, element.row, element.value});
	}

	return smTransposed;
}