#include <stdio.h>
#include <stdlib.h>

#include "ArrList.h"

arrayList* creatArrayList(int size) {
	arrayList* al;
	al = (arrayList*)malloc(sizeof(arrayList));
	al->data = (elementArrayList*)malloc(size * sizeof(elementArrayList));
	al->size = 0;
	al->capacity = size;

	return al;
}

void distroyArrayList(arrayList* al);

int isEmptyArrayList(arrayList* al);
int isFullArrayList(arrayList* al);
int sizeArrayList(arrayList* al);

int insertArrayList(arrayList* al, int pos, elementArrayList item);
elementArrayList deleteArrayList(arrayList* al, int pos);
void initArrayList(arrayList* al);

elementArrayList getitemArrayList(arrayList* al, int pos);
int replaaceItemArrayList(arrayList* al, int pos, elementArrayList item);

elementArrayList printArrayList(arrayList* al);
