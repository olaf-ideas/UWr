const express = require('express')
const app = express()
const port = 3000

app.use(express.urlencoded({ extended: true }));

app.get('/', (req, res) => {
	let tabelka_zajec = ''

	for (let i = 1; i <= 10; i++) {
		tabelka_zajec += `Zadanie ${i}: <input type="number" name="zadanie${i}" min="0" max="10" value="0"><br>`
	}

    res.send(`
        <form action="/print" method="POST">
            <h2>Formularz zgłoszenia zadań</h2>
            Imię: <input type="text" name="imie" required><br>
            Nazwisko: <input type="text" name="nazwisko" required><br>
            Nazwa zajęć: <input type="text" name="nazwa_zajec" required><br>
            <h3>Wyniki zadań:</h3>`
			+ tabelka_zajec +
			`<button type="submit">Wyślij</button>
        </form>
    `)
})

app.post('/print', (req, res) => {
    console.log(req.body)
	
	let html = `
        <h2>Pasek zgłoszenia zadań</h2>
        <p>Imię: ${req.body.imie}</p>
        <p>Nazwisko: ${req.body.nazwisko}</p>
        <p>Nazwa zajęć: ${req.body.nazwa_zajec}</p>
		`

	html += `<table>`
	html += `<tr><th>Zadanie</th><th>Punkty</th></tr>`
	for (let i = 1; i <= 10; i++) {
		html += `<tr><td>${i}</td><td>${req.body['zadanie' + i]}</td></tr>`
	}
	html += `</table>`
	html += `<a href="/">Cofnij</a>`

	res.send(html)
})

app.listen(port, () => {
	console.log(`Serwer śmiga na porcie ${port}`)
})

