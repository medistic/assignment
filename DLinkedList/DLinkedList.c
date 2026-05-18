#include "DLinkedList.h"

DLinkedList* createDLinkedList() {
	DLinkedList* temp;
	temp = (DLinkedList*)malloc(sizeof(DLinkedList));
	temp->head.prev = &(temp->head);
	temp->head.next = &(temp->head);
	temp->size = 0;

	return temp;
}

void destroyDLinkedList(DLinkedList* dl) {
	while (dl->size != 0) {
		deleteFirstDLinkedList(dl);
	}

	free(dl);
}

int insertFirstDLinkedList(DLinkedList* dl, PointType item) {
	PointType* nPtr = (PointType*)malloc(sizeof(PointType));
	*nPtr = item;

	nPtr->prev = &(dl->head);
	nPtr->next = dl->head.next;
	dl->head.next = nPtr;
	nPtr->next->prev = nPtr;

	dl->size++;
	return 0;
}

int insertLastDLinkedList(DLinkedList* dl, PointType item) {
	PointType* nPtr = (PointType*)malloc(sizeof(PointType));
	*nPtr = item;

	nPtr->prev = dl->head.prev;
	nPtr->next = &(dl->head);
	nPtr->prev->next = nPtr;
	nPtr->next->prev = nPtr;

	dl->size++;
	return 0;
}

PointType deleteFirstDLinkedList(DLinkedList* dl) {
	while (!isEmptyDLinkedList(dl)) {
		PointType* temp = dl->head.next;
		PointType item = *temp;
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;

		dl->size--;
		free(temp);
		return item;
	}
	return (PointType) { 0, 0 };
}

void printDLinkedList(DLinkedList* dl) {
	printf("Print DLinkedList:\n");
	printf("Size: %d\n", dl->size);

	if (dl->size == 0) {
		return;
	}

	PointType* start = dl->head.next;

	for (int i = 0; i < dl->size; i++) {
		printf("[%d] x: %d\n",
			i, start->x);

		start = start->next;
	}
}
int isEmptyDLinkedList(DLinkedList* dl) {
	if (dl->size != 0) {
		return 0;
	}
	return 1;
}
int sizeDLinkedList(DLinkedList* dl) {
	return dl->size;
}

DLinkedList* insertItemDLinkedList(DLinkedList* dl, PointType* pre, PointType item) {
	PointType* nPtr = (PointType*)malloc(sizeof(PointType));
	*nPtr = item;

	nPtr->prev = pre;
	nPtr->next = pre->next;
	pre->next = nPtr;
	nPtr->next->prev = nPtr;

	dl->size++;
	return dl;
}
PointType deleteAtDLinkedList(DLinkedList* dl, int at) {
	while (!isEmptyDLinkedList(dl)) {
		PointType* temp = dl->head.next;
		for (int i = 0;i < at - 1;i++) {
			temp = temp->next;
		}
		PointType item = *temp;
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;

		dl->size--;
		free(temp);
		return item;
	}
	return (PointType) { 0, 0 };
}

PointType deleteLastDLinkedList(DLinkedList* dl) {
	while (!isEmptyDLinkedList(dl)) {
		PointType* temp = dl->head.prev;
		PointType item = *temp;
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;

		dl->size--;
		free(temp);
		return item;
	}
	return (PointType) { 0, 0 };
}
PointType deleteItemDLinkedList(DLinkedList* dl, PointType item) {
	while (!isEmptyDLinkedList(dl)) {
		PointType* temp = dl->head.next;
		if (temp->x == item.x) {
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			dl->size--;
			free(temp);
			return item;
		}
		do {
			temp = temp->next;
			if (temp == &(dl->head)) {
				return (PointType) { 0, 0 };
			}
		} while (temp->x != item.x);
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		dl->size--;
		free(temp);
		return item;
	}
	return (PointType) { 0, 0 };
}
PointType getItemDLinkedList(DLinkedList* dl, int at) {
	PointType* temp = dl->head.next;
	for (int i = 0;i < at - 1;i++) {
		temp = temp->next;
	}
	return *temp;
}

DLinkedList* replaceItemDLinkedList(DLinkedList* dl, int at, PointType item) {
	
	PointType* temp = dl->head.next;
	for (int i = 0;i < at - 1;i++) {
		temp = temp->next;
	}
	temp->x = item.x;
	return dl;
}

PointType nextItemDLinkedList(DLinkedList* dl, PointType* pre) {
	if (pre->next == &(dl->head)) {
		return (PointType) { 0, 0 };
	}
	return *(pre->next);
}

DLinkedList* insertAtDLinkedList(DLinkedList* dl, int at, PointType item) {
	
	if (at == 1) {
		insertFirstDLinkedList(dl,item);
		return dl;
	}
	PointType* temp = &(dl->head);
	for (int i = 0;i < at - 1;i++) {
		temp = temp->next;
	}

	insertItemDLinkedList(dl, temp, item); 
	return dl;
}