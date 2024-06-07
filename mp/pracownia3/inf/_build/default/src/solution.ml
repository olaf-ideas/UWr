open Ast

let rec print_env env =
  match env with
  | e :: env ->
    (match e with
    | Some x -> Printf.printf "%s " x
    | None -> Printf.printf "None ");
    print_env env
  | [] -> Printf.printf ";\n"

let rec compile_aexp (env : var option list) (a : aexp) : cmd list =
  match a with
  | Int n -> [CONST n]
  | Var x ->
    (* print_env env; *)
    (match List.find_index (fun y -> y = Some x) env with
    | Some i -> [TOP; LOAD i]
    | None   -> failwith ("Variable " ^ x ^ " not found"))
  | Binop (op, a1, a2) ->
      let c1 = compile_aexp env a1 in
      let c2 = compile_aexp (None :: env) a2 in
      c1 @ [PUSH] @ c2 @ [PRIM op]
  | Call (f, args) ->
    let rec prepare_args env args =
      match args with
      | [] -> []
      | a :: args -> compile_aexp env a @ [PUSH] @ prepare_args (None :: env) args in
    prepare_args env args @ [CALL f] @ [LEAVE (List.length args)]
    (* List.concat (List.map (fun a -> compile_aexp env a @ [PUSH]) args) @ [CALL f] *)

let rec compile_bexp (env : var option list) (b : bexp) : cmd list =
  match b with
  | Bool true  -> [CONST 1]
  | Bool false -> [CONST 0]
  | Cmp (op, a1, a2) ->
    compile_aexp env a1 @ [PUSH] @ compile_aexp (None :: env) a2 @ [CMP op]
  | And (b1, b2) ->
    let c1 = compile_bexp env b1 in
    let c2 = compile_bexp env b2 in
    c1 @ [BRANCH (c2, [CONST 0])]
  | Or (b1, b2) ->
    let c1 = compile_bexp env b1 in
    let c2 = compile_bexp env b2 in
    c1 @ [BRANCH ([CONST 1], c2)]
  | Not b ->
    compile_bexp env b @ [PUSH; CONST 0; CMP Eq]

let rec compile_stmt (env : var option list) (s : stmt) : cmd list =
  match s with 
  | Block stmts -> List.concat (List.map (compile_stmt env) stmts)
  | Assgn (x, a) ->
    let ca = compile_aexp (None :: env) a in
    (match List.find_index (fun y -> y = Some x) env with
    | Some i -> [TOP; PUSH] @ ca @ [STORE i]
    | None   -> failwith ("Variable " ^ x ^ " not found"))
  | If (b, s1, s2) ->
    let cb = compile_bexp env b in
    let cs1 = compile_stmt env s1 in
    let cs2 = compile_stmt env s2 in
    cb @ [BRANCH (cs1, cs2)]
  | While (b, s) ->
    let cb = compile_bexp env b in
    let cs = compile_stmt env s in
    [WHILE (cb, cs)]
  | Read x ->
    (match List.find_index (fun y -> y = Some x) env with
    | Some i -> [TOP; PUSH; READ; STORE i]
    | None   -> failwith ("Variable" ^ x ^ "not found"))
  | Write a ->
    compile_aexp env a @ [WRITE]
  | Return a ->
    compile_aexp env a @ [RET]

(* local funcs are not used for now*)
let rec compile_func (env : var option list) (Func (name, args, local_vars, local_funs, body) : func) : (name * cmd list) list = 
  let rename_local_func = fun (Func (func_name, args, local_vars, local_funs, body)) -> Func (name ^ "_" ^ func_name, args, local_vars, local_funs, body) in
  let compiled_local_funcs = List.concat (List.map (compile_func env) (List.map rename_local_func local_funs)) in
  let env = (List.map Option.some local_vars) @ [None] @ (List.rev (List.map Option.some args)) @ env in
  let rec rename_local_func_calls cmds : cmd list =
    match cmds with
    | [] -> []
    | CALL f :: cmds ->
      (match List.find_opt (fun (Func (h, _, _, _, _)) -> h = f) local_funs with
      | Some _ -> CALL (name ^ "_" ^ f)
      | None   -> CALL f) :: cmds
    | c :: cmds -> c :: rename_local_func_calls cmds in
    let func_body = rename_local_func_calls (compile_stmt env body) in
    let rec delete_local_vars cmds : cmd list =
    match cmds with
    | [] -> []
    | RET :: cmds -> [LEAVE (List.length local_vars); RET] @ delete_local_vars cmds
    | c :: cmds -> c :: delete_local_vars cmds in
  (name, [ENTER (List.length local_vars)] @ (delete_local_vars func_body)) :: compiled_local_funcs

let compile_prog ((vars, funcs, main_stmt) : prog) : vm_prog =
  let env = List.map Option.some vars in
  let compiled_funcs = List.concat (List.map (compile_func env) funcs) in
  let main_code = compile_stmt env main_stmt in
  ([ENTER (List.length vars)] @ main_code @ [LEAVE (List.length vars)], compiled_funcs)
