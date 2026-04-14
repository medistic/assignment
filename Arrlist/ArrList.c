#include <stdio.h>
#include <stdlib.h>

typedef int elementArrayList;
typedef struct arrayListType {
	elementArrayList * data;
	elementArrayList size;
	elementArrayList capacity;

}arrayList;


//struct arrayListType {
//	elementArrayList* data;
//	elementArrayList size;
//	elementArrayList capacity;
//
//};
//typedef struct arrayListType arrayList;
//와 같다

extern arrayList* creatArrayList(int size);
extern void distroyArrayList(arrayList* al);

extern int isEmptyArrayList(arrayList* al);
extern int isFullArrayList(arrayList* al);
extern int sizeArrayList(arrayList* al);

extern int insertArrayList(arrayList* al, int pos, elementArrayList item);
extern elementArrayList deleteArrayList(arrayList* al, int pos);
extern void initArrayList(arrayList* al);

extern elementArrayList getitemArrayList(arrayList* al, int pos);
extern int replaaceItemArrayList(arrayList* al, int pos, elementArrayList item);

extern elementArrayList printArrayList(arrayList* al);
