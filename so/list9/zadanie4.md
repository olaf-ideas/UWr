# Zadanie 4

### Czemu, w przeciwieństwie do `TCP`, serwer może rozpocząć pracę zaraz po wykonaniu funkcji `bind(2)`?

W UDP nie potrzeba, żadnych `ACK`, przez co nie musimy tworzyć połącznia i o nie dbać, tylko 
po prostu wysyłamy pakiet przez internet, który dociera lub nie do drugiej strony. Żaden listen
nie jest potrzeby.

### Z jakiej przyczyny interfejs `read(2)` i `write(2)` po stronie serwera może być niewystarczający?

Funkcje takie jak `read(2)` nie zwracają skąd przyszedł pakiet. `read(2)` traktuje pakiet jak ciąg
bajtów, `recvfrom()` czeka, aż cały pakiet przyjdzie w całości.

Dla read/write potrzeba osobną mechanikę zestawiania połączenia.

### Semantyka `recvfrom`

Odbiera datagramy z gniazda i jednocześnie przechwytuje adresy nadawcy.
Blokuje doputy nie otrzymany jest kolejny pakiet (chyba, że `fctl` mówi inaczej).
Zwraca liczbę odebranych bajtów, lub -1 dla błędu. W TCP oznacza, że połączenie zostało zamknięte.

### Semantyka `sendto`

Wysyła datagramy przez gniazdo do adresu docelowego.
Wysłanie datagramu jest operacją atomową. Jest blokujący.
Brak gwarancji dostarczenia. Wzraca liczbę wysłanych bajtów lub -1 dla błędu.

### Kiedy po stronie klienta następnue związanie gniazda `UDP` z adresem lokalnym?

bind()

### Zreferuj efekt jaki przynosi wykonanie `connect(2)` na gnieździe klienta.

Nie ma żadnego three-way handshake. Kernel sprawdza czy jakies błędy (unreachable destination, i inne),
zapisuje port oraz adress ip i odrazu wraca do oryginalnego procesu. Po connect nasze gniazdo 
otrzymuje status connected UDP socket.

Co za tym idzie możemy używać read oraz write do przesyłania wiadomości
Wysyłane wiadomości tylko z i do połączonego gniazda.

Używane tylko w momentach połączenia z dokładnie jednym innym serwerem.
Jest to szybsze tylko w tym przypadku, tak to trzeba konczyc połączenia 
przed zmiana polaczenia.

### Jak `recvmsg` i `sendmsg` naprawiło potrzedni problem?

Funkcje `recv/recvfrom` wymajaa ciągłości bufora (cały odbierany komunikat
mieścił się w jednym ciągłym bloku pamięci). Podawana struktura `msghdr` która zawiera tablice
struktur iovec, która wskazuje na bloki pamięci.

`sendmsg` rozwiązuje ten sam problem. Gdy chcemy przesłać parę tablic pamięci wystarczy, że
podamy je w sturkturze i tylr. 