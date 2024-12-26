import matplotlib.pyplot as plt

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
M = 10000

fig, ax = plt.subplots()

with open('konkurs-I-345615-dane.txt', 'r') as data:
	xs = list(map(float, data.readline().split()))
	ys = list(map(float, data.readline().split()))
	ts = [k / 27 for k in range(28)] # list(map(float, data.readline().split()))
	us = [k / M for k in range(M + 1)] # list(map(float, data.readline().split()))
	data.readline()

	print("xs", xs)
	print("ys", ys)
	print("ts", ts)
	print("us", us)

	S_x = NIFS3(ts, xs)
	S_y = NIFS3(ts, ys)

	plt.plot([S_x.interpolate(t) for t in us], [S_y.interpolate(t) for t in us], color='green')

#ax.set_ylim(0, -50)
plt.grid(False)
plt.show()
