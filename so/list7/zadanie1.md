# Zadanie 1

### Wyjaśnij różnicę między odwzorowaniami plików w pamięć i odwzorowaniami pamięci anoniminowej.

Patrz na tablicę. (Plikowe są z pliku, anonimowe są z czapy)

### Jaką zawartością wypałniana jest pamięc wirtualna należąca do tychże odwzorowań?

Dla odwzorowań plików są to dane wewnątrz pliku. A dla odwzorowań anonimowych są to zera.

### Czym różni się odwzorowanie prywatne od dzielonego?

Odwzorowanie prywatne isoluję pamięc od innych procesów. W przeciwieństwie do dzielonego gdzie inne procesy mogą do niej dostęp i mogą ją modyfikować.

### Czy pamięc obiektóœ odwzorowanych prywatnie może być współdzielona?

Tak, kernel optymalizuje strony, które używane są przez te same procesy. Używa mechanizmu Copy-On-Write, który kopiuje zawartość strony, gdy potrzebna jest modyfikacja.

### Czemu można tworzyć odwzorowania plików urządzeń blokowych w pamięć, a znakowych nie?

Dane są efemeryczne, raz odczytane.
Jest to strumień, a nie nośnik danych.
Urządzenia blokowe posiadają wspólny interfejs działąnia. Brak definicji przecunięcia.