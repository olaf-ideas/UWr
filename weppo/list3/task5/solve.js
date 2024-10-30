function createGenerator(n) {
    var _state = 0;
    return {
        next: function() {
            return {
                value: _state,
                done: _state++ >= n
            }
        }
    }
};

var foo = {
    [Symbol.iterator] : function() { return createGenerator(10) }
};

var foo1 = {
    [Symbol.iterator] : function() { return createGenerator(30) }
};

var foo2 = {
    [Symbol.iterator] : function() { return createGenerator(42) }
};

for (var f of foo1) {
    console.log(f);
}