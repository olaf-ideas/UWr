function fib() {
    let a = 0, b = 1;
    return {
        next: function() {
            let now = a;

            let c = a + b;
            a = b;
            b = c;
            return {
                value: now,
                done: false
            }
        }
    }
}

function *fib_gen() {
    let a = 0, b = 1;
    for (;;) {
        yield a;
        let c = a + b;
        a = b;
        b = c;
    }
}

for (let i of fib_gen()) {
    console.log(i);
    if (i > 100) {
        break;
    }
}