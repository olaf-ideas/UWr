(* CSE *)

(* zmien wyrażenie e na kanoniczną formę do alpha porównywań*)
let alpha_form (e : expr) (stk : 'a list) : expr = 
  match e with
  | Int n -> Int n
  | Bool b -> Bool b
  | Binop (op, b, c) -> Binop (op, alpha_form b stk, alpha_form c stk)
  | If (a, b, c) -> If (alpha_form a stk, alpha_form b stk, alpha_form c stk)
  | Var x ->
    (match List.find_index (fun y -> compare y x = 0) stk then
      | None -> Var x
      | Some i -> Var ("variable" ^ Int.to_string i))
  | Let (id, b, c) -> Let (id, alpha_form b stk, alpha_form c (id :: stk))

(* zbierz wszystkie podwyrażenia oraz ich kanoniczną formę *)
let gather_subexpr (e : expr) : ((expr * expr) list) =
  let gather_subexpr_help (e : expr) (stk : 'a list) : ((expr * expr) list) =   
    (e, alpha_form e) :: 
    (match e with
      | Int n -> []
      | Bool b -> []
      | Binop (b1, e1, e2) ->
        (gather_subexpr_help e1 stk) @ 
        (gather_subexpr_help e2 stk)
      | If (a, b, c) ->
        (gather_subexpr_help a stk) @
        (gather_subexpr_help b stk) @
        (gather_subexpr_help c stk)
      | Var v -> []
      | Let (id, b, c) ->
        (gather_subexpr_help b stk) @
        (gather_subexpr_help b (id :: stk)))
  in gather_subexpr_help e []

(* znajdź kanoniczną formę która występuje dwa razy *)
let find_equivalent (e : expr) : (expr option) =
  let find_equivalent_help (se : ((expr * expr) list)) : (expr option) =
    match se with
    | [] -> None
    | (e, alpha_e) :: se' ->
      (match List.find_opt (fun (e', alpha_e') -> compare alpha_e' alpha_e = 0) se' with
        | Some (e', alpha_e') -> Some alpha_e
        | None -> find_equivalent_help se')
  in find_equivalent_help (gather_subexpr e)

let cse (e : expr) : (expr option) =
  (* zastąp każde wystąpienie e_alpha na var w wyrażeniu e*)
  let cse_apply e e_alpha var stk =
    if compare (alpha_form e stk) e_alpha = 0 then
      var
    else
    (match e with
    | Binop (op, e1, e2) -> 
        Binop (op, 
              cse_apply e1 e_alpha var, 
              cse_apply e2 e_alpha var)
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
  | Some e_alpha -> cse_apply e e_alpha []
  | None -> None


