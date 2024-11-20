const stdin = process.openStdin();

var hidden_value = Math.floor(Math.random() * 101);

function game(d) {
    // console.log("d:" , d, Number(d));
    if (!isNaN(Number(d))) {
        if (d > hidden_value) {
            console.log('Podana wartość jest większa od ukrytej');
        }
        else
        if (d < hidden_value) {
            console.log('Podana wartość jest mniejsza od ukrytej');
        }
        else {
            console.log("Sukcess!!!");
            process.exit();
        }
    }
    else {
        console.log('Podana wartość nie jest liczbą.');
    }
}

console.log("Odgadnij liczbę: ");
stdin.addListener('data', d => game(d.toString()));