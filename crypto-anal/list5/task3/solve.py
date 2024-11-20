from fractions import Fraction
from random import randint
from math import isqrt, gcd, floor, log2
from Crypto.Util import number
import matplotlib.pyplot as plt

def square_root(x):
	if x < 0:
		return -1
	
	y = isqrt(x)
	
	if y * y == x:
		return y
	return -1

def ext_gcd(a, b):
	if a == 0:
		return (b, 0, 1)
	r = b // a
	g, x, y = ext_gcd(b - r * a, a)
	return (g, y - r * x, x)

def attack(n, e):
	x = Fraction(e, n)
	p_old = (1, 0)
	q_old = (0, 1)

	while True:
		p_1, p_2 = p_old
		q_1, q_2 = q_old

		a_i = floor(x)

		p = a_i * p_1 + p_2
		q = a_i * q_1 + q_2

		# print(i, Fraction(p, q), Fraction(p, q) - Fraction(e, n))
		
		if p > 0 and (e * q - 1) % p == 0:
			c_i = (e * q - 1) // p

			A = 1
			B = -(n - c_i + 1)
			C = n

			delta = square_root(B * B - 4 * A * C)

			if delta != -1 and (-B + delta) % (2 * A) == 0 and \
							   (-B - delta) % (2 * A) == 0:
				P = (-B + delta) // (2 * A)
				Q = (-B - delta) // (2 * A)

				if P * Q == n and P > 1 and Q > 1:
					return (P, Q)
		
		if Fraction(a_i, 1) == x:
			break

		x = Fraction(1) / (x - Fraction(a_i))

		p_old = (p, p_1)
		q_old = (q, q_1)
	
	return None

P = []

for i in range(10):
	success = 0

	for rep in range(1024):
		continue

		p = number.getPrime(513)
		q = number.getPrime(513)

		print(i, rep, p, q)
		
		assert 2 ** 512 <= p < 2 ** 513
		assert 2 ** 512 <= q < 2 ** 513

		phi = (p - 1) * (q - 1)

		d = randint(2 ** (253 + i), 2 ** (254 + i))
		while gcd(phi, d) != 1:
			d = randint(2 ** (253 + i), 2 ** (254 + i))
	
		n = p * q

		_, e, _ = ext_gcd(d, phi)
		e %= phi

		res = attack(n, e)
		if res != None:
			assert (p, q) == res or (q, p) == res
			success += 1
	
	if success == 0:
		break

	print(i, success / 1024, 0 if success == 0 else -log2(success / 1024))
	
	P.append(-log2(success / 1024))

P = [-0.0, -0.0, 0.31000202858055464, 2.011315313227834, 4.045803689613125, 5.356143810225276, 8.415037499278844, 10.0]
print(P)

plt.plot(range(len(P)), P, marker='o')
plt.xlabel('i')
plt.ylabel('P_i')
plt.grid(True)
plt.show()
