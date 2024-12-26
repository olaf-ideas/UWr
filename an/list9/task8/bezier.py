import matplotlib.pyplot as plt
import numpy as np
import math

class Bezier:
	def __init__(self, W, weights):
		self.W = W
		self.weights = weights

	def interpolate(self, t):
     
		def C(n, k):
			return math.comb(n, k)
     
		def B(n, k):
			return t ** k * (1 - t) ** (n - k) * C(n, k)

		n = len(self.W) - 1
	
		s = sum([B(n, i) * self.weights[i] for i in range(n + 1)])
  
		X, Y = 0, 0
		for i, (x, y) in enumerate(self.W):
			X += x * B(n, i) * self.weights[i] / s
			Y += y * B(n, i) * self.weights[i] / s

		return (X, Y)

W = [(39.5, 10.5), (30, 20), (6, 6), (13, -12), (63, -12.5), (18.5, 17.5), (48, 63), (7, 25.5), (48.5, 49.5), (9, 19.5), (48.5, 35.5), (59, 32.5), (56, 20.5)]
weights = [1, 2, 3, 2.5, 6, 1.5, 5, 1, 2, 1, 3, 5, 1]

bezier = Bezier(W, weights)

xs = []
ys = []

for t in np.linspace(0, 1, num=1000):
	x, y = bezier.interpolate(t)

	xs.append(x)
	ys.append(y)

print(xs, ys)

plt.plot(xs, ys, color='blue')
plt.grid(False)
plt.show()
