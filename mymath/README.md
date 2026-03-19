int a, b = 0, 0

int GCd, Lcm = 0,0

int min = 2147483647

int max = -2147483648

받아오기 a b

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

Gcd{
	for (i = max, i = < (a * b) ,i++) {
		if(i%a == 0 && i%b == 0)
		Gcd = i
}
}

Lcm{
	for (i = 2, i =< min ,i++) {
		if (a % i == 0 && b % i == 0)
		lcm = i
}
}
