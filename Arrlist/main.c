#include <stdio.h>
#include <stdlib.h>

#include "ArrList.h"

int main() {
	arrayList* myArrayList;
	myArrayList = creatArrayList(100);

	if (sizeArrayList(myArrayList) == 0) {
		printf("created\n");
	}
	insertArrayList(myArrayList, 0, 10);
	insertArrayList(myArrayList, 1, 20);
	insertArrayList(myArrayList, 2, 30);
	insertArrayList(myArrayList, 3, 40);

	printArrayList(myArrayList);

	deleteArrayList(myArrayList, 0);
	deleteArrayList(myArrayList, 2);
	deleteArrayList(myArrayList, 1);

	printArrayList(myArrayList);
}
