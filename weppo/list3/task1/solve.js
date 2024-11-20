function memo_fib_rec(n) {
    var cache = {};

    function fib_rec(n) {
        if (n <= 1) {
            return n;
        }
        if (n in cache) {
            return cache[n];
        }
        return cache[n] = fib_rec(n - 1) + fib_rec(n - 2);
    }

    return fib_rec(n);
}

function fib_rec(n) {
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    return fib_rec(n - 1) + fib_rec(n - 2);
}

// function memoize(fn) {
//     var cache = {};

//     return function(n) {
//         if (n in cache) {
//             return cache[n];
//         }
//         else {
//             var result = fn(n);
//             cache[n] = result;
//             return result;
//         }
//     }
// };

// let memo_fib_rec = memoize(fib_rec);

for (let n = 10; n <= 30; n++) {
    console.time("fib_rec");
    console.log(n, fib_rec(n));
    console.timeEnd("fib_rec");

    console.time("memo_fibrec");
    console.log(n, memo_fib_rec(n));
    console.timeEnd("memo_fibrec");
}