const fs = require('fs');
const fsp = require('fs').promises;
const util = require('util');

fs.readFile('file.txt', 'utf8', (err, data) => {
 	console.log('Data:', data);
});

function rf_promise(path, encoding) {
    return new Promise((resolve, reject) => {
        fs.readFile(path, encoding, (err, data) => {
            if (err) reject(err);
            else resolve(data);
        });
    });
}

rf_promise('file.txt', 'utf8').then(data => console.log('Data:', data))

// promisify
const read_promisify = util.promisify(fs.readFile);
read_promisify('file.txt', 'utf8').then(data => console.log('Data:', data))

//fs.promises
const read_promised = fsp.readFile;
read_promised('file.txt', 'utf8').then(data => console.log('Data:', data))

async function read_await() {
	const data = await read_promised('file.txt', 'utf8');
	console.log('Dane:', data);
}

read_await();
