# Zadanie 2

### Jak policzyć:

Rozmiar bloku: `block_size = 1024 << s_log_block_size;`

Liczbę i-węzłów: `s_inodes_count`

Bloków przechowywanych w grupie bloków: `s_blocks_per_group`

Liczby wpisów tablicy deskryptorów grup bloków: Tutaj wejść do tablicy i wyjąć wartości

### Wymień składowe należącedo grupy bloków oraz podaj ich rozmiar w blokach

superblock - 1
block group descriptor table - 1
block bitmap - 1
inode bitmap - 1
inode table - 214
data blocks - 7974

### Gdzie są kopie zapasowe superbloku?

Od revision 1 kopie zapasowe są w 0, 1 oraz potęgach 3, 5 i 7.