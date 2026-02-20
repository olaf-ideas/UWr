# Zadanie 1

### Czym różni się przetwarzanie równoległe od przetwarzania współbieżnego?

W przetwarzaniu współbieżnym jednostki pracy mogą się przeplatać w czasie.
Nie jest wymagana jednoczesna praca.

W przetwarzaniu równoległym conajmniej dwie jednostki pracy są wykonywane
jednocześnie.

### Czym charakteryzują się procedury wielobieżne?

Procedura jest wielobieżna, jeżeli może zostać wywołana ponownie zanim
zakończy się poprzednie wywołanie. Zazwyczaj nie używa współdzielonego,
mutowalnego stanu. Nie zwraca wskaźnika do współdzielonego bufora oraz
nie polega na niedeterministycznych 