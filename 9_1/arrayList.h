#include <stdio.h>
#include <stdlib.h>

typedef int elementArrayList;

typedef struct arrayListType {
	elementArrayList* data;
	int size;
	int capacity;
} arrayList;

// typedef struct arrayListType arrayList;

extern arrayList* createArrayList(int size);
extern void destroyArrayList(arrayList* al);
extern int isEmptyArrayList(arrayList* al);
extern int isFullArrayList(arrayList* al);
extern int sizeArrayList(arrayList* al);
extern int insertArrayList(arrayList* al, 
	int pos, elementArrayList item);
extern elementArrayList deleteArrayList(
	arrayList* al, int pos);
extern int initArrayList(arrayList* al);
extern elementArrayList getItemArrayList(
	arrayList* al, int pos);
extern int replaceItemArrayList(arrayList* al,
	int pos, elementArrayList item);
extern void printArrayList(arrayList* al);



/*
elementArrayList *data = malloc(...)
elementArrayList data[100] <=> int data[100]

*/
