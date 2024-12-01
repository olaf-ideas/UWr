const fs = require('fs');
const readline = require('readline');

function processLogs(logFile) {
    var ips = {};

    const rl = readline.createInterface({
        input: fs.createReadStream(logFile),
        output: process.stdout,
        terminal: false
    });

    rl.on('line', (line) => {
        const ip = line.split(' ')[1];
        
        if (ip in ips) {
            ips[ip] += 1;
        }
        else {
            ips[ip] = 1;
        }
    });

    rl.on('close', () => {
        let sorted = Object.entries(ips).sort((a, b) => b[1] - a[1]);
        sorted = sorted.slice(0, 3);

        // console.log(sorted);
        sorted.forEach(([ip, num]) => {
            console.log(ip + " " + num);
        });
    });
};

processLogs('server_logs.txt');
