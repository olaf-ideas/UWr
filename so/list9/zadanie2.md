# Zadanie 2

### Omów różnice `TCP` a `UDP`

(Gwarancja dostarczenia danych, Zachowanie kolejnosci pakietow,
Wykrywanie bledow i wysyla dane ponownie, kontrola przeciazenia i przeplywu,
duze opoznienie)

(Nie gwarantuje dostarcenia danych, nie pilunje kolejnosci pakietow,
Brak retransmisji przy bledach, bardzo male opznienia, prosty)

TCP to protokół, w którym najpierw jest ustanawiane połączenie,
a następnie trwa dialog pomiędzy dwoma procesami. Za każdym
wysłym pakietem wysyłany jest komunikat ACK. Dzieki temu mamy
gwarancję, żę przesyłąne pakiety dotarły na drugą stronę.

UDP tylko nasłuchuje nadchodzące pakiety i nie oczekuje na potwierdzenie.
TCP jest wolniejesze od UDP przez swój mechanizm potwierdzania
otrzymania każdego komunikatu.

### Czym różni się komunikacja półdupleksowa od dupleksowej?

Komunikacja półdupleksowa umożliwa na rozmowę w obie strony,
ale tylko w jedną stronę w jednym momencie. Peły dupleks
umożliwia dwustroną komunikację w każdym momencie.

TCP -> duplexowy
UDP -> poldouplexowy, nie zarzadza komunikacja dwukierunkowa, brak kontroli i synchronizacji

### Jak `TCP` radi sobie z zagubieniem segmentu lub faktem, że segmenty mogą przyjść do odbiorcy w innej kolejności niż zostały wysłane

TCP -> sequence numbers
Potwierdzania ACK
Retransmicja (Potrojne ACK, TImeout RTO)
Bufotowanie segmentow

TCP indeksuje wysyłane pakiety. W ten sposób wie jaka jest dokładnie kolejność pakietów.
Jeśli podrodze zgubiono pewiem pakiet to bufferowana jest pewna cześć pakietów z późniejszego czasu
oraz oczekiwany jest pakiet z kolejnym numerkiem.

### Control flow w `TCP`

TCP control flow chroni szybkie procesy spamujące pakietami od wolnym odbiorców
używając mechanizmu przesuwnego okienka. To okno mówi ile bajtów można wysłać zanim
otrzyma się ACK.

