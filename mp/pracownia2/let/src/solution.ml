(* abstract syntax tree *)

open Ast
(* 
type bop = Mult | Div | Add | Sub | Eq | Lt | Gt | Leq | Geq | Neq

type ident = string

type expr =
  | Int of int
  | Bool of bool
  | Binop of bop * expr * expr
  | If of expr * expr * expr
  | Var of ident
  | Let of ident * expr * expr *)

(* CSE *)

(* zmien wyrażenie e na kanoniczną formę do alpha porównywań *)
let rec alpha_form (e : expr) (stk : 'a list) : expr = 
  match e with
  | Int n -> Int n
  | Bool b -> Bool b
  | Binop (op, b, c) -> Binop (op, alpha_form b stk, alpha_form c stk)
  | If (a, b, c) -> If (alpha_form a stk, alpha_form b stk, alpha_form c stk)
  | Var x ->
    (match List.find_index (fun y -> compare y x = 0) stk with
      | None -> Var x
      | Some i -> Var ("variable" ^ string_of_int i))
  | Let (id, b, c) -> Let ("", alpha_form b stk, alpha_form c (id :: stk))

(* zbierz wszystkie podwyrażenia oraz ich kanoniczną formę *)
let gather_subexpr (e : expr) : ((expr * expr) list) =
  let rec gather_subexpr_help (e : expr) (stk : 'a list) : ((expr * expr) list) = 
    (match e with
      | Int _ | Bool _ | Var _ -> []
      | _ -> [(e, alpha_form e stk)]
    ) @
    (match e with
      | Int _ | Bool _ | Var _ -> []
      | Binop (_, e1, e2) ->
        (gather_subexpr_help e1 stk) @ 
        (gather_subexpr_help e2 stk)
      | If (a, b, c) ->
        (gather_subexpr_help a stk) @
        (gather_subexpr_help b stk) @
        (gather_subexpr_help c stk)
      | Let (id, b, c) ->
        (gather_subexpr_help b stk) @
        (gather_subexpr_help c (id :: stk)))
  in gather_subexpr_help e []

(* znajdź kanoniczną formę która występuje dwa razy *)
let find_equivalent (e : expr) : ((expr * expr) option) =
  let rec find_equivalent_help (se : ((expr * expr) list)) : ((expr * expr) option) =
    match se with
    | [] -> None
    | (e, alpha_e) :: se' ->
      (match List.find_opt (fun (_, alpha_e') -> compare alpha_e' alpha_e = 0) se' with
        | Some (_, _) -> Some (e, alpha_e)
        | None -> find_equivalent_help se')
  in find_equivalent_help (gather_subexpr e)

let cse (e : expr) : (expr option) =
  (* zastąp każde wystąpienie e_alpha na var w wyrażeniu e *)
  let rec cse_apply (e : expr) (e_alpha : expr) (var : ident) (stk : 'a list) =
    if compare (alpha_form e stk) e_alpha = 0 then
      Var (var)
    else
    (match e with
    | Binop (op, e1, e2) -> 
        Binop (op, 
               cse_apply e1 e_alpha var stk, 
               cse_apply e2 e_alpha var stk)
    | If (a, b, c) -> 
        If (cse_apply a e_alpha var stk, 
            cse_apply b e_alpha var stk,
            cse_apply c e_alpha var stk)
    | Let (id, b, c) ->
        Let (id,
             cse_apply b e_alpha var stk,
             cse_apply c e_alpha var (id :: stk))
    | _ -> e)
  in match find_equivalent e with
  | Some (e', e_alpha) -> 
    let fresh_var = "cse_variable_" ^ string_of_int (Random.int 100000) in 
      Some (Let (fresh_var, e', cse_apply e e_alpha fresh_var []))
  | None -> None

let parse (s : string) : expr =
  Parser.prog Lexer.read (Lexing.from_string s)
  (* Test 1: Prosty przypadek, gdzie występują dwa identyczne wyrażenia *)

let test1 = "let x = 1 + 1 in x + (1 + 1)" (* Powinno znaleźć wspólne podwyrażenie 1 + 1 *)

(* Test 2: Dwa α-równoważne podwyrażenia *)
let test2 = "let x = let y = 2 + 2 in y in let z = 2 + 2 in z" (* α-równoważne: y = 2+2, z = 2+2 *)

(* Test 3: Różne wyrażenia *)
let test3 = "let x = 3 + 3 in let y = 4 + 4 in x + y" (* Brak wspólnych podwyrażeń *)

(* Test 4: Podwyrażenie z różnymi zmiennymi *)
let test4 = "let x = let y = 2 in y * y in let z = let w = 2 in w * w in x + z" (* α-równoważne: y * y, w * w *)

(* Test 5: Wyrażenia bez wspólnych podwyrażeń *)
let test5 = "let x = 1 + 2 in let y = 3 + 4 in x + y" (* Brak wspólnych podwyrażeń *)

(* Przetestowanie funkcji getCseChange *)
let run_tests () =
  let test_cases = [test1; test2; test3; test4; test5] in
  List.iter (fun test_case ->
    let expr = parse test_case in
    let result = cse(expr) in
    match result with
    | Some _ -> Printf.printf "CSE change found for: %s\n" test_case
    | None -> Printf.printf "No CSE change for: %s\n" test_case
  ) test_cases
(* Test 6: Wyrażenie z If i równoważnymi podwyrażeniami *)
let test6 = 
  "let x = if true then 2 else 3 in let y = if true then 2 else 3 in x + y" 
(* Wspólne podwyrażenia: (if true then 2 else 3) *)

(* Test 7: Zagnieżdżone Let z równoważnymi podwyrażeniami *)
let test7 = 
  "let a = 5 in let b = a * 2 in let c = a * 2 in b + c" 
(* Wspólne podwyrażenie: a * 2 *)

(* Test 8: If z zagnieżdżeniem *)
let test8 = 
  "let a = 10 in let b = if a > 5 then 2 else 3 in let c = if a > 5 then 2 else 3 in b + c" 
(* Wspólne podwyrażenie: (if a > 5 then 2 else 3) *)

(* Test 9: Wyrażenie z wolnymi zmiennymi *)
let test9 = 
  "let x = y + 1 in let z = y + 1 in x + z" 
(* Wspólne podwyrażenie: y + 1 *)

(* Test 10: If z wolnymi zmiennymi *)
let test10 = 
  "let a = y in let b = if a > x then 2 else 3 in let c = if a > x then 2 else 3 in b + c" 
(* Wspólne podwyrażenie: (if a > x then 2 else 3) *)

(* Funkcja testująca getCseChange z nowymi testami *)
let run_new_tests () =
  let test_cases = [test6; test7; test8; test9; test10] in
  List.iter (fun test_case ->
    let expr = parse test_case in
    let result = cse(expr) in
    match result with
    | Some _ -> Printf.printf "CSE change found for: %s\n" test_case
    | None -> Printf.printf "No CSE change for: %s\n" test_case
  ) test_cases
