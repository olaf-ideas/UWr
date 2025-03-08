import matplotlib.pyplot as plt
import numpy as np
import random

class NIFS3:
	def __init__(self, xs, ys):
		n = len(xs)

		self.xs = xs
		self.ys = ys
		self.M = [0 for i in range(n + 1)]

		def d(k):
			def f(xs, ys):
				if len(xs) == 1:
					return ys[0]
				return (f(xs[1:], ys[1:]) - f(xs[:-1], ys[:-1])) / (xs[-1] - xs[0])
			return 6 * f([xs[k - 1], xs[k], xs[k + 1]], [ys[k - 1], ys[k], ys[k + 1]])

		p, q, u = [0], [0], [0]
		for k in range(1, n - 1):
			h_k = xs[k] - xs[k - 1]
			h_k_plus1 = xs[k + 1] - xs[k]

			lambda_k = h_k / (h_k + h_k_plus1)

			p.append(lambda_k * q[k - 1] + 2)
			q.append((lambda_k - 1) / p[k])
			u.append((d(k) - lambda_k * u[k - 1]) / p[k])

		for k in range(n - 2, 0, -1):
			self.M[k] = u[k] + q[k] * self.M[k + 1]

	def interpolate(self, x):
		k = 1
		while self.xs[k] < x:
			k += 1

		h_k = self.xs[k] - self.xs[k - 1]

		return h_k ** (-1) * \
			   (((1 / 6) * self.M[k - 1] * (self.xs[k] - x) ** 3) + \
				((1 / 6) * self.M[k] * (x - self.xs[k - 1]) ** 3) + \
				((self.ys[k - 1] - (1 / 6) * self.M[k - 1] * h_k ** 2) * (self.xs[k - 0] - x)) + \
				((self.ys[k - 0] - (1 / 6) * self.M[k - 0] * h_k ** 2) * (x - self.xs[k - 1])))

def display_spline(x_data,y_data,t_data,u_data):
	S_x = NIFS3(t_data, x_data)
	S_y = NIFS3(t_data, y_data)

	xs, ys = [], []
	for u in u_data:
		xs.append(S_x.interpolate(u))
		ys.append(-S_y.interpolate(u))
	
	xs = np.array(xs)
	ys = np.array(ys)
	
	color = "#"+''.join([random.choice('0123456789ABCDEF') for _ in range(6)])
	
	plt.plot(xs, ys, color='red' , linewidth = 3)
	return plt 
