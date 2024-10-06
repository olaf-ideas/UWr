import numpy as np
import matplotlib.pyplot as plt

# Funkcja do obliczenia wskaźnika W
def oblicz_w(Pc, Pe):
    if Pc >= 112:
        beta = 1.2
    elif Pc >= 100:
        beta = 1.15
    elif Pc >= 88:
        beta = 1.1
    elif Pc >= 76:
        beta = 1.05
    else:
        beta = 1.0

    W = 0.25 * beta * (Pc / 64) + 0.75 * (Pe / 20)
    return W

# Funkcja do określenia oceny na podstawie W
def okresl_ocene(W):
    if W >= 1.8:
        return 'bdb'
    elif W >= 1.6:
        return 'db+'
    elif W >= 1.4:
        return 'db'
    elif W >= 1.2:
        return 'dst+'
    elif W >= 1.0:
        return 'dst'
    else:
        return 'ndst'

# Funkcja do mapowania ocen na wartości numeryczne
def ocena_num(ocena):
    mapping = {'bdb': 5, 'db+': 4.5, 'db': 4, 'dst+': 3.5, 'dst': 3, 'ndst': 2}
    return mapping.get(ocena, 0)

# Zakres punktów z ćwiczeń i egzaminu
Pc_values = np.arange(0, 121, 1)
Pe_values = np.arange(0, 51, 1)

# Tablica ocen
oceny = np.zeros((len(Pc_values), len(Pe_values)), dtype=object)

# Obliczanie ocen dla każdej kombinacji Pc i Pe
for i, Pc in enumerate(Pc_values):
    for j, Pe in enumerate(Pe_values):
        W = oblicz_w(Pc, Pe)
        oceny[i, j] = okresl_ocene(W)

# Konwersja tablicy ocen na wartości numeryczne
oceny_num = np.vectorize(ocena_num)(oceny)

# Przygotowanie wykresu
Pc_grid, Pe_grid = np.meshgrid(Pc_values, Pe_values)
fig, ax = plt.subplots()

# Tworzenie wykresu z dokładnym podziałem na poziomy ocen
levels = [2, 3, 3.5, 4, 4.5, 5]  # Poziomy odpowiadające wartościom ocen
contour = ax.contourf(Pc_grid, Pe_grid, oceny_num.T, levels=levels, cmap='viridis', alpha=0.75)

# Dodanie etykiet i tytułów
ax.set_title("Ocena w zależności od punktów z ćwiczeń (Pc) i egzaminu (Pe)")
ax.set_xlabel("Liczba punktów z ćwiczeń (Pc)")
ax.set_ylabel("Liczba punktów z egzaminu (Pe)")

# Dodanie legendy z poziomami ocen
legend_labels = ['ndst', 'dst', 'dst+', 'db', 'db+', 'bdb']
handles = [plt.Rectangle((0,0),1,1, color=contour.cmap(contour.norm(i))) for i in levels]
ax.legend(handles, legend_labels, title="Oceny")

# Wyświetlenie wykresu
plt.show()

