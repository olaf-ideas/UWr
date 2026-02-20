# Zadanie 1

### Na czym polega problem odwórcenia priorytetów?

Przykład tego problemu:

3 procesy o malejacym priorytecie: 1, 2, 3
 

> 3 blokuje zasoby
> 1 probuje dostac się do zasobów, ale są zablokowane przez 3
> 2 rozpoczyna się i wygrywa priorytetem z 3
> 2 kończy się
> 3 kończy się i odblokowuje zasoby
> 1 blokuje zasowy
> 1 kończy się i odblokowuje zasoby

Procesy skończą się w kolejności, 3, 2, 1, choć 1 miał wyższy priorytet oraz
najpierw mogliśmy skończyć proces 3, rozpoczac 1 a na konczu zaczac 2.

``` man futex
Priority-inheritance futexes
       Linux  supports  priority-inheritance (PI) futexes in order to handle priority-in‐
       version problems that can be encountered with normal futex locks.  Priority inver‐
       sion is the problem that occurs when a high-priority task is  blocked  waiting  to
       acquire  a lock held by a low-priority task, while tasks at an intermediate prior‐
       ity continuously preempt the low-priority task from the  CPU.   Consequently,  the
       low-priority task makes no progress toward releasing the lock, and the high-prior‐
       ity task remains blocked.
```

### Na czym polega dziedziczenie priorytetów?

Gdy inny wątek o wyższym prioryterze próbuje dostać się do zasobów
podwyższane są uprawnienia wątku, który tą blokadę osiągnął.

W ten sposób priotytet zwolnienia blokady jest zwiększany i zwalczamy
problem odwrócenia priorytetów.

### W jakim celu `mutex` pamięta właściciela, tj. wątek który trzyma blokadę?

Jest mu potrzebny właśnie podczas metody dziedziczenia priorytetów.
Proces, który próbuje dostać się do zablokowanych zasobów wysyła komunikat
o zwiększeniu priorytetu innemu procesowi, id tego procesu znajdziemy w mutex'ie.

### W jaki sposób należy rozszerzyć implementację operacji `mutex_lock` i `mutex_unlock`,
żeby nie dopuścić do odwrócenia priorytetów?

void mutex_lock() {
    if zablokowany:
        mutex -> wez id z mutexa
        i zwiększ priotytet włąściciela do mojego priorytetu
}

### Czy semafory są odpowrne na problem odwrócenia priorytetów?

Nie, ten sam problem następuje, bo binary semafor = mutex.