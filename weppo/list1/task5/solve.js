function fib_rec(n) { // O(phi ^ n)
    if (n == 0)
        return 0
    if (n == 1)
        return 1
    return fib_rec(n - 1) + fib_rec(n - 2)
}

function fib_iter(n) { // O(n)
    let a = 0;
    let b = 1;
    for (let i = 0; i < n; i++) {
        let c = a + b;
        a = b;
        b = c;
    }
    return a;
}

for (let n = 10; n <= 39; n++) {
    console.time("fib_rec");
    console.log(n, fib_rec(n));
    console.timeEnd("fib_rec");

    console.time("fib_iter");
    console.log(n, fib_iter(n));
    console.timeEnd("fib_iter");
}