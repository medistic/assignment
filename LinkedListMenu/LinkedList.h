#include <stdio.h>
#include <stdlib.h>

typedef struct pointType {
	int x;
	struct pointType* next;
} PointType;

typedef struct linkedList {
	struct pointType* head;
	int size;
} LinkedList;

extern LinkedList* createLinkedList();
extern int destroyLinkedList(LinkedList* li);
extern int isEmptyLinkedList(LinkedList* li);
extern int sizeLinkedList(LinkedList* li);
extern LinkedList* insertFirstLinkedList(LinkedList* li, PointType item);
extern int printLinkedList(LinkedList* li);
extern LinkedList* insertLastLinkedList(LinkedList* li, PointType item);
extern LinkedList* deleteAtLinkedList(LinkedList* li, int at);
extern LinkedList* insertItemLinkedList(LinkedList* li, PointType* pre, PointType item);
extern PointType deleteFirstLinkedList(LinkedList* li);
extern PointType deleteLastLinkedList(LinkedList* li);
extern PointType getItemLinkedList(LinkedList* li, int pos);
extern LinkedList* replaceItemLinkedList(LinkedList* li, int pos, PointType item);
extern PointType* nextItemLinkedList(LinkedList* li, int pre);