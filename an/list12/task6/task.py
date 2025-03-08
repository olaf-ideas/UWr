import numpy as np
from functools import reduce
import scipy.integrate as integrate

def NC_integration(f, a, b, n):	
	
	x = np.linspace(a, b, n + 1)
	y = f(x)

	P = reduce(np.polymul, [[1, -x[i]] for i in range(n + 1)])
	
	A = [0 for i in range(n + 1)]
	for k in range(n // 2 + 1):
		Q = np.polyint(np.polydiv(P, [1, -x[k]])[0])
		A[k] = np.polyval(Q, b) - np.polyval(Q, a)

		for i in range(n + 1):
			if k != i:
				A[k] /= x[k] - x[i]

	for i in range(n // 2 + 1, n + 1):
		A[i] = A[n - i]
	
	#print(A)
	
	return np.dot(y, A)

def f1(x):
	return np.cos(x)

def f2(x):
	return x ** -1

def f3(x):
	return 1 / (1 + x ** 2)

for n in range(2, 26):
	print("n =", n)
	print(NC_integration(f1, -3, 4, n), integrate.quad(f1, -3, 4))
	print(NC_integration(f2, 1, 2, n), integrate.quad(f2, 1, 2))
	print(NC_integration(f3, -5, 5, n), integrate.quad(f3, -5, 5))
	print()
