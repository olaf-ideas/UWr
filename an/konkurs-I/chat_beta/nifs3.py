import matplotlib.pyplot as plt
from matplotlib.patches import Circle
from PIL import Image
import fitz


class NIFS3:
	def __init__(self, xs, ys):
		n = len(xs)
		self.xs = xs
		self.ys = ys
		self.M = [0 for _ in range(n + 1)]

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
		return h_k ** (-1) * (
			((1 / 6) * self.M[k - 1] * (self.xs[k] - x) ** 3) +
			((1 / 6) * self.M[k] * (x - self.xs[k - 1]) ** 3) +
			((self.ys[k - 1] - (1 / 6) * self.M[k - 1] * h_k ** 2) * (self.xs[k] - x)) +
			((self.ys[k] - (1 / 6) * self.M[k] * h_k ** 2) * (x - self.xs[k - 1]))
		)


class DraggablePoint:
	def __init__(self, ax, editor, x, y, color='red'):
		self.ax = ax
		self.editor = editor
		self.circle = Circle((x, y), 5, color=color, alpha=0.8, picker=True)
		self.ax.add_patch(self.circle)
		self.press = None
		self.connect()

	def connect(self):
		self.cidpress = self.circle.figure.canvas.mpl_connect('button_press_event', self.on_press)
		self.cidrelease = self.circle.figure.canvas.mpl_connect('button_release_event', self.on_release)
		self.cidmotion = self.circle.figure.canvas.mpl_connect('motion_notify_event', self.on_motion)

	def on_press(self, event):
		if event.inaxes != self.ax or event.button != 1:
			return
		contains, _ = self.circle.contains(event)
		if not contains:
			return
		self.press = (self.circle.center, event.xdata, event.ydata)

	def on_motion(self, event):
		if self.press is None or event.inaxes != self.ax:
			return
		x0, y0 = self.press[0]
		dx = event.xdata - self.press[1]
		dy = event.ydata - self.press[2]
		self.circle.center = (x0 + dx, y0 + dy)
		self.editor.update_spline()

	def on_release(self, event):
		if self.press is None or event.button != 1:
			return
		self.press = None
		self.editor.update_spline()

	def remove(self):
		self.circle.remove()


# Rotate image and call editor
pdf_path = "konkurs-I-napis-2024.pdf"
image_path = "rotated_image.jpg"

img = Image.open(image_path).convert("RGB")
width, height = img.size

min_x, min_y, max_x, max_y = width, height, 0, 0

class ImageEditor:
	def __init__(self, image_path):
		self.image_path = image_path
		self.fig, self.ax = plt.subplots()
		self.ax.imshow(plt.imread(image_path), interpolation='bilinear')
		self.ax.set_title("Left-click to add points, drag to move, right-click to delete")
		self.ax.set_xlim(min_x - 15, max_x + 15)
		self.ax.set_ylim(max_y + 15, min_y - 15)

		self.points = []
		self.spline_line = None
		self.connect()

	def connect(self):
		self.cidclick = self.fig.canvas.mpl_connect('button_press_event', self.onclick)

	def onclick(self, event):
		if event.inaxes != self.ax:
			return
		if event.button == 1 and not event.key == 'shift':  # Left-click to add
			point = DraggablePoint(self.ax, self, event.xdata, event.ydata)
			self.points.append(point)
			self.update_spline()
		elif event.button == 3:  # Right-click to delete
			for point in self.points:
				contains, _ = point.circle.contains(event)
				if contains:
					point.remove()
					self.points.remove(point)
					self.update_spline()
					break

	def update_spline(self):
		if self.spline_line:
			self.spline_line.remove()

		if len(self.points) < 3:
			self.fig.canvas.draw()
			return

		# self.points.sort(key=lambda p: p.circle.center[0])
		xs, ys = zip(*[p.circle.center for p in self.points])
		ts = [i / (len(xs) - 1) for i in range(len(xs))]
		us = [i / 100 for i in range(100)]

		print("xs:", xs)
		print("ys:", ys)

		S_x = NIFS3(ts, xs)
		S_y = NIFS3(ts, ys)

		interpolated_xs = [S_x.interpolate(u) for u in us]
		interpolated_ys = [S_y.interpolate(u) for u in us]

		self.spline_line, = self.ax.plot(interpolated_xs, interpolated_ys, color='red')
		self.fig.canvas.draw()

pixels = img.load()

for y in range(height):
	for x in range(width):
		r, g, b = pixels[x, y]

		if r > 120 and g < 160 and b < 150:
			#print(r, g, b, x, y)
			min_x = min(min_x, x)
			min_y = min(min_y, y)
			max_x = max(max_x, x)
			max_y = max(max_y, y)

# Convert PDF to image
doc = fitz.open(pdf_path)[0]
doc.get_pixmap(dpi=300).save(image_path)

# Rotate image
with Image.open(image_path) as img:
    rotated = img.rotate(90, expand=True)
    rotated.save(image_path)

# Run the editor
editor = ImageEditor(image_path)
plt.show()

