from random import *

K = 4
s = 15
for i in range(K):
	cnt = randint(0, randint(0, s))
	s -= cnt
	print(cnt, end=' ')
print()
