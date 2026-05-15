#include "LinkedList.h"

LinkedList* createLinkedList() {
	struct linkedList* r;
	r = (struct linkedList*)malloc(sizeof(struct linkedList));

	r->head = (struct pointType*)NULL;
	r->size = 0;

	return r;
}

int destroyLinkedList(LinkedList* li) {
	struct pointType* nptr, * mptr;
	for (nptr = li->head; nptr != NULL; nptr = mptr) {
		mptr = nptr->next;
		free(nptr);
	}

	free(li);

	return 1;
}

int isEmptyLinkedList(LinkedList* li) {
	return (li->size == 0);
}

int sizeLinkedList(LinkedList* li) {
	return li->size;
}

LinkedList* insertFirstLinkedList(LinkedList* li, PointType item) {
	PointType* ptr = (PointType*)malloc(sizeof(PointType));
	*ptr = item;

	ptr->next = li->head;
	li->head = ptr;
	li->size++;

	return li;
}

int printLinkedList(LinkedList* li) {
	PointType* current = li->head;
	printf("LinkedList:\n");
	printf("LinkedList size: %d\n", li->size);

	for (int i = 0; i < li->size; i++) {
		printf("[%d] x:%d", 
			i, current->x);

		current = current->next;
	}
	return 1;
}

LinkedList* insertLastLinkedList(LinkedList* li, PointType item) {
	PointType* ptr = li->head;

	while (ptr->next != NULL) {
		ptr = ptr->next;
	}

	PointType* nPtr = (PointType*)malloc(sizeof(PointType));
	*nPtr = item;
	nPtr->next = NULL;

	ptr->next = nPtr;

	li->size++;

	return li;
}

LinkedList* deleteAtLinkedList(LinkedList* li, int at) {
	if (at >= 0 && at < li->size) {
		PointType* nPtr = li->head;

		for (int i = 0; i < at - 1; i++) {
			nPtr = nPtr->next;
		}

		if (at == 0) {
			PointType* fPtr = li->head;
			li->head = fPtr->next;
			printf("삭제된 값: %d\n", fPtr->x);
			free(fPtr);
		}
		else {
			PointType* fPtr = nPtr->next;
			nPtr->next = fPtr->next;
			printf("삭제된 값: %d\n", fPtr->x);
			free(fPtr);
		}

		li->size--;
		
	}
	return li;
}

LinkedList* insertItemLinkedList(LinkedList* li, PointType* pre, 
	PointType item) {

	PointType* nPtr = (PointType*)malloc(sizeof(PointType));
	*nPtr = item;

	if (pre != NULL) {
		nPtr->next = pre->next;
		pre->next = nPtr;
	}
	else {
		nPtr->next = li->head;
		li->head = nPtr;
	}

	li->size++;
	return li;
}

PointType deleteFirstLinkedList(LinkedList* li) {
	if (li->size != 0) {
		PointType* tPtr = li->head;
		li->head = tPtr->next;

		PointType temp = *tPtr;
		free(tPtr);

		li->size--;
		return temp;
	}
	else {
		return (PointType) { 0, 0 };
	}
}

PointType deleteLastLinkedList(LinkedList* li) {
	PointType* pre;
	PointType* iPtr;

	pre = li->head;

	if (pre == NULL) {
		return (PointType) { 0, 0 };
	}

	iPtr = pre->next;

	if (iPtr == NULL) {
		li->head = NULL;

		PointType temp;
		temp = *pre;

		free(pre);
		li->size--;

		return temp;
	}
	else {
		while (iPtr->next != NULL) {
			pre = iPtr;
			iPtr = iPtr->next;
		}

		PointType temp;
		temp = *iPtr;

		pre->next = NULL;
		free(iPtr);
		li->size--;

		return temp;
	}
}

PointType getItemLinkedList(LinkedList* li, int pos) {
	PointType* nPtr = li->head;
	for (int i = 0; i < pos; i++) {
		nPtr = nPtr->next;
	}
	return *nPtr;
}
LinkedList* replaceItemLinkedList(LinkedList* li, int pos, PointType item) {
	PointType* nPtr = li->head;
	for (int i = 0; i < pos; i++) {
		nPtr = nPtr->next;
	}
	nPtr->x = item.x;
	return li;
}
PointType* nextItemLinkedList(LinkedList* li, int pre) {
	PointType* nPtr = li->head;
	for (int i = 0; i < pre; i++) {
		nPtr = nPtr->next;
	}
	return nPtr->next;
}