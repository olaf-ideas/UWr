# Zadanie 4

### Kontrprzykład 1

1) acquire -> active = 1 waiting = 0 must_wait = False
2) acquire -> active = 2 waiting = 0 must_wait = False
3) acquire -> active = 3 waiting = 0 must_wait = True
4) acquire -> active = 3 waiting = 1 must_wait = True
5) acquire -> active = 3 waiting = 2 must_wait = True
6) acquire -> active = 3 waiting = 3 must_wait = True

1) release -> active = 2 waiting = 3 must_wait = True
2) release -> active = 1 waiting = 3 must_wait = True
3) release -> active = 0
    block.post()
    block.post()
    block.post()

    Zanim 4), 5), 6) wezmą mutex.wait()
    to przyjdzie 7) acquire -> acitive += 1
    a następnie 
    5) acquire -> active = 2, waiting = 2 must_wait = False
    4) acquire -> active = 3, waiting = 1 must_wait = True
    6) acquire -> active = 4, waiting = 0 must_wait = False

> Brak zachowania max 3 procesów

### Kontrprzykład 2

Semafor blcok nie gwarantuje FIFO, więc block.post()
nie musi odblokować wątków w tej samej kolejności
(choćby dlatego, że pierwszy na wejsćiu jest 1) to
mógł być context-switch na 2) i 2) był pierwszy w block.wait())

