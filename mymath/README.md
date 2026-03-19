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

int lcm(int i, int j) 
	{
		int r;
		while (j != 0) {
			r = i % j;
			i = j;
			j = r;
		}
		return i;
	}

gcd = (i * j) / lcm(i, j);
