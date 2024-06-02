open Ast

module M = Map.Make(String)

exception Type_error
exception Unbound_var of ident

exception MyExn

type env = value M.t

and value =
  | VUnit
  | VInt of int
  | VBool of bool
  | VClosure of pattern * expr * env
  | VPair of value * value
  | VCtor of cname * value

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
  | _ -> raise Type_error

let rec is_pattern_valid p blacklist =
  match p with
  | PPair (p1, p2) ->
   (match is_pattern_valid p1 blacklist with
   | Some blacklist -> is_pattern_valid p2 blacklist
   | None -> None)
  | PVar x -> 
   (if List.exists (fun y -> x = y) blacklist 
	then None
	else Some (x :: blacklist))
  | _ -> Some blacklist

let match_pattern env v p =
  let rec help_match_pattern env v p =
	  match v, p with
	  | _,       PWildcard  -> Some env
	  | VUnit,   PUnit      -> Some env
	  | _,       PUnit      -> None
	  | VInt n,  PInt m when n = m -> Some env
	  | _,       PInt _     -> None
	  | VBool x, PBool y when x = y -> Some env
	  | _,       PBool _    -> None
	  | _,       PVar  x    -> Some (M.add x v env)
	  | VCtor(c1, v), PCtor(c2, p) when c1 = c2 ->
		help_match_pattern env v p
	  | _, PCtor _ -> None
	  | VPair(v1, v2), PPair(p1, p2) ->
		(match help_match_pattern env v1 p1 with
		| None -> None
		| Some env -> help_match_pattern env v2 p2)
	  | _, PPair _ -> None
  in 
  (* help_match_pattern env v p *)
  if is_pattern_valid p [] = None
  then None
  else help_match_pattern env v p

let rec eval_env (env : env) (e : expr) : value =
  match e with
  | Unit  -> VUnit
  | Int n -> VInt n
  | Bool b -> VBool b
  | Ctor(c, e) -> VCtor(c, eval_env env e)
  | If (p, t, e) ->
      (match eval_env env p with
      | VBool true -> eval_env env t
      | VBool false -> eval_env env e
      | _ -> raise Type_error)
  | Binop (And, e1, e2) ->
      (match eval_env env e1 with
      | VBool true -> eval_env env e2
      | VBool false -> VBool false
      | _ -> raise Type_error)
  | Binop (Or, e1, e2) ->
      (match eval_env env e1 with
      | VBool false -> eval_env env e2
      | VBool true -> VBool true
      | _ -> raise Type_error)
  | Binop (op, e1, e2) -> eval_op op (eval_env env e1) (eval_env env e2)
  | Let (x, e1, e2) ->
      let r = eval_env env e1 in
	  (match match_pattern env r x with
	  | Some env -> eval_env env e2
	  | None -> raise Type_error)
  | Var x ->
      (match M.find_opt x env with
      | Some v -> v
      | None -> raise (Unbound_var x))
  | Fun (x, e) -> VClosure (x, e, env)
  | App (e1, e2) ->
      (match eval_env env e1, eval_env env e2 with
      | VClosure (x, body, clo_env), v -> 
	  	(match match_pattern clo_env v x with
		| Some clo_env -> eval_env clo_env body
        | None -> raise Type_error)
	  | _, _ -> raise Type_error)
  | Pair(e1, e2) ->
      VPair(eval_env env e1, eval_env env e2)
  | Fst e ->
      (match eval_env env e with
      | VPair(v1, _) -> v1
      | _ -> raise Type_error)
  | Snd e ->
      (match eval_env env e with
      | VPair(_, v2) -> v2
      | _ -> raise Type_error)
  | Raise -> raise MyExn
  | Try(e1, e2) ->
      (try eval_env env e1 with
      | MyExn -> eval_env env e2)
  | Match(e, cs) -> match_clauses env (eval_env env e) cs

and match_clauses env v cs =
  match cs with
  | [] -> failwith "match failure"
  | (p, x, e) :: cs ->
    match match_pattern env v p with
    | Some env -> 
		(match x with
		| Some x -> eval_env (M.add x v env) e
		| None -> eval_env env e)
    | None -> match_clauses env v cs

let eval_prog = eval_env M.empty

let rec string_of_value v =
  match v with
  | VUnit       -> "()"
  | VInt n      -> string_of_int n
  | VBool true  -> "true"
  | VBool false -> "false"
  | VClosure _  -> "<fun>"
  | VPair(v1, v2) ->
    "(" ^ string_of_value v1 ^ ", " ^ string_of_value v2 ^ ")"
  | VCtor(c, v) ->
   (match v with
   | VPair _ -> c ^ string_of_value v
   | VUnit -> c ^ "()"
   | VCtor _ -> c ^ "(" ^ string_of_value v ^ ")"
   | _ -> c ^ " " ^ string_of_value v)

let print_value v =
  print_endline (string_of_value v)
