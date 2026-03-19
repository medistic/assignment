typedef struct value{
	int den;
	int numer;
}fractions;

//통분하기
extern int denom( fractions i,fractions j);
//사칙연산
extern fractions add(fractions i, fractions j);
extern fractions minus(fractions i, fractions j);
extern fractions multi(fractions i, fractions j);
extern fractions divis(fractions i, fractions j);
//약분하기
extern fractions abbre(fractions i);
//연산범위 검사
extern int testb(fractions i, fractions j, char a);

