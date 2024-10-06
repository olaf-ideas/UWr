open Ast

module M = Map.Make(String)

type env = tp M.t

let env_empty = M.empty
let env_add env x tp = M.add x tp env
let env_lookup env x = M.find_opt x env
let rec infer_type env (e : expr) =
  match e with
  | Unit   -> TUnit
  | Int  _ -> TInt
  | Bool _ -> TBool
  | Var  x ->
    (match env_lookup env x with
    | Some tp -> tp
    | None    -> failwith ("Unbound variable " ^ x))
  | Let (x, e1, e2) ->
    let tp = infer_type env e1 in
    infer_type (env_add env x tp) e2
  | Fun (x, tp1, e) ->
    TArr(tp1, infer_type (env_add env x tp1) e)
  | App (e1, e2) ->
    (match infer_type env e1 with
    | TArr(tp', tp) ->
      check_type env e2 tp';
      tp
    | _ -> failwith "type error")
  | Binop(op,e1,e2) ->
    if op =  And || op = Or then
      (match infer_type env e1, infer_type env e2 with
        |TBool, TBool -> TBool
        | _ -> failwith "type error" 
      )
    else if op = Geq || op = Neq || op = Eq || op == Lt || op == Gt || op == Leq then
      (match infer_type env e1, infer_type env e2 with
      |TInt,TInt -> TBool
      | _ -> failwith "type error" 
    )
    else
      (match  infer_type env e1, infer_type env e2 with
      | TInt,TInt -> TInt
      | _ -> failwith "type eror"
      )
  | If(w,e1,e2) ->
    if infer_type env w = TBool then
      let t1 = infer_type env e1 in
      check_type env e2 t1;t1
    else
      failwith "type error in if"
  | Pair(e1,e2) -> TPair( infer_type env e1, infer_type env e2)
  | Fst e -> 
    ( match infer_type env e with
    | TPair(t1,_) -> t1
    | _ -> failwith "type error fst applied not to pair"
    )
  | Snd e -> 
    ( match infer_type env e with
    | TPair(_,t2) -> t2
    | _ -> failwith "type error fst applied not to pair"
    )

and check_type env e tp =
  let tp' = infer_type env e in
  if tp = tp' then ()
  else
    failwith "type error"

let check_program p =
  let _ : tp = infer_type env_empty p in
  p
