
for (let n = 1; n <= 100000; n++) {
	let ok = true;
	let n_str = n.toString();
	let sum = 0;
	for (let i = 0; i < n_str.length; i++) {
		let digit = Number(n_str[i]);
		if (digit == 0 || n % digit != 0) {
			ok = false;
		}
		sum += digit;
	}

	if (sum == 0 || n % sum != 0) {
		ok = false;
	}

	if (ok) {
		console.log(n);
	}
}
