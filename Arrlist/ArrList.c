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
void reallocArrayList(arrayList* al) {
	int alSize = al->size;
	realloc(al, (alSize + 1) * sizeof(arrayList));
}
void distroyArrayList(arrayList* al) {
	free(al->data);
	free(al);
}

int isEmptyArrayList(arrayList* al) {
	if (al->size == 0) {
		return 1;
	}
	else {
		return 0;
	}
}
int isFullArrayList(arrayList* al) {
	if (al->size == al->capacity) {
		return 1;
	}
	else {
		return 0;
	}
}
int sizeArrayList(arrayList* al) {
	return al->size;
}

int insertArrayList(arrayList* al, int pos, elementArrayList item) {
	
	if (pos<0 || pos>al->size) {
		return 0;
	}
	else {
		for (int i = (al->size); i >= pos; i--) {
			al->data[i + 1] = al->data[i];
		}
		al->data[pos] = item;
	}
	al->size++;
	return 1;
}
elementArrayList deleteArrayList(arrayList* al, int pos) {
	elementArrayList item = al->data[pos];
	
	if (pos<0 || pos>al->size) {
		return 0;
	}

	

	else {
		for (int i = pos; i < (al->size)-1; i++) {
			al->data[i] = al->data[i+1];
		}

	}
	al->size--;
	return item;
}
void initArrayList(arrayList* al) {
	for (int i = (al->size)-1; i >=0; i--) {
		al->data[i] = 0;
	}
}

elementArrayList getitemArrayList(arrayList* al, int pos) {
	return al->data[pos];
}
int replaaceItemArrayList(arrayList* al, int pos, elementArrayList item) {
	al->data[pos] = item;
	return 1;
}

elementArrayList printArrayList(arrayList* al) {
	printf("arraylist:");
	for (int i = 0; i < al->size; i++) {
		printf("%d ", al->data[i]);
	}
	printf("\n");
	return 1;
}
