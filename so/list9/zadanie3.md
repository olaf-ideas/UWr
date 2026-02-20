# Zadanie 3

### W którym momencie następuje związanie gnazda z adresem lokalnym i zdalnym?

> connect <-> accept

### Która ze stron używa portów ulotnych (ephemeral)?

Klient używa portu ulotnego. Dla niego nie jest ważne z jakiego portu wysyła,
ważne do jakiego portu wysyła i jakie proces po drugiej stronie ma to uzyskać.

### Co specyfikuje drugi argument wywołania systemowego `listen(2)`?

Jest to backlog, czyli maksymalna długość kolejki przychodzących połączeń.
Gdy kolejka jest pełna proces pytający się o połączenie otrzyma `ECONNREFUSED`.

### Z jakim numerem portu jest związane gniazdo przekazywane do i zwracane z `accept(2)`.

Jest ono zdefiniowane w `struct sockaddr`.

### Skąd serwer wie, że klient zakończył połączenie?

Iniciator wysyła komunikat `FIN`, następnie odbiorca wysyła `ACK` `FIN`,
a na końcu iniciator wysyła `ACK`. Serwer oraz klient także uznają połączenie
za zakończone jeśli po dłużym czasie, żaden pakiet nie wraca z odpowiedzią.