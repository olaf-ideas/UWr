open Ast

module M = Map.Make(String)

type env = tp M.t

let env_empty = M.empty
let env_add env x tp = M.add x tp env
let env_lookup env x = M.find_opt x env

let rec occurs x (tp : tp) =
  match tp_view tp with
  | TUVar y -> uvar_equal x y
  | TInt | TBool | TUnit -> false
  | TArr(tp1, tp2) ->
    occurs x tp1 || occurs x tp2
  | TPair(tp1,tp2) ->
    occurs x tp1 || occurs x tp2

let rec unify_type tp1 tp2 =
  match tp_view tp1, tp_view tp2 with
  | TUVar x, TUVar y when uvar_equal x y -> ()
  | TUVar x, _ ->
    if occurs x tp2 then failwith "unification error"
    else set_uvar x tp2
  | _, TUVar x ->
    if occurs x tp1 then failwith "unification error"
    else set_uvar x tp1
  | TInt, TInt -> ()
  | TInt, _    -> failwith "unification error"
  | TBool, TBool -> ()
  | TBool, _    -> failwith "unification error"
  | TUnit, TUnit -> ()
  | TUnit, _    -> failwith "unification error"
  | TArr(ta1, tb1), TArr(ta2, tb2) ->
    unify_type ta1 ta2;
    unify_type tb1 tb2
  | TArr _, _    -> failwith "unification error"
  | TPair(ta1,tb1), TPair(ta2,tb2)->
    unify_type ta1 ta2;
    unify_type tb1 tb2
  | TPair _, _    -> failwith "unification error"
  

let rec infer_type env (e : expr) =
  match e with
  | Unit   -> t_unit
  | Int  _ -> t_int
  | Bool _ -> t_bool
  | Var  x ->
    (match env_lookup env x with
    | Some tp -> tp
    | None    -> failwith ("Unbound variable " ^ x))
  | Let (x, e1, e2) ->
    let tp = infer_type env e1 in
    infer_type (env_add env x tp) e2
  | Fun (x, tp1, e) ->
    t_arr tp1 (infer_type (env_add env x tp1) e)
  | App (e1, e2) ->
    (match tp_view (infer_type env e1) with
    | TArr(tp', tp) ->
      check_type env e2 tp';
      tp
    | TUVar r ->
      let tp' = fresh_uvar () in
      let tp  = fresh_uvar () in
      set_uvar r (t_arr tp' tp);
      check_type env e2 tp';
      tp
    | _ -> failwith "type error")
  | Pair(e1,e2) -> t_pair (infer_type env e1) (infer_type env e2)
  | Fst e -> 
    (match tp_view (infer_type env e) with
    | TPair(t1,_) -> t1
    | TUVar r ->
      let tp' = fresh_uvar () in
      let tp  = fresh_uvar () in
      set_uvar r (t_pair tp' tp);
      tp'
    | _ -> failwith "type error")
  | Snd e -> 
    (match tp_view (infer_type env e) with
    | TPair(_,t2) -> t2
    | TUVar r ->
      let tp' = fresh_uvar () in
      let tp  = fresh_uvar () in
      set_uvar r (t_pair tp' tp);
      tp
    | _ -> failwith "type error")
  | LetRec(dets, e) -> 
  	infer_type (rec_type env dets) e
  | Binop(op,e1,e2) ->
    if op = And || op = Or then
      (match tp_view (infer_type env e1), tp_view(infer_type env e2) with
        | TBool, TBool -> t_bool
		| TUVar _, TUVar _ -> t_bool
        | TUVar _, TBool -> t_bool
        | TBool, TUVar _ -> t_bool
        | _ -> failwith "type error")
    else if op = Geq || op = Neq || op = Eq || op == Lt || op == Gt || op == Leq then
      (match tp_view (infer_type env e1), tp_view(infer_type env e2) with
      	| TInt, TInt -> t_bool
      	| TUVar _, TUVar _ -> t_bool
      	| TUVar _, TInt -> t_bool
      	| TInt, TUVar _ -> t_bool
      	| _ -> failwith "type error")
    else
      (match tp_view (infer_type env e1), tp_view(infer_type env e2) with
      	| TInt, TInt -> t_int
      	| TUVar _, TUVar _ -> t_int
      	| TUVar _, TInt -> t_int
      	| TInt, TUVar _ -> t_int
      	|  _ -> failwith "type eror")
  | If(w,e1,e2) ->
    if tp_view (infer_type env w) = TBool then
      let t1 = infer_type env e1 in
      check_type env e2 t1;t1
    else
      failwith "type error"
  | Raise -> fresh_uvar()
  | Try (e1,e2) -> 
    let t1 = infer_type env e1 in
    check_type env e2 t1;
    t1

and check_type env e tp =
  let tp' = infer_type env e in
  unify_type tp tp'

and rec_type env dets = 
  let env, dets = List.fold_left_map (fun acc (cname, e) ->
    let uv = fresh_uvar () in 
    (env_add acc cname uv, (e, uv))) env dets in
  List.iter (fun (e, uv) -> check_type env e uv) dets;
  env
    (* match dets with
  | [] -> env
  | (name,e) :: dets ->
    let tp = infer_type env e in
    rec_type (env_add env name tp) dets *)

let check_program p =
  let _ : tp = infer_type env_empty p in
  p
