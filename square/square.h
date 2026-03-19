typedef struct value {
	int den;
	int numer;
}fractions;

//통분하기
extern int denom(fractions i, fractions j);
//넓이,둘레
extern fractions ext(fractions i, fractions j);
extern fractions circum(fractions i, fractions j);
//약분하기
extern fractions abbre(fractions i);
//연산범위 검사
extern int testb(fractions i, fractions j);
