# Zadanie 7

Blocked oznacza, kto zablokował

Niezmiennik:
W sekcji krytycznej zawsze mamy
turn = id lub blocked[1 - id] == false


blocked[1 - id] = false -> oznacza, że drugi watek
nie zaczal jeszcze blokowania, mozna spokojnie robic co sie chce

blocked[1 - id] = true -> drugi watek jest w fazie blokowanie.

    turn == id

    ja jestem zablokowany, oraz wiem ze drugi watek nie jest w sekcji krytyczej po wlasnie zmienil turn = 1 - id czyli na moj id, nie wykonuje kodu bo ja teraz pracuje (blocked[id] = true oraz turn = id)