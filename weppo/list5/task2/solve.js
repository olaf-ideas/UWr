const stdin = process.openStdin();
console.log('Wpisz swoje imię: ');
stdin.addListener('data', d => {
    console.log('Witaj ' + d.toString());
    process.exit();
});