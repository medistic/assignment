#include <stdio.h>
#include <stdlib.h>

typedef struct pointType {
	int x;
	struct pointType* prev;
	struct pointType* next;
} PointType;

typedef struct dLinkedList {
	PointType head;
	int size;
} DLinkedList;

extern DLinkedList* createDLinkedList();
extern void destroyDLinkedList(DLinkedList* dl);

extern int insertFirstDLinkedList(DLinkedList* dl, PointType item);
extern int insertLastDLinkedList(DLinkedList* dl, PointType item);
extern PointType deleteFirstDLinkedList(DLinkedList* dl);

extern void printDLinkedList(DLinkedList* dl);
extern int isEmptyDLinkedList(DLinkedList* dl);
extern int sizeDLinkedList(DLinkedList* dl);
extern PointType deleteAtDLinkedList(DLinkedList* dl, int at);
extern DLinkedList* insertItemDLinkedList(DLinkedList* dl, PointType* pre, PointType item);
extern PointType deleteLastDLinkedList(DLinkedList* dl);
extern DLinkedList* insertAtDLinkedList(DLinkedList* dl, int at, PointType item);
extern PointType deleteItemDLinkedList(DLinkedList* dl, PointType item);
extern PointType getItemDLinkedList(DLinkedList* dl, int at);
extern DLinkedList* replaceItemDLinkedList(DLinkedList* dl, int at, PointType item);
extern PointType nextItemDLinkedList(DLinkedList* dl, PointType* pre);