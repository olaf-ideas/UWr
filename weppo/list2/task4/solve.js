// typeof zwraca typ zmiennej jako string
// instanceof sprawdza czy zmienna nalezy do klasy lub jej pochodnej (nie obsluguje prymitywow, np int'ow)

console.log(typeof undefined);
console.log(typeof 2137);
console.log(typeof false);

const krowa = {
    waga: 243324
};

console.log(krowa instanceof Object);
console.log(krowa instanceof Array);

const tablica = [2, 1, 3, 7];

console.log(tablica instanceof Array);
console.log(tablica instanceof Object);