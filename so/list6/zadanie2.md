# Zadanie 2

### Jaką rolę pełnią bity uprawnień `rwx` dla katalogów w systemach uniksowych?

w -> pozwala nam usuwać/zmieniać nazwę plików wewnątrz katalogu 
r -> pozwala nam przeczytać katalog, otrzymując listę nazw plików wewnątrz
x -> pozwala nam wejść do katalogu, gdy podajemy ścieżkę

```
olaf@laptop:~$ mkdir test
olaf@laptop:~$ cd test
olaf@laptop:~/test$ ls -lAh
total 0

olaf@laptop:~/test$ cd ..

olaf@laptop:~$ chmod -x test
olaf@laptop:~$ cd test
bash: cd: test: Permission denied

olaf@laptop:~$ chmod +x test

olaf@laptop:~$ chmod -r test
olaf@laptop:~$ cd test
olaf@laptop:~/test$ ls
ls: cannot open directory '.': Permission denied

olaf@laptop:~/test$ cd ..
olaf@laptop:~$ chmod +r test

olaf@laptop:~$ chmod -w test
olaf@laptop:~$ cd test
olaf@laptop:~/test$ touch abc.txt
touch: cannot touch 'abc.txt': Permission denied
```

### Opisz znaczenie bitów `set-gid` i `sticky`

Gdy bit `set-gid` jest ustawiony w folderze, to każdy w nim stworzony folder dziedziczy wymagane tożsamości od rodzica.
Stworzony foldery dziedziczą `set-gid` od rodzica.

Przydatne gdy mamy folder, który jest udostępniany innym użytkownikom.

`sticky` bit dla katalogu oznacza, że tylko właściciel pliku, właściciel folderu lub root mogą usuwać i zmieniać nazwy pliku wewnątrz katalogu.

### Napisz w pseudokodzie i zreferuj procedurę `bool my_access(struct stat *sb, int mode)`

```
bool my_access(struct stat *sb, int mode) {
    if (mode == F_OK) {
        return sb != NULL;
    }
    
    uid_t uid = getuid();
    if (uid == 0) {
        if (mode & X_OK) {
            return (sb->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0;
        }
        return true;
    }

    if (uid == sb->st_uid) {
        return ((sb->st_mode >> 6) & mode) == mode;
    }

    gid_t gids[65536];
    int count = getgroups(65536, gids);

    int perm = (sb->st_mode & 0b111); // perm for others

    for (int i = 0; i < count; i++) {
        if (gids[i] == sb->st_gid) {
            perm = (sb->st_mode >> 3) & 0b111;
            break;
        }
    }

    if ((mode & R_OK) && (perm & 0b100) == 0)
        return false;
    
    if ((mode & W_OK) && (perm & 0b010) == 0)
        return false;
    
    if ((mode & X_OK) && (perm & 0b001) == 0)
        return false;

    return true;
}
```