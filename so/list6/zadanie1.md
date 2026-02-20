# Zadanie 1

Początkowa tożsamoćś: `ruid=1000, euid=0, suid=0`

### (a) setuid(2000)

Aktualny użytkownik jest administratorem (euid = 0), więc `setuid`, więc wszystko zmieni się na 2000.

`ruid=2000, euid=2000, suid=2000`

### (b) setreuid(-1, 2000)

`setreuid(uid_t ruid, uid_t euid)` zmienia real oraz effective user ID na podane lub pozostawiają bez zmian, gdy podamy `-1`

`ruid=1000, euid=2000, suid=0`

### (c) seteuid(2000)

Każdy użytkownik, może zmienić swoj effective id na real user ID lub saved set-user-ID. W przypadku root, można zmienić na dowolną wartość.

`ruid=1000, euid=2000, suid=0`

### (d) setresuid(-1, 2000, 3000)

Zwykły użykownik, może tylko przepermutować swoje ID.
Root, może wybrać dowolne wartości.

`ruid=1000, euid=2000, suid=3000`

### Czy proces z tożsamością `ruid=0, euid=1000, suid=1000` jest uprzywilewany?

Nie. Tylko `euid=0` stwierdza, czy process jest root'em. Można natomiast zmienić proces na uprzywilejowany przez `seteuid`

