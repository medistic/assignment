#include <stdio.h>
#include <stdlib.h>

#include "arrayList.h"

int main() {
	arrayList* myArrayList;
	myArrayList = createArrayList(100);

	if (sizeArrayList(myArrayList) == 0) {
		printf("Create ok\n");
	}

	// Insert test
	insertArrayList(myArrayList, 0, 10); 
	insertArrayList(myArrayList, 1, 20);
	insertArrayList(myArrayList, 2, 30);
	insertArrayList(myArrayList, 0, 5);

	printArrayList(myArrayList);

	// Delete test
	deleteArrayList(myArrayList, 0);
	deleteArrayList(myArrayList, 1);
	deleteArrayList(myArrayList, 2);

	printArrayList(myArrayList);
	destroySparseMatrix(myArrayList);
}
