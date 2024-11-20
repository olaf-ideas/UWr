from sage.all import *

n, e, c = map(int, input().split())

def int2bytes(n):
	return n.to_bytes((n.bit_length()+7)//8, 'big')

def int2str(n):
	return int2bytes(n).decode()

def bytes2int(b):
	return int.from_bytes(b, 'big')

def str2int(s):
	return bytes2int(s.encode())

def ext_gcd(a, b):
	if a == 0:
		return (b, 0, 1)
	r = b // a
	g, x, y = ext_gcd(b - a * r, a)
	return (g, y - r * x, x)

def mod_inv(a, m):
	g, x, y = ext_gcd(a, m)
	assert g == 1
	return x % m

a = ceil(sqrt(n))
b2 = a ** 2 - n

while not is_square(b2):
	a += 1
	b2 = a ** 2 - n

p, q = a - sqrt(b2), a + sqrt(b2)

print("done")

d = mod_inv(e, (p - 1) * (q - 1))
d = int(d)

print(d)

m = pow(c, d, n)
print(m)
print(int2str(m))
assert m ** 3 < n
