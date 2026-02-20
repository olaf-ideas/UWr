# Zadanie 5

### Podaj cztery warunki konieczne do zaistnienia zakleszczenia

1. Mutual exclusion
Wątki mają wszystkie dane, które potrzebują na wyłączność
2. Hold-and-wait
Wątki czekają na zablokowanie kolejnych danych, równolegle trzymając już wcześniej
3. No preemption
Blokady nie mogą być usunięte z wątków, które je trzymają
4. Circular wait
Występuje cykliczne oczekiwanie na zwolnienie blokady przez wątki

### Wyjaśnij w jaki sposób można przeciwdziałać zakleszczeniom?

Najprostszy sposób to usunięcie cyklicznych wymagań - stworzenie total-order na wszystkich blokadach.

Aby uniknąć hold-and-wait można dodać jedną dużą blokadę, która trzyma inne mutex'y.

Aby uniknąć no-preemption można używać `pthread_mutex_trylock()`, który zwraca wartość fałsz jeśli nie udało się uzyskać blokady, co umożliwia usunięcie blokady bez czekanie na resztę.

Można pisać kod, który nie używa mutex'ów i jest lock-free oraz wait-free używająć instrukcji hardwarowych.

Można unikać dead-lock poprzez odpowiednie schedule wątków, aby wogóle nie było potrzeby używać blokad.

### Czym są klasy blokad + Opis lockdep
Każda blokada, która działą w podobny sposób jest w tej samej klasie blokad (klasa równoważności), np. każda blokada do vnode jest w jednej klasie.

Każda klasa L to dwie listy (wszystkie blokady, które były zablokowane kiedy blokuje sie blokade klasy L). Jest to lista klas blokad, które wystąpiły przed L. Można to zwizualizować jako krawędzie przychodzących wierzchołków w grafie. Druga lista jest odpowiednikeim klas blokad, które zostały zablokowane po zablokowaniu blokady z L. Są to krawędzie wychodzące.

Takie sprawdzanie brutalnie jest O(n^2), więc używa się hashowania. Gdy następuje nowe blokowanie liczony jest hash zbioru juz zablokowanych blokad. Gdy sprawdzimy, że dany zbiór jest dobry to dodajemy to do hashmapy dobrych i liczymy dalej. Tylko sprawdzamy gdy napotkamy na nową sytuację.

Gdy lockdep nie ma false negative - gdy powie, że nie ma deadlocków to mamy tego pewność.

### Z czym sobie nie radzi?

Gdy kod tworzy niemożliwe zakleszczenia, które są do dowodzenia przez swoją wewnętrzną logikę.
Przykład innego blokowania drzewa, kiedy chcemy korzeń lub kiedy chcemy inny wierzcholek.

