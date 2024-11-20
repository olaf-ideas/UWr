from random import randint
from Crypto.Util import number

#def ext_gcd(a, b):
#	if a == 0:
#		return (b, 0, 1)
#	r = b // a
#	(g, x, y) = ext_gcd(b - r * a, a)
#	return (g, y - r * x, x)

def ext_gcd(a, b):
	x, y = 1, 0
	x1, y1, a1, b1 = 0, 1, a, b
	while b1 > 0:
		r = a1 // b1
		x, x1 = x1, x - r * x1
		y, y1 = y1, y - r * y1
		a1, b1 = b1, a1 - r * b1
	return a1, x, y

def crt(nm):
	res = (0, 1)

	for a, m in nm:
		b, n = res

		g, x, y = ext_gcd(m, n)
		assert (a - b) % g == 0

		res = (((b - a) % n * x % n // g * m + a) % (n * m // g), (n * m // g))

	return res[0]

def cube_root(x):
	L, R = 0, x

	while R - L > 1:
		M = (L + R) // 2

		if M ** 3 < x:
			L = M
		else:
			R = M
	
	return R

def bytes2int(b):
	return int.from_bytes(b, 'big')

def str2int(s):
	return bytes2int(s.encode())

def int2bytes(n):
	return n.to_bytes((n.bit_length() + 7) // 8, 'big')

def int2str(n):
	return int2bytes(n).decode()

p1, q1 = number.getPrime(1024), number.getPrime(1024)
p2, q2 = number.getPrime(1024), number.getPrime(1024)
p3, q3 = number.getPrime(1024), number.getPrime(1024)

e = 3

n1 = p1 * q1
n2 = p2 * q2
n3 = p3 * q3

original_m = str2int('Bardzo tajna wiadomosc') # randint(0, 2 ** 1024)

m1 = pow(original_m, e, n1)
m2 = pow(original_m, e, n2)
m3 = pow(original_m, e, n3)

m_3 = crt([(m1, n1), (m2, n2), (m3, n3)])

m = cube_root(m_3)

assert m == original_m
print("SUCCESS")
print(int2str(m))
