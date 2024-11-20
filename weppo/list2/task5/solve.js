const krowa = {
    _waga: 234233,

    get waga() {
        return this._waga;
    },

    set waga(ile) {
        this._waga = ile;
    }
};

krowa.costam = "Ilestam";
krowa.robcos = function() {
    console.log("32442342");
};

Object.defineProperty(krowa, "wiek", {
    get() {
        return this._wiek;
    },

    set(ile) {
        console.log("internal wiek setter");
        this._wiek = ile;
    }
});

console.log(krowa);

krowa.wiek = 8;

console.log(krowa);

// gettery i settery do wlasciwosci musza byc dodane przez defineProperty