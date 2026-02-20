# Zadanie 2

### Podaj scenariusze użycia prywatnych i dzielonych odwzorowań plików w pamięć albo pamięci anonimowej

Private File: Ładowanie binarki do pamięci.

Private Anonymous: malloc

Shared File: Baza danych

Shared Anonymous: Rozmawianie pomiędzy wątkami

### Podaj jak je utworzyć z użyciem wywołania `mmap(2)`

mmap(
    NULL, 
    lenth, 
    PROT_READ | PROT_WRITE | PROT_EXEC, 
    MAP_PRIVATE | MAP_SHARED | MAP_ANONYMOUS
)

### Co się dzieje z odwzorowaniami po wywołaniu `fork(2)`?

Jeśli był prywatny to jest tworzona leniwa kopia copy-on-write.

Jeśli był dzielony to dziecko oraz rodzic pracują na tej samej pamięci.

### Czy wywołanie `execve(2)` tworzy odwzorowania prytatne czy dzielone?

Przestrzeń adresów jest wyrzucana, a kernel tworzy nowe odwzorowanie.
Utworzone strony są prywatne, lecz jednocześnie są współdzielone z copy-on-write.

### W jaki sposób jądro systemu automatycznie zwiększa rozmiar stosu do ustalonego limitu?

Używa do tego `mmap`, który przydziela kolejną porcję pamięci zaraz za starym końcem stosu.

### Kiedy jądro wyśle sygnał `SIGBUS` do procesu posiadającego odwzorowanie pliku w pamięci.

Możemy otrzymać taki sygnał, gdy zczytywane dane są w źle aliasowane, próbujemy się dostać do nieistniejącego fizycznego adresu lub nastąpił błąd po stronie hardware'u.

Często otrzymujemy `SIGBUS`, gdy próbujemy się dostać do zawartości za plikiem, lecz wciąż w poprawnej stronie.