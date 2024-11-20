const fs = require('fs');
const fsp = require('fs').promises;
const util = require('util');

// Klasyczny sposób z użyciem callback
fs.readFile('example.txt', 'utf8', (err, data) => {
  if (err) {
    console.error('Błąd:', err);
    return;
  }
  console.log('Dane (klasyczny):', data);
});


// ręcznie napisana funkcjs
function rf_promise(path, encoding) {
    return new Promise((resolve, reject) => {
        fs.readFile(path, encoding, (err, data) => {
            if (err) reject(err);
            else resolve(data);
        });
    });
}

// readFilePromise('example.txt', 'utf8')
//     .then(data => console.log('Dane:\n', data))
//     .catch(err => console.error('Bład:', err))


// promisify
const read_promisify = util.promisify(fs.readFile);
// readFileAsync("example.txt",'utf8')
//     .then(data => console.log('Dane:\n', data))
//     .catch(err => console.error('Bład:', err))

//fs.promises
const read_promised = fsp.readFile;
// read_promised("example.txt",'utf8')
//     .then(data => console.log('Dane:\n', data))
//     .catch(err => console.error('Bład:', err))

async function read_await() {
    try {
        const data = await read_promised('example.txt', 'utf8');
        console.log('Dane:', data);
    } catch (err) {
        console.error('Error:', err);
    }
}

read_await();
