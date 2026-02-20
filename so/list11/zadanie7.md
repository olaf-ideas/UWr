# Zadanie 7

### Czemu fragmentacja systemu plików jest szkodliwym zjawiskiem?

High fragmentation rates cause greater disk access time affecting
overall throughput, and increased metadata overhead causing less 
efficient mapping.

### Zreferuj pracę:

Główny problem: w ext3 można przetrzymywać max 16TB.
Teraz można 2^(48+12) = 1EB z 4KB block size

Czemu nie uzywać 64bitow zamiast 48? 1EB na te czasy trwałby 119 lat
dla e2fsck oraz 65536 razy wiecej dla 2^64 bloków.


Dodano META_BG (metablock group), które grupują block grupy.
(dla 4KB bloków, metablock group trzyma 64 group block or 8GB of disk space)
Backupy sa w ostatniej i przedostatniej grupie metablock grupy.


Zmieniono unit rozmiaru pliku z 512MB na wielosc blocku, ale tylko
dla plikow z flaga EXT_HUGE_FILE_FL.

Usunięto limit 32000 folderów w folderze, teraz sky is the limit.

Zmieniono glebokosc HTree z 2 na 3.

### Opisz w jaki sposó odroczony przydział bloków zapobiega fragmentacji?

Wszystkie operacje są odroczone ąż do page flush time, a nie podczas write().

Ten trick pozwala na połączenie wielu alokacji blokow w jedno zapytanie,
zapobiegając fragmentacji oraz cykle CPU. To także usprawnia działania
plików o któtkim żywocie. (30% improved throughput and 50% reduction in CPU).

Dodano także parę usprawnień, używa się SID do ukłądania plików obok siebie.

W ext4 jest także online defragmentacja. (e4defrag). Kopiuje dane do tymczasowego
inode, potem alokuje przedzial a potem znowu odklada pliki na miejsce.

### Wytłumacz jak zakresy (ang. extents) pomagają w ograniczaniu rozmiaru metadanych prezechowujących adressy bloków należących do danego pliku.

Extent to jeden deskryptor który reprezentuje przedział ciągłej fizycznych bloków.
Zamiast trzymać listę każdego bloku reprezentującego plik, tworzy sie wskaznik
na pierwszy z nich oraz ostatni. The przedziały są właśnie extent.

Bardzo pofragmentowany plik potrzebowałby wiele extentow (w ext4 to jest extent nodes).

Nie potrzeba uzywac az tylu indirect pointerow.

### Czy po defragmentacji systemu plikow ext4 liczba wolnych blockow moze wzrosnac?

Tak jesli bedzie uzywana kompresja lub zostana uzyte extenty zamiast indirect list.

### Jak wygląda najproszty tool do defragmentacji.

Dla każdego pliku zrob tymczasowy inode dla plik z ciągłym extentem na cały plik.
Następnie kopiuj oryginalny plik do page cache oraz flush na dirty. Finalnie
podmień pointer starego inode aby wskazywal na nowa pamiec.