from sage.all import *

n, e, c = map(int, input().split())

def fermat(n):
	a = ceil(sqrt(n))
	b2 = a ** 2 - n
	while not is_square(b2):
		a += 1
		b2 = a ** 2 - n
	return (a - sqrt(b2), a + sqrt(b2))

def pollard(n, B):
	a = 2
	for p in primes(B):
		pp = 1
		while pp * p <= B:
			pp *= p
		a = pow(a, pp, n)
		g = gcd(a - 1, n)
		if 1 < g < n:
			return g
	return None

print(fermat(n))
