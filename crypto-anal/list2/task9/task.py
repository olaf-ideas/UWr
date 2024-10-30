import os

def my_randint(i):
	k = 0
	while 2 ** k < i:
		k += 1
 
	while True:
		j = int.from_bytes(os.urandom((k + 7) >> 3)) & ((1 << k) - 1)
		if j < i:
			return j

n = int(input())
p = [i for i in range(n)]

for i in range(n - 1, 0, -1):
	j = my_randint(i + 1)
	p[i], p[j] = p[j], p[i]

print(*p)
