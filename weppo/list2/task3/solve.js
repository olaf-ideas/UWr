console.log( (![]+[])[+[]]+(![]+[])[+!+[]]+([![]]+[][[]])[+!+[]+[+[]]]+(![]+[])[!+[]+!+[]] );

// [] <=> pusta tablica to true
// ![] <=> wartosc boolowska false
// ![] + [] <=> polaczenie z pusta tablica daje napis "false"
// "false"[+[]] <=> +[] konwertowane jest na 0 wiec jest to rownowazne "f"
// (![]+[])[+[]] <=> "f"

// +[] <=> false
// !+[] <=> !false <=> true
// +!+[] <=> 1
// (![]+[])[+!+[]] <=> "a"

// [![]] <=> [false]
// [![]] + [] <=> "false"
// [![]] + [] [[]] <=> "false" + undefined <=> "falseundefined"
// +!+[] + [+[]] <=> 1 + [0] <=> "10"
// "falseundefined"["10"] <=> "i"

// (![] + []) <=> "false"
// (![] + [])[!+[]+!+[]] <=> "false"[1 + 1] <=> "false"[2] <=> "l"

// => "fail"