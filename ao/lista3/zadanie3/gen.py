from random import *

K = 6
s = 15
for i in range(K):
	cnt = randint(0, randint(0, s))
	s -= cnt
	print(cnt, end=' ')
print()
