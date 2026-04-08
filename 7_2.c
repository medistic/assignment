#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
	srand(time(NULL));

	struct student {
		char name[20];
		int SID;
		int grade;
		
	};
	int sum = 0;
	int maxim = -2147483647;
	int minim = 2147483647;
	int num = 0;
	scanf("%d", &num);
	if (num > 100) {
		printf("not good num");
		return 0;
	}

	struct student* people = (struct student*)malloc(sizeof(struct student) * num);

	FILE* fp;
	fp = fopen("name_list.txt", "r");

	if (fp == NULL) printf("fail to open file\n");
	

	for (int i = 0; i < num; i++) {
		(fgets((*(people+i)).name, sizeof((*(people + i)).name), fp));
	}

	for (int i = 0; i < num; i++) {
		(*(people + i)).SID = i + 1;
		(*(people + i)).grade = rand() % 101;
	}
	for (int i = 0; i < num; i++) {
		if ((*(people + i)).grade >= maxim) {
			maxim = (*(people + i)).grade;
		}
		if ((*(people + i)).grade <= minim) {
			minim = (*(people + i)).grade;
		}
	}
	for (int i = 0; i < num; i++) {
		printf("name:%s", (*(people + i)).name);
		printf("SID: %d\n", (*(people + i)).SID);
		printf("grade: %d\n", (*(people + i)).grade);
		printf("\n");
	}
	printf("minimum grade: %d\n", minim);
	printf("maximum grade: %d\n", maxim);

}
