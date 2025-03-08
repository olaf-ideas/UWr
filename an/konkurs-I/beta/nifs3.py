'''
    Edytor NIFS3, umożliwia dokładne tworzenie i edytowanie rysunku z NIFS3.
    Tutorial:
        1) Wyklikaj lewym na główym wykresie punkty NIFS3 i je poprzestawiaj (lewy + SHIFT) puty efekt nas nie zadowala
        2) Klikająć 1 możemy włączać i wyłączać rysowanie punktów interpolacji $us$
        3) Następni na osi liczbowej dodajemy/przestawiamy/usuwamy punkty interpolacji $us$
        4) Gdy efekt literki nas zadowolił klikamy ENTER i przechodzimy do kolejnej literki
        5) Po naciśnięciu ENTER aktualna literka jest zapisywana do pliku
'''

import matplotlib.pyplot as plt
from matplotlib.patches import Circle
from PIL import Image
import fitz

'''
Implementacja NIFS3 z definicji.
'''
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

'''
Pomocna klasa, która umożliwa edytorowi przesuwanie punktów na wykresie.
'''
class DraggablePoint:
    def __init__(self, ax, editor, x, y, color='red', size=2):
        # print("punkcik: ", ax, x, y)
        self.ax = ax
        self.editor = editor
        self.circle = Circle((x, y), size, color=color, alpha=0.8, picker=True)
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
        self.editor.update_plots()

    def on_release(self, event):
        if self.press is None or event.button != 1:
            return
        self.press = None
        self.editor.update_plots()

    def remove(self):
        self.circle.remove()

'''
Klasa przesuwających się punktów na osi czasu.
'''
class TimeDraggablePoint(DraggablePoint):
    def on_motion(self, event):
        if self.press is None or event.inaxes != self.ax:
            return

        x0, y0 = self.press[0]
        dx = event.xdata - self.press[1]

        new_x = max(0, min(1, x0 + dx))
        self.circle.center = (new_x, y0)
        self.editor.update_plots()


pdf_path = "konkurs-I-napis-2024.pdf"
image_path = "rotated_image.jpg"

img = Image.open(image_path).convert("RGB")
width, height = img.size

min_x, min_y, max_x, max_y = width, height, 0, 0

class ImageEditor:
    def __init__(self, image_path):
        self.image_path = image_path
        self.fig, (self.ax_main, self.ax_time) = plt.subplots(2, 1, figsize=(10, 15), height_ratios=[2,1], width_ratios=[1])
        self.ax_main.imshow(plt.imread(image_path), interpolation='bilinear')
        self.ax_main.set_title("NIFS3 Edytor - Na zielono NIFS3, Na czerwono interpolacja us")
        self.ax_main.set_xlim(min_x - 15, max_x + 15)
        self.ax_main.set_ylim(max_y + 15, min_y - 15)

        self.ax_time.imshow(plt.imread(image_path), interpolation='bilinear')
        self.ax_time.set_title("Os czasu (op 0 do 1)")
        self.ax_time.set_xlim(0, 1)
        self.ax_time.set_ylim(0.4, 0.6)
        self.ax_time.set_xticks([i / 10 for i in range(11)])
        self.ax_time.set_yticks([])

        self.visibility = 1
        self.points = []
        self.time_points = []
        self.spline_lines = []
        self.time_lines = []
        self.connect()

    def connect(self):
        self.cidclick = self.fig.canvas.mpl_connect('button_press_event', self.onclick)
        self.cidkey = self.fig.canvas.mpl_connect('key_press_event', self.onkey)

    '''
        Gdy jesteśmy na głównym wykresie lub osi czasu możemy:
            -> kliknąć lewym i stworzyć kolejny punkt
            -> kliknąć lewym z SHIFT na punkt i go przesuwać
        Na osi czasu możemy także prawym usuwać punkty.
    '''
    def onclick(self, event):
        if event.inaxes == self.ax_main:
            if event.button == 1 and not event.key == 'shift':
                point = DraggablePoint(self.ax_main, self, event.xdata, event.ydata, color='green')
                self.points.append(point)

                if len(self.points) >= 3:
                    for point in self.time_points:
                        point.remove()
                    self.time_points = []
                    
                    xs, ys = zip(*[p.circle.center for p in self.points])
                    for t in [i / (len(xs) - 1) for i in range(len(xs))]:
                        point = TimeDraggablePoint(self.ax_time, self, t, 0.5, color='green', size=0.003)
                        self.time_points.append(point)
                
                    
                self.update_plots()

        elif event.inaxes == self.ax_time:
            if event.button == 1 and not event.key == 'shift':
                point = TimeDraggablePoint(self.ax_time, self, event.xdata, 0.5, color='green', size=0.003)
                self.time_points.append(point)
                self.update_plots()

            elif event.button == 3:
                for point in self.time_points:
                    contains, _ = point.circle.contains(event)
                    if contains:
                        point.remove()
                        self.time_points.remove(point)
                        self.update_plots()
                        break

    def onkey(self, event):
        if event.key == 'enter':
            self.save_data()
        elif event.key == '1':
            self.visibility ^= 1
            self.update_plots()

    '''
        Zapisywanie danych w formacie podanym w treści zadania.
    '''
    def save_data(self):
        if len(self.points) < 3:
            print("Potrzeba wiecej punktow")
            return

        xs, ys = zip(*[p.circle.center for p in self.points])
        ts = [i / (len(xs) - 1) for i in range(len(xs))]
        us = sorted([p.circle.center[0] for p in self.time_points])
        
        data_filename = "konkurs-I-345615-dane.txt"
        summary_filename = "konkurs-I-345615-podsumowanie.txt"

        with open(data_filename, "a") as data_file:
            data_file.write(f"x := {list(xs)}, ")
            data_file.write(f"y := {list(ys)}, ")
            data_file.write(f"t := {list(ts)}, ")
            data_file.write(f"u := {list(us)}\n\n")

        with open(summary_filename, "a") as summary_file:
            summary_file.write(f"1, {len(xs)}, {len(us)}\n")

        print(f"Zapisywanie danych do {data_filename} oraz {summary_filename}.")

        self.points = []
        for point in self.time_points:
            point.remove()
        self.time_points = []
        self.update_plots()

    '''
        Główna funkcja rysowania obu wykresów.
        Jest wywoływana po każdej aktualizacji.
    '''
    def update_plots(self):
        if len(self.points) < 3:
            self.fig.canvas.draw()
            return

        xs, ys = zip(*[p.circle.center for p in self.points])
        ts = [i / (len(xs) - 1) for i in range(len(xs))]
        us = [i / 1000 for i in range(1001)]

        S_x = NIFS3(ts, xs)
        S_y = NIFS3(ts, ys)

        interpolated_xs = [S_x.interpolate(u) for u in us]
        interpolated_ys = [S_y.interpolate(u) for u in us]

        for line in self.spline_lines:
            line.remove()
        self.spline_lines.clear()

        self.spline_lines.append(self.ax_main.plot(interpolated_xs, interpolated_ys, color='green', linestyle='--', linewidth=0.45)[0])
        self.fig.canvas.draw()
        
        if len(self.points) < 3:
            self.fig.canvas.draw()
            return
        
        if not self.time_points:
            for line in self.time_lines:
                line.remove()
            self.time_lines.clear()
            self.fig.canvas.draw()
            return

        us = sorted([p.circle.center[0] for p in self.time_points])

        interpolated_xs = [S_x.interpolate(u) for u in us]
        interpolated_ys = [S_y.interpolate(u) for u in us]

        for line in self.time_lines:
            line.remove()
        self.time_lines.clear()

        self.time_lines.append(self.ax_main.plot(interpolated_xs, interpolated_ys, color='red', alpha=self.visibility)[0])
        self.fig.canvas.draw()

# pixels = img.load()
# for y in range(height):
#     for x in range(width):
#         r, g, b = pixels[x, y]

#         if r > 120 and g < 160 and b < 150:
#             min_x = min(min_x, x)
#             min_y = min(min_y, y)
#             max_x = max(max_x, x)
#             max_y = max(max_y, y)

min_x, max_x, min_y, max_y = 331, 1600, 329, 800

doc = fitz.open(pdf_path)[0]
doc.get_pixmap(dpi=200).save(image_path)

with Image.open(image_path) as img:
    rotated = img.rotate(90, expand=True)
    rotated.save(image_path)

editor = ImageEditor(image_path)
plt.show()
