
# factor 188623968747499
# 188623968747499: 11622419 16229321

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

n = 188623968747499
e = 3
c = 109688584900477
p = 11622419
q = 16229321

d = mod_inv(e, (p - 1) * (q - 1))

print(d)
print(int2str(pow(c, d, n)))
