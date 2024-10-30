const costam = {
    ilestam: 20,
};

const krowa = {
    waga: 100,
    "jak robi krowa": "muuu",
    5: 8,
    costam: "wartosc",
    "[object Object]": "tutaj"
};

console.log(krowa);

console.log(krowa.waga);
console.log(krowa["jak robi krowa"]);
console.log(krowa[5]);
console.log(krowa[costam]); // costam jest zamieniane na napis '[object Object]'
console.log(krowa["5"]);

// . dziala tylko dla zwyklych naz
// [] dziala dla nietypowych kluczy typu napis ze spacjami oraz liczb
// nalezy pamietac o tym ze wszystko jest konwertowane na string, takze liczby

const tablica = [2, 1, 3, 7];

tablica["krowa"] = "costam"; // element zostanie dodany na koniec tablicy jak klucz wartosc
console.log(tablica);

tablica[krowa] = "hehe"; // gdy argumentem jest jakis obiekt zostaje on zkonwertowany na string
console.log(tablica);

tablica.length = 3;
console.log(tablica); // gdy dlugosc jest zkracana to usuwane sa tylko elementy z liczbowych indeksow

tablica.length = 8;
console.log(tablica); // wydluzajac liste dodawane sa puste elementy

tablica.length = -5;
console.log(tablica); // ustawiajac na ujemna wartosc js wykrzacza sie i stwierdza ze jest to niepoprawna dlugosc