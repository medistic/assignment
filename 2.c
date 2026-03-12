#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS

int main() {


    int neg = 0;
    float a;
    scanf("%f",a);
    int inte = a;
    float real = a - inte;

    if (inte < 0)
    {
        inte = -inte;
        real = -real;
        neg = 1;
    }
    int list[8] = { 0, };
    int arr1[8] = { 0, };
    int nth = 8;
    while (inte > 0) {
        list[nth - 1] = inte % 2;
        inte = inte / 2;
        nth = nth - 1;
    }

    if (neg == 1) {
        for (int i = 0; i < 8; i++) {
            if (list[i] == 1) {
                list[i] = 0;
            }
            else if (list[i] == 0) {
                list[i] = 1;
            }
        }
    }
    nth = 0;
    while (real != 0.0) {
        if (nth > 7) {
            break;
        }
        real = real * 2;
        inte = real;

        arr1[nth] = inte;

        real = real - inte;
        nth += 1;
    }
    if (neg == 1) {
        for (int i = 0; i < 8; i++) {
            if (arr1[i] == 1) {
                arr1[i] = 0;
            }
            else if (arr1[i] == 0) {
                arr1[i] = 1;
            }
        }
    }
    arr1[7] += 1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (arr1[j] == 2) {
                if (j == 0) {
                    list[7] += 1;
                }
                arr1[j] = 0;
                arr1[j - 1] += 1;
            }
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (arr1[j] == 2) {

                list[j] = 0;
                if (j == 0) {
                    break;
                }
                list[j - 1] += 1;
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        printf("%d", list[i]);
    }
    printf(".");
    for (int i = 0; i < 8; i++) {
        printf("%d", arr1[i]);
    }

}
