function forEach(a, f) {
    for (let i = 0; i < a.length; i++) {
        f(a[i], i);
    }
}

function map(a, f) {
    let res = [];
    for (let i = 0; i < a.length; i++) {
        res.push(f(a[i], i));
    }
    return res;
}

function filter(a, f) {
    let res = [];
    for (let i = 0; i < a.length; i++) {
        if (f(a[i], i)) {
            res.push(a[i]);
        }
    }
    return res;
}

var a = [1, 2, 3, 4];

forEach( a, _ => { console.log( _ ); } );

console.log(filter( a, _ => _ < 3 ));

console.log(map( a, _ => _ * 2 ));

console.log(filter( a, function(x, i) { return x % 2 == 0; }));