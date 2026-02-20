# Zadanie 3

### Jaką tożsamość będzie miał na początku proces wykonuący `su`, jeśli przed `execve` było `euid=1000`?

```
olaf@laptop:~$ ls -lAh /usr/bin/su
-rwsr-xr-x 1 root root 71K Nov 21  2024 /usr/bin/su

olaf@laptop:~$ su root
Password: 
root@laptop:/home/olaf# id
uid=0(root) gid=0(root) groups=0(root)
```

Przez to, że mamy ustawiony bit `set-uid` to po uruchomieniu pliku `su` nasza cała toższamości zamieni się właściciela pliku `su`, czyli na 0 (root).

### Zreferuj działanie uproszczonej wersji programu `su`

Pobieramy uid programu. Pytamy o hasło jeśli użytkownik nie jest root'em.
Ustawiamy uid i gid należący do użytkownika w argumencie.
Ustawiamy odpowiednie zmienne środowiskowe.
Finalnie odpalamy instację nowej powłoki.