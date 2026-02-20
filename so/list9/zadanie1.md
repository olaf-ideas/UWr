# Zadanie 1

### Wyjaśnij zadania pełnione przez protokoły warstwy: łącza, sieciowej i transportowej

Warstwa łącza: definiuje przemieszczanie datagramu za pomocą pojedynczego łącza.
Definiuje format pakietót. Warstwa ta odpowiada za detekcję błędów, retransmisje i sterowaniem
przepływu

Warstwa sieciowa: definiuje topologię sieci. Przypisuje numery IP każdemu urządzeniu. 
Używana do adresowania oraz routingu

Warstwa transportowa: definiuje podział danych na pakiety. Zapewnia pewny przekaz pomiędzy dwoma
stacjami. Wykorzystuje dwa protokoły, TCP oraz UDP. Stosuje kontrolę integralności pakietów.

### Zademontruj program wireshark




### Czemu protokoły łącza i sieciowej nie są używane do komunikacji między procesami użytkownika?

Te protokoły są potrzebne tylko do rozpoznawania urządzeń w nieprzewidywalnym świecie sieci komputerowej.
Wszystko co się dzieje na lokalnym komputerze jest monitorowane oraz kontrolowane przez kernel. Nie ma
potrzebmy adresować procesów osobnymi adresami ip lub tworzyć specjalne ramki na przesyłanie danych.
