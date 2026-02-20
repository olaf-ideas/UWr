# Lista 2

## Zadanie 1

## Zadanie 2

Linux: fork + exec

1. fork(): tworzy w jądrze nową strukturę `task struct`, dzieli tablicę deskryptorów prez referencję, ustawia pamięc jako copy-on-write, kopiuje maski sygnałów i handler'y. Rodzic dostaje PID dziecka, dziecko dostaje 0.
2. execve(): jądro podmienia obraz procesu; ładuje ELF, tworzy nową przestrzeń adresową, mapuje segmenty, buduje stoz z argv/environ, resetuje handlery sygnałów do domyślnych, zamyka deskryptory.

Windows:

1. CreateProcess zamiast fork, tworzy nowy proces z jednym wątkiem startowym

fork(): alokacja nowego `task_struct`; utworzenie referencji do stron i deskryptorów; wpis w drzewie procesów
execve(): zamkniecie starych deskryptorow, wczytanie loader'a, mapowanie segmentów, inicjacja stosu (argc,argv,envp,auxv), reset handlerow, maska sygnalow zostaje

Dlaczego spawn nie dziala?
> Przekierowanie strumienia nalezy zrobic po utworzeniu dziecka, lecz przed uruchomieniem execve. Dziecko nie wie czy chce `stdin` z terminala czy z pliku. Brak okienka czasowego w dziecku na manipulację fd/sygnalami.

## Zadanie 3

a)
> The entire virtual address space of the parent is replicated in the child, including the states of mutexes, condition variables, and other pthreads objects;
(To jest copy-on-write (COW))

> The child inherits copies of the parent's set of open file descriptors. Each file descriptor in the child refers to the same open file descriptor (see open(2)) as the corresponding file descriptor in the parent. This means that the two file descriptors share open file status flags, file offset, and signal-driven I/O attributes

> The child inherits copies of the parent's set of open message queue descriptors.

> The child inherits copies of the parent's set of open directory streams (see opendir(3))

b)

Zostaje PID/PPID,pgid,sid, maska sygnałów, otwarte FD (bez CLOEXEC), cwd, umask, limity, pending signals zostają

Czemu prezd wywołaniem fork należy opróżnić bufory biblioteki stdio(3)?
> Dziecko jest kopią swojego rodzica i otrzymuje kopię bufora, przez to
wypisane na wyjściu zostaną dwie kopie bufora.

Co jądro robi w trakcie wywołania `execve` z konfiguracją zainstalowych procedur obsługi sygnałów?
> The dispositions of any signals that are being caught are reset to the default

## Zadanie 4

```
olaf@laptop:~/uwr/UWr/so/list2$ pgrep xeyes
6182
olaf@laptop:~/uwr/UWr/so/list2$ kill -s SIGUSR1 6182
olaf@laptop:~/uwr/UWr/so/list2$ kill -s SIGUSR2 6182
olaf@laptop:~/uwr/UWr/so/list2$ kill -s SIGHUP 6182
olaf@laptop:~/uwr/UWr/so/list2$ cat /proc/6182/status
Name:   xeyes
Umask:  0022
State:  T (stopped)
Tgid:   6182
Ngid:   0
Pid:    6182
PPid:   4785
TracerPid:      0
Uid:    1000    1000    1000    1000
Gid:    1000    1000    1000    1000
FDSize: 256
Groups: 20 24 25 27 29 30 44 46 100 106 112 114 117 1000 
NStgid: 6182
NSpid:  6182
NSpgid: 6182
NSsid:  4785
VmPeak:    11756 kB
VmSize:    11756 kB
VmLck:         0 kB
VmPin:         0 kB
VmHWM:      5000 kB
VmRSS:      5000 kB
RssAnon:             548 kB
RssFile:            4452 kB
RssShmem:              0 kB
VmData:      932 kB
VmStk:       132 kB
VmExe:        12 kB
VmLib:      3308 kB
VmPTE:        64 kB
VmSwap:        0 kB
HugetlbPages:          0 kB
CoreDumping:    0
THP_enabled:    1
Threads:        1
SigQ:   5/30848
SigPnd: 0000000000000000
ShdPnd: 0000000000000a03
SigBlk: 0000000000000000
SigIgn: 0000000000000000
SigCgt: 0000000000000000
CapInh: 0000000000000000
CapPrm: 0000000000000000
CapEff: 0000000000000000
CapBnd: 000001ffffffffff
CapAmb: 0000000000000000
NoNewPrivs:     0
Seccomp:        0
Seccomp_filters:        0
Speculation_Store_Bypass:       thread vulnerable
SpeculationIndirectBranch:      conditional enabled
Cpus_allowed:   ff
Cpus_allowed_list:      0-7
Mems_allowed:   00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000001
Mems_allowed_list:      0
voluntary_ctxt_switches:        146
nonvoluntary_ctxt_switches:     2

```

`SigPnd` - sygnały oczekujące dla wątku
`ShdPnd` - sygnały dla grupy wątków
`SigBlk` - sygnały zablokowane
`SigIgn` - sygnały ignorowane
`SigCgt` - sygnały handlowane

```
olaf@laptop:~/uwr/UWr/so/list2$ fg
xeyes
Hangup
```

### Zadanie 5

sinit.c obsługuje reboot oraz wyłączanie systemu, odpowiada ze grzebanie dzeci oraz co jakis czas czeka na wygnal i go odpowiednio obsluguje.

SIGUSR1 -> wylacza system
SIGCHLD,SIGALRM -> grzebie wszystkie dzieci oraz ustawia kolejne grzebanie za 30 sekund
SIGINT -> restartuje system

`sigprocmask` -> ustawia maske obslugiwanych sygnalow (blokowanie/odblokowanie)
`sigwait` -> czekanie na sygnal

W `waitpid` jest ustawiona flaga `WNOHANG` wiec grzebie dzieci bez czekania
a nastepnie ustawia alarm za 30 sekund na kolejne grzebanie.

### Zadanie 6

