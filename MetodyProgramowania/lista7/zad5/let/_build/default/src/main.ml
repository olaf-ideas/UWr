open Ast

let parse (s : string) : expr =
  Parser.prog Lexer.read (Lexing.from_string s)

type value =
  | VInt of int
  | VBool of bool

let eval_op (op : bop) (v1 : value) (v2 : value) : value =
  match op, v1, v2 with
  | Add,  VInt i1, VInt i2 -> VInt (i1 + i2)
  | Sub,  VInt i1, VInt i2 -> VInt (i1 - i2)
  | Mult, VInt i1, VInt i2 -> VInt (i1 * i2)
  | Div,  VInt i1, VInt i2 -> VInt (i1 / i2)
  | Eq,   VInt i1, VInt i2 -> VBool (i1 = i2)
  | Lt,   VInt i1, VInt i2 -> VBool (i1 < i2)
  | Gt,   VInt i1, VInt i2 -> VBool (i1 > i2)
  | Leq,  VInt i1, VInt i2 -> VBool (i1 <= i2)
  | Geq,  VInt i1, VInt i2 -> VBool (i1 >= i2)
  | Neq,  VInt i1, VInt i2 -> VBool (i1 <> i2)
  | _ -> failwith "type error"


(* Evaluation via substitution *)

module Subst = struct

let rec subst (x : ident) (s : expr) (e : expr) : expr =
  match e with
  | Binop (op, e1, e2) -> Binop (op, subst x s e1, subst x s e2)
  | If (p, t, e) -> If (subst x s p, subst x s t, subst x s e)
  | Var y -> if x = y
               then s
               else e
  | Let (y, e1, e2) -> if x = y
                         then Let (y, subst x s e1, e2)
                         else Let (y, subst x s e1, subst x s e2)
  | Sum (y, n, m, k) ->
      Sum(y, subst x s n, subst x s m, subst x s k)
  | _ -> e
  
let expr_of_value (v : value) : expr =
  match v with
  | VInt a -> Int a
  | VBool a -> Bool a

let rec eval (e : expr) : value =
  match e with
  | Int n -> VInt n
  | Bool b -> VBool b
  | If (p, t, e) ->
      (match eval p with
      | VBool true -> eval t
      | VBool false -> eval e
      | _ -> failwith "type error")
  | Binop (And, e1, e2) ->
      (match eval e1 with
      | VBool true -> eval e2
      | VBool false -> VBool false
      | _ -> failwith "type error")
  | Binop (Or, e1, e2) ->
      (match eval e1 with
      | VBool false -> eval e2
      | VBool true -> VBool true
      | _ -> failwith "type error")
  | Binop (op, e1, e2) -> eval_op op (eval e1) (eval e2)
  | Let (x, e1, e2) -> let s = expr_of_value (eval e1) in
                       eval (subst x s e2)
  | Sum (x, n, m, k) ->
    (match eval(n), eval(m) with
    | VInt l, VInt r when l <= r ->
      (match (eval (Sum(x, Int(l + 1), Int(r), k))), (eval (subst x n k)) with
      | VInt sum, VInt s -> VInt(sum + s)
      | _ -> failwith "type error")
    | VInt _, VInt _ -> VInt 0
    | _ -> failwith "type error")
  | Var x -> failwith ("unbound value " ^ x)


let rec closed (e : expr) : bool =
  match e with
  | Int _ -> true
  | Bool _ -> true
  | If (p, t, e) -> closed(p) && closed(t) && closed(e)
  | Binop(_, e1, e2) -> closed(e1) && closed(e2)
  | Let (x, e1, e2) -> closed(e1) && (closed (subst x (expr_of_value (eval e1)) e2))
  | Sum (x, n, m, k) -> closed(n) && closed(m) && closed(subst x (expr_of_value (eval n)) k)
  | Var _ -> false

let interp (s : string) : value =
  eval (parse s)

end


(* Evaluation via environments *)

module Env = struct

module M = Map.Make(String)

type env = value M.t

let rec eval_env (env : env) (e : expr) : value =
  match e with
  | Int n -> VInt n
  | Bool b -> VBool b
  | If (p, t, e) ->
      (match eval_env env p with
      | VBool true -> eval_env env t
      | VBool false -> eval_env env e
      | _ -> failwith "type error")
  | Binop (And, e1, e2) ->
      (match eval_env env e1 with
      | VBool true -> eval_env env e2
      | VBool false -> VBool false
      | _ -> failwith "type error")
  | Binop (Or, e1, e2) ->
      (match eval_env env e1 with
      | VBool false -> eval_env env e2
      | VBool true -> VBool true
      | _ -> failwith "type error")
  | Binop (op, e1, e2) -> eval_op op (eval_env env e1) (eval_env env e2)
  | Let (x, e1, e2) ->
      let r = eval_env env e1 in
      let new_env = M.update x (fun _ -> Some r) env in
      eval_env new_env e2
  | Sum (x, n, m, k) ->
    (match (eval_env env n), (eval_env env m) with
    | VInt l, VInt r when l <= r ->
      let next_env = M.update x (fun _ -> Some (VInt(l))) env in
      (match (eval_env env (Sum(x, Int(l + 1), Int(r), k))), (eval_env next_env k) with
      | VInt sum, VInt s -> VInt(sum + s)
      | _ -> failwith "type error")
    | VInt _, VInt _ -> VInt 0
    | _ -> failwith "type error")
  | Var x ->
      (match M.find_opt x env with
      | Some v -> v
      | None -> failwith ("unbound value" ^ x))

let eval : expr -> value = eval_env M.empty 

let interp (s : string) : value =
  eval (parse s)

end