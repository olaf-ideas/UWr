# Zadanie 5

### Jakie zadania pełni `exit(3)`?

Wywołuje funkcje zarejestrowane przez `atexit()`.
Flushuje, uzamyka deskryptory, usuwa pliki `tmpfile()`.

```
All open stdio(3) streams are flushed and closed.  Files created by tmpfile(3) are removed.
```

`exit` pomaga w wyczyszczeniu buforów oraz zakmnięciu strumieni.

Po wykowaniu `fork` cała pamięć wirtualna procesu jest kopiowana,
do tego także wliczają się bufory `stdio`. 

### Jak zapobiec tym problemom?
Wystarczy wykonać `flush()` przed.

### Jaka jest domyślna strategia buforowania strumienia związanego z

#### (a) plikiem terminala

Line buffered

#### (b) Plikiem zwykłym

Fully buffered

#### (c) Standardowym wyjściem blędów `stderr`

No buffering

### Piszesz program, który używa biblioteki `stdio`. Działanie programu da się przerwać sygnałem `SIGINT`. ...

Rozwiązania:
1) Nie używamy `stdio`
2) Usuwamy buferowanie przez setbuf()
3) Globalna atomowa flaga, która jest sprawdzana podobnie jak errno

