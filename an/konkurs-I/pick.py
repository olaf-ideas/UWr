import fitz
import matplotlib.pyplot as plt
from PIL import Image

pdf_path = "konkurs-I-napis-2024.pdf"
image_path = "konkurs-I-napis-2024.jpg"
output_path = "output.txt"

doc = fitz.open(pdf_path)[0]
doc.get_pixmap(dpi=300).save(image_path)

img = Image.open(image_path).convert("RGB")
width, height = img.size

pixels = img.load()
min_x, min_y, max_x, max_y = width, height, 0, 0

for y in range(height):
	for x in range(width):
		r, g, b = pixels[x, y]

		if r > 120 and g < 160 and b < 150:
			#print(r, g, b, x, y)
			min_x = min(min_x, x)
			min_y = min(min_y, y)
			max_x = max(max_x, x)
			max_y = max(max_y, y)

print(min_x, max_x, min_y, max_y)

image = plt.imread(image_path)

batches = []
current = []

def onclick(event):
	global current
	
	if event.button == 1:
		current.append((event.xdata, event.ydata))
		plt.scatter(event.xdata, event.ydata, c='red', s=5)
		plt.draw()
	elif event.button == 3:
		current.append((event.xdata, event.ydata))
		plt.scatter(event.xdata, event.ydata, c='yellow', s=5)
		plt.draw()

		batches.append(current)
		current = []
		print("Batch saved")

def onkeypress(event):
	if event.key == "enter":
		plt.close()

fig, ax = plt.subplots()
ax.imshow(image, interpolation='bilinear')

ax.set_xlim(min_x - 15, max_x + 15)
ax.set_ylim(max_y + 15, min_y - 15)

fig.canvas.mpl_connect('button_press_event', onclick)
fig.canvas.mpl_connect('key_press_event', onkeypress)
plt.show()

with open(output_path, "w") as file:
	for batch in batches:
		if len(batch) == 0:
			continue

		xs, ys = [], []
		
		for (x, y) in batch:
			xs.append(x)
			ys.append(y)
	
		M = 80

		ts = [i / (len(xs) - 1) for i in range(len(xs))]
		us = [i / (M - 1) for i in range(M)]

		print("xs", xs)
		print("ys", ys)
		print("ts", ts)
		print("us", us)

		file.write(" ".join(map(str, xs)) + '\n')
		file.write(" ".join(map(str, ys)) + '\n')
		file.write(" ".join(map(str, ts)) + '\n')
		file.write(" ".join(map(str, us)) + '\n')
		file.write("\n")

print("done")
