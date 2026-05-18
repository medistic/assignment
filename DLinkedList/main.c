#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "DLinkedList.h"

int main() {
	DLinkedList* myList1;
	myList1 = createDLinkedList();

	int menu = -1;

	do {
		scanf(" %d", &menu);
		if (menu == 1) {
			PointType item;
			scanf(" %d", &item.x);
			insertFirstDLinkedList(myList1, item);
		}
		else if (menu == 2) {

			PointType item;
			scanf(" %d", &item.x);
			if (sizeDLinkedList(myList1) == 0) {
				insertFirstDLinkedList(myList1, item);
				continue;
			}
			insertLastDLinkedList(myList1, item);
		}
		else if (menu == 3) {
			PointType item;
			int at;
			scanf(" %d", &item.x);
			scanf(" %d", &at);


			insertAtDLinkedList(myList1, at, item);
		}

		else if (menu == 4) {
			int at;
			scanf(" %d", &at);
			if (myList1->size != 0) {
				int deleted;
				deleted = deleteAtDLinkedList(myList1, at).x;
				printf("%d",deleted );
			}
		}
		else if (menu == 5) {
			printDLinkedList(myList1);
		}
		else if (menu == 6) {
			printf("DLinkedList size: %d\n", sizeDLinkedList(myList1));
		}
		else if (menu == 0) {
			destroyDLinkedList(myList1);
		}
	} while (menu != 0);
}
