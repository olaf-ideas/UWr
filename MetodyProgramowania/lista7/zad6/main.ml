type ident = string

type qbf =
  | Top
  | Bot
  | Var of ident
  | Forall of ident * qbf
  | Exists of ident * qbf
  | Not of qbf
  | Conj of qbf * qbf
  | Disj of qbf * qbf

(* Zad 6. *)

let rec subst (x : ident) (s : qbf) (f : qbf) : qbf =
	match f with
	| Var y -> if x = y
				 then s
				 else f
	| Forall (y, t) -> Forall (y, subst x s t)
	| Exists (y, t) -> Exists (y, subst x s t)
	| Not y -> Not (subst x s y)
	| Conj (p, q) -> Conj (subst x s p, subst x s q)
	| Disj (p, q) -> Disj (subst x s p, subst x s q)
	| _ -> f

let rec eval (f : qbf) : bool =
	match f with
	| Top -> true
	| Bot -> false
	| Var x -> failwith ("unbound value " ^ x)
	| Forall (x, t) -> (eval (subst x Top t)) && (eval (subst x Bot t))
	| Exists (x, t) -> (eval (subst x Top t)) || (eval (subst x Bot t))
	| Not x -> not (eval x)
	| Conj (p, q) -> (eval p) && (eval q)
	| Disj (p, q) -> (eval p) || (eval q)

(* Zad 7. *)
  
module M = Map.Make(String)

type env = bool M.t

let rec eval_env (env : env) (f : qbf) : bool =
	match f with
	| Top -> true
	| Bot -> false
	| Var x ->
		(match M.find_opt x env with
		| Some v -> v
		| None -> failwith ("unbound value" ^ x))
	| Forall (x, t) ->
		let env_T = M.update x (fun _ -> Some true) env in
		let env_F = M.update x (fun _ -> Some false) env in
		(eval_env env_T t) && (eval_env env_F t)
	| Exists (x, t) ->
		let env_T = M.update x (fun _ -> Some true) env in
		let env_F = M.update x (fun _ -> Some false) env in
		(eval_env env_T t) || (eval_env env_F t)
	| Not x -> not (eval_env env x)
	| Conj (p, q) -> (eval_env env p) && (eval_env env q)
	| Disj (p, q) -> (eval_env env p) || (eval_env env q)
