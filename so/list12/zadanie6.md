# Zadanie 6

```
while (turn != id) {
    while (blocked[1 - id])
        continue;
    turn = id;
}
```

Wątki mogą na przemian zabierać sobię turę
przez co otrzymamy live-lock.