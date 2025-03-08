import scipy.integrate as integrate
from functools import cache
import math

import numpy as np
import matplotlib.pyplot as plt

def Romberg_method(f, m, k, a, b):
	
	@cache
	def T(m, k):
		if m > 0:
			return (4 ** m * T(m - 1, k + 1) - T(m - 1, k)) / (4 ** m - 1)

		value = 0

		h_k = (b - a) / (2 ** k)
		for i in range(2 ** k + 1):
			now = f(a + h_k * i)
			if i == 0 or i == 2 * k:
				now /= 2

			value += now

		return h_k * value
	
	return T(m, k)

def compute_errors(f, a, b, max_m):
	result, error = integrate.quad(f, a, b)
	errors = np.full((max_m, max_m), np.nan)
	for m in range(max_m):
		for k in range(max_m - 1 - m):
			errors[m, k] = math.log(abs(result - Romberg_method(f, m, k, a, b)))
	print("Done computing errors")
	return errors

def plot_error_surface(errors, title):
    m_vals = np.arange(errors.shape[0])
    k_vals = np.arange(errors.shape[1])
    M, K = np.meshgrid(m_vals, k_vals, indexing="ij")

    fig = plt.figure(figsize=(10, 7))
    ax = fig.add_subplot(111, projection='3d')
    surf = ax.plot_surface(M, K, errors, cmap='viridis', edgecolor='k')

    ax.set_title(title)
    ax.set_xlabel("m (order of extrapolation)")
    ax.set_ylabel("k (subdivisions)")
    ax.set_zlabel("Error")
    fig.colorbar(surf, shrink=0.5, aspect=10)
    plt.show()

def example1(x):
	return 2025 * x ** 7 - 2006 * x ** 6 - 2016 * x ** 2

#print("--- Example 1 ---")
#test_Romberg_method(example1, -5, 3)

def example2(x):
	return 1 / (1 + 25 * x ** 2)

#print("--- Example 2 ---")
#test_Romberg_method(example2, -1, 1)

def example3(x):
	return math.cos(3 * x + math.pi / 4)

#print("--- Example 3 ---")
#test_Romberg_method(example3, -3 * math.pi, math.pi / 6)

max_m = 21

plot_error_surface(compute_errors(example1, -5, 3, max_m), "example 1")
#plot_error_surface(compute_errors(example2, -1, 1, max_m), "example 2")
#plot_error_surface(compute_errors(example3, -3 * math.pi, math.pi / 6, max_m), "example 3")
