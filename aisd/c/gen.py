from random import *

n = 5000
p = 1 #100
m = 10 ** 6 + 7
print(n, p, m)
for i in range(p):
	for j in range(3):
		for k in range(3):
			if randint(0, 1) == 0:
				print('x', end='')
			else:
				print('.', end='')
		print() 
