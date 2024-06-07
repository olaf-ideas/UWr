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

let rec add_leave_at_return (local_vars : var list) (cmds : cmd list) : cmd list =
  match cmds with
  | [] -> []
  | RET :: cmds -> [LEAVE (List.length local_vars); RET] @ add_leave_at_return local_vars cmds
  | c :: cmds -> c :: add_leave_at_return local_vars cmds

let rec print_dict (dict : (name * name) list) =
  match dict with
  | (a, b) :: dict -> Printf.printf "%s %s\n" a b; print_dict dict
  | _ -> ()

let rec print_local_funs local_funs =
  match local_funs with
  | Func (name, _, _, _, _) :: local_funs -> 
    Printf.printf "%s " name; print_local_funs local_funs
  | _ -> Printf.printf "\n"

let rec apply_renaming (dict : (name * name) list) (cmds : cmd list) : cmd list =
  match cmds with
  | [] -> []
  | CALL f :: cmds ->
    (match List.assoc_opt f dict with
    | Some h -> Printf.printf "In dict %s -> %s\n" f h; print_dict dict; CALL h
    | None   -> Printf.printf "Not in dict %s\nDict:" f; print_dict dict; CALL f) :: apply_renaming dict cmds
  | WHILE (c1, c2) :: cmds ->
    WHILE(apply_renaming dict c1, apply_renaming dict c2) :: apply_renaming dict cmds
  | BRANCH (c1, c2) :: cmds ->
    BRANCH (apply_renaming dict c1, apply_renaming dict c2) :: apply_renaming dict cmds
  | c :: cmds -> c :: apply_renaming dict cmds

let rec add_prefix_to_funs (prefix : name) (funs : func list) : (func list) =
  match funs with
  | [] -> []
  | Func (name, args, local_vars, local_funs, body) :: funs ->
    Func (prefix ^ name, args, local_vars, local_funs, body) :: add_prefix_to_funs prefix funs

let rec compile_funs (env : var option list) (dict : (name * name) list) (funs : func list) : (name * cmd list) list =
  match funs with
  | [] -> []
  | Func (name, args, local_vars, local_funs, body) :: funs ->
    let env = (List.map Option.some local_vars) @ [None] @ (List.rev (List.map Option.some args)) @ env in
    let new_dict = dict @ (List.map (fun (Func (fun_name, _, _, _, _)) -> (fun_name, name ^ "_" ^ fun_name)) local_funs) in
    Printf.printf "Old dict:\n"; print_dict dict;
    Printf.printf "New dict:\n"; print_dict new_dict;
    Printf.printf "Local funs: "; print_local_funs local_funs;
    let local_funs = add_prefix_to_funs (name ^ "_") local_funs in
    let fun_code = compile_stmt env body in
    let fun_code = [ENTER (List.length local_vars)] @ fun_code @ [RET] in
    let fun_code = add_leave_at_return local_vars fun_code in
    let fun_code = apply_renaming new_dict fun_code in
    (name, fun_code) :: compile_funs env new_dict local_funs @ compile_funs env dict funs

let compile_prog ((vars, funs, main_stmt) : prog) : vm_prog =
  let env = List.map Option.some vars in
  let compiled_funcs = compile_funs env [] funs in
  let main_code = compile_stmt env main_stmt in
  ([ENTER (List.length vars)] @ main_code @ [LEAVE (List.length vars)], compiled_funcs)
