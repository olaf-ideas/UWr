const https = require('https');
const fs = require('fs');

const options = {
	pfx: fs.readFileSync('cert.pfx'),
	passphrase: 'weppo'
};

https.createServer(options, (req, res) => {
	res.setHeader('Content-Type', 'text/html; charset=utf-8');
	res.writeHead(200);
	res.end("<h1>SSL smiga</h1>");
}).listen(3000, () => {
 	console.log("Serwer HTTPS na https://localhost:3000");
});

