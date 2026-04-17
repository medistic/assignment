#include <stdio.h>
#include <stdlib.h>

#include "arrayList.h"

typedef struct sparseMatrixType {
	int row;          
	int col;          
	arrayList* al;  
} sparseMatrix;

extern sparseMatrix* createSparseMatrix(int row, int col);
extern int addElementSparseMatrix(sparseMatrix* sm, int row, int col, int value);
extern int printSparseMatrix(sparseMatrix* sm);
extern sparseMatrix* addSparseMatrix(sparseMatrix* sm1, sparseMatrix* sm2);
extern sparseMatrix* transposeSparseMatrix(sparseMatrix* sm);