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

function* fib_gen() {
    let a = 0, b = 1;
    for (;;) {
        yield a;
        let c = a + b;
        a = b;
        b = c;
    }
}

function* take(it, top) {
    while (top--) {
        let res = it.next();
        yield res.value;
    }
}

for (let num of take(fib(), 10)) {
    console.log(num);
}
