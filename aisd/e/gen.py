from random import *

n = 10**6
print(n)
L = 0
for _ in range(n):
	op = 'IIIDS'[randint(0, 4)]
	if L == 0:
		op = 'I'

	if op == 'I':
		print(op, randint(0, L), randint(0, +10**6))
		L += 1
	elif op == 'D':
		print(op, randint(1, L))
		L -= 1
	else:
		l = randint(1, L)
		r = randint(l, L)
		print(op, l, r)
