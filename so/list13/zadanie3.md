# Zadanie 3

### Opisz semantykę operacji `FUTEX_WAIT` i `FUTEX_WAKE` mechanizmu `futex(2)`

futex - fast user space mutex

futex
- avoids dropping into the kernel unless it really has to

futex {
    kernel service,
    user library
}

kernel service -> provides wait queue

FUTEX_WAIT -> śpij jeśli wartość jest taka jakej oczekiwaliśmy

FUTEX_WAKE -> obudź n wątków czekających


futex(wskaźnik na słowo 4 bajtowe, op, /* argumenty */)


### Czym różnią się blokady adaptacyjne od zwykłych blokad usypiających?

Blokady adaptywne po otrzymaniu komunikatu o braku dostępu
do zasobu, przez pewien czas robią spin-lock w razie szybkiej zmiany w innym wątku. Po dłuższym czasie oddają CPU do kernela.

Mamy mniej context-switch'y, szybsza reakcja w któtkich sekcjach krytycznych.

### Zreferuj implementację prostej blokady z operacjami `__lock` i `__unlock`. 

Opis wartości w futex'ie jest w kodzie

Jeśli futex nie ma nikogo, kto na niego czeka
to jest blokowany. 

Potem jest pętla 10 iteracji, która robi spin-lock
czekając na szybką zmianę w konkurencji, gdy
nadal nie udało się uzyskać blokady to 
odpalamy __futexwait()


unlock tylko patrzy jak futex jest zablokowany
to zaktualizuj wartosc na dodatnia oraz obudz jakis inny
watek jesli istnieje.

### Jak zachowuje się blokada w warunkach wysokiego wspólzawodnictwa?

Gdy współzawodnictwo jest wysokie to czas pomiędy zmianami blokady wątków jest bardzo szybki przez wprowadznienie spinlock'a, gdy jest on długi to opanuje normalny tryb pracy. Spin-lock zmniejsza ilość wejść do kernel-space (context-switch).

### W jakich warutnkach usypiamy i wybudzamy wątki?

Tylko gdy po 10 iteracjach nadal futex będzie zablokowany to wątek zasypia. 

Wybudzamy tylko gdy istnieje jeszcze inny watek ktory czeka na wybudzenie
