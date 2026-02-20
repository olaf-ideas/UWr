# Zadanie 6

### Co robi system plików `ext2` przy tworzeniu dowiązania twardego i symbolicznego?
Dowiązania twarde to wskaźniki na inode danego pliku, działa jak referencja.
Usuwanie oryginalnego pliku nie psuje dowiązania. Dane istnieja dopuki przynajmniej
jedno twarde dowiazanie istnieje. Musi linkować rzeczy w tym samym systemie plików.


Dowiązania symboliczne to nic innego jak text opisujący ścieżkę do oryginalnego pliku.
Dla symlinku krotszego od 60 bajtow dane sa zapisywane w samym inode, uzywa sie
pol ktore normalnie bylyby uzywane do zapisywania pointerow.

### Gdzie jest przechowywana zawartość dowiązania symbolicznego?

Jest to osobny plik tekstowy opisujacy sciezke do pliku

### Jak stworzyć pętlę?

`ln -s hello ./hello`

### Kiedy jądro systemu operacyjnego wykryje pętlę i zwróci błąd `ELOOP`

Błąd pojawia się po przekroczeniu limitu głębokości lub po wykryciu, że odwołujemy się
do tego samego symlinku.

### Czemu nie da sie zrobić pętli z użyciem dowiązania twardego?

Dowiązanie twarde potrzebuje wskaźnik na istniejący inode.
A on nie istnieje, bo jeszcze nie utworzyliśmy hardlinku.