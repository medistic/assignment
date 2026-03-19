str a, b

int GCd, Lcm = 0,0

int min = 2147483647

int max = -2147483648

받아오기 a b
a,b의 자료형 판단후 정수일경우 정수로 반환

if (a < 0){

	print: not good num
}

if (b < 0) {

	print: not good num
	
}

if (a < min)
	min = a

if (b < min)
	min = b

if (a > max)
	max = a

if (b > max)
	max = b

Gcd(i,j){
	for (i = max, i = < (a * b) ,i++) {
		if(i%a == 0 && i%b == 0)
		Gcd = i
}
}

lcm = (i * j) / GCD(i, j);
