#include <stdio.h>

int main(){
    int discr = -5; // 어떤 보수를 사용하는지 판별하기 위해 음수를 변수로 설정
    if ( ~5 == discr){ // 1의 보수의 특징인 1은 0으로 0은 1로 바꾸는 성질을 같은 원리인 not연산자를 통해서 판별
        printf("1's complement");
    }
    else{
        printf("2's complement");
    }

    return 0;
}
