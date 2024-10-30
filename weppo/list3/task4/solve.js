function sum(...args) {
    let res = 0;
    for (let v of args) {
        res += v;
    }
    return res;
}

console.log(sum(1, 2, 3));
console.log(sum(1, 2, 3, 4, 5));