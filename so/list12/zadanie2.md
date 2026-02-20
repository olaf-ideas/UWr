# Zadanie 2

### Podaj definicję sekcji krytycznej

"Each process has a segment of code, called a critical section,
in which the process may be accessing — and updating — data that is shared
with at least one other process. The important feature of the system is that,
when one process is executing in its critical section, no other process is allowed
to execute in its critical section. That is, no two processes are executing in their
critical sections at the same time."


### Wymień i uzasadnij założenia rozwiązania problemu sekcji krytycznej

1. Mutual exclusion. If process P_i is executing in its critical section, then no other processes can be executing in their criticla sections.

-> z definizji sekcji krytycznej

2. Progress. If no process is executing in its critical section and some processes wish to enter thier critical sections, then only those processes that are not executing in their remainder sections can participate in deciding which will enter its critical section next, and this selection cannot be postponed indefinetely.

-> nie ma sensu, aby ktoś inny niezwiązany z sekcją krytyczną blokował dalsze obliczenia, gdy nikt nie zajmuje sekcji krytycznej to pozwól pierwszemu lepszemu procesowi na jej zablokowanie

3. Bounded waiting. There exists a bound, or limit, on the number of times that other processes are allowed to enter their critical secitons after a process has made a requiest to enter its critical section and before that request is granted.

-> zapobiegamu tutaj problemowi zagłodzenia (starvation), w którym process nie ma czasu, którym by się wykonał

### Czemu w programach przestrzeni użytkownika do jej implementacji nie możemy używać wyłączania przerwać?

Wieloprocesorowe środowiska uniemożliwiają wyłączenia przerwań. Gdy zablokujemy przerwania informacja trafiaja do wszystkich innych procesorów. Otrzymanie tej wiadomości opóźnia wejście do sekcji krytycznej, a efektywność systemu spada. 

### Odwołując się do prawa Amdahla powiedz czemu programistom powinno zależeć na tym, by sekcje krytyczne były możliwie jak najkrótsze - określa się to również mianem blokowania drobnoziarnistego

speedup = performance for the eintere task when enhancement are applied / perf for the same task without those enhancements

-> Jeśli wielowątkowość stanowi mały procent to zwiększanie liczby wątków nie pomaga

