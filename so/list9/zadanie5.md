# Zadanie 5

### Kiedy `read(2)` i `write(2)` na gniazdach strumieniowych zwracają `short count`?

`read(2)` zwraca mniej bajtów, bo:
- TCP jest strumieniem i zostanie zwrócone to co jest w buforze
- Drugi koniec zamknął połączenie
- Gniazdo jest nieblokujące

`write(2)` wzraca mnie bajtów, bo:
- w buforze jest za mało mniejsca
- w trybie nieblokującym coś mogło półść nie tak podczas wysyłania

### Skąd wiemy, że odebrany datagram `UDP` nie został obcięty przez jądro w trakcie kopiowania do przestrzeni użytkownika?

Jeśli podany bufor jest za krótki to datagram zostanie ucięty.
1) W `recvmsg` mamy MSG_TRUN w msg_flags po powrocie
2) Używamy bufora o rozmiarze `MTU` i nie mamy takiego problemu

### Z jakich przyczyn należy być przygotowanym na to, że operacje na gniazdach zwrócą `EINTR`?

`EINTR` = wywołanie zostało przerwana przez sygnał przed zakończeniem

Typowe sytuacje:
> SIGCHLD, SIGALRM, SIGINT
> Wątek był zablokowany przez accept, read write connect itp

Nalęży robić pętlą dopóki wystapuje błąd EINTR

### Co się stanie, jeśli klient spróbuje zapisać do gniazda powiązanego z połączeniem, które serwer zdążył już zamknąć?

Otrzymamy `SIGPIPE`

### Dlaczego w kodzie funkcie `open_listenfd` użyto wywołania `setsockopt(2)` z opcją `SO_REUSEADDR`? Co by się stało gdyby programista o tym zapomniał?

`SO_REUSEADDR` pozwala ponownie zbindować serwer do tego samego adresu/portu, nawet jeśli w systemie istnieją jescze żyjące połączenia.

Bez flagi oraz zbyt szybkim resecie otrzymamy `EADDRINUSE`, a z flagą trzeba czasami poczekać na zwolnienie się adresu.
