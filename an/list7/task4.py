import numpy as np
import matplotlib.pyplot as plt

def chebyshev_nodes(n):
	return np.array([np.cos((2 * k - 1) / (2 * n) * np.pi) for k in range(1, n + 1)])

def polynomial(x, nodes):
	p = 1
	for xi in nodes:
		p *= (x - xi)
	return p

n_values = range(13, 14)

x = np.linspace(-1, 1, 1000)
plt.figure(figsize=(12, 8))

for n in n_values:
	equidistant_nodes = np.linspace(-1, 1, n + 1)
	equidistant_poly = [polynomial(xi, equidistant_nodes) for xi in x]

	chebyshev_nodes_list = chebyshev_nodes(n)
	chebyshev_poly = [polynomial(xi, chebyshev_nodes_list) for xi in x]
	
	plt.plot(x, equidistant_poly, label=f'Równoodległe (n={n})', alpha=0.6)
	plt.plot(x, chebyshev_poly, label=f'Czebyszew (n={n})', linestyle='dashed', alpha=0.6)

plt.xlabel('x')
plt.ylabel('pn+1(x)')
plt.axhline(0, color='black', linewidth=0.8, linestyle='--')
plt.legend(loc='upper left', bbox_to_anchor=(1.05, 1))
plt.grid()
plt.tight_layout()
plt.show()

