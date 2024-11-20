from fractions import Fraction
from math import *

def square_root(x):
	y = int(sqrt(x))

	while y * y > x:
		y -= 1
	while y * y < x:
		y += 1

	if y * y == x:
		return y
	return -1

n = 1966981193543797
e = 323815174542919

x = Fraction(e, n)
p_old = (1, 0)
q_old = (0, 1)

for i in range(25):
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

			print("P:", P, "Q:", Q)
	
	if Fraction(a_i, 1) == x:
		break

	x = Fraction(1) / (x - Fraction(a_i))

	p_old = (p, p_1)
	q_old = (q, q_1)
