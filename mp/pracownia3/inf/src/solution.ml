open Ast

let father_dict : ((name * name) list ref) = ref []

let rec lookup_for_variable_addr (env : var option list) (x : var) (index : int) : (int * cmd list) =
  match env with
  | [] -> failwith ("Variable " ^ x ^ " not found")
  | Some y :: _ when y = x -> (index, [])
  | Some y :: env when (String.starts_with ~prefix:"stack_pointer" y) -> 
    let (idx, cmds) = lookup_for_variable_addr env x 0 in
    (idx, LOAD index :: cmds)
  | _ :: env -> lookup_for_variable_addr env x (index + 1)

let rec lookup_for_function_env (env : var option list) (father_name : name) (index : int) : cmd list =
  match env with
  | [] -> failwith ("Function: " ^ father_name ^ " not found")
  | Some y :: _ when y = ("stack_pointer:" ^ father_name) -> [LOAD index]
  | Some y :: env when (String.starts_with ~prefix:"stack_pointer" y) ->
    LOAD index :: lookup_for_function_env env father_name 0
  | _ :: env -> lookup_for_function_env env father_name (index + 1)

let rec compile_aexp (env : var option list) (dict : (name * name) list) (a : aexp) : cmd list =
  match a with
  | Int n -> [CONST n]
  | Var x ->
    let (idx, cmds) = lookup_for_variable_addr env x 0 in
    [TOP] @ cmds @ [LOAD idx]
  | Binop (op, a1, a2) ->
      let c1 = compile_aexp env dict a1 in
      let c2 = compile_aexp (None :: env) dict a2 in
      c1 @ [PUSH] @ c2 @ [PRIM op]
  | Call (f, args) ->
    let rec prepare_args env args =
      match args with
      | [] -> []
      | a :: args -> compile_aexp env dict a @ [PUSH] @ prepare_args (None :: env) args in
    let cmds = lookup_for_function_env env (List.assoc (List.assoc f dict) !father_dict) 0 in
    [TOP] @ cmds @ [PUSH] @ (prepare_args (None :: env) args) @ [CALL f] @ [LEAVE (List.length args + 1)]

let rec compile_bexp (env : var option list) (dict : (name * name) list) (b : bexp) : cmd list =
  match b with
  | Bool true  -> [CONST 1]
  | Bool false -> [CONST 0]
  | Cmp (op, a1, a2) ->
    let c1 = compile_aexp env dict a1 in
    let c2 = compile_aexp (None :: env) dict a2 in
    c1 @ [PUSH] @ c2 @ [CMP op]
  | And (b1, b2) ->
    let c1 = compile_bexp env dict b1 in
    let c2 = compile_bexp env dict b2 in
    c1 @ [BRANCH (c2, [CONST 0])]
  | Or (b1, b2) ->
    let c1 = compile_bexp env dict b1 in
    let c2 = compile_bexp env dict b2 in
    c1 @ [BRANCH ([CONST 1], c2)]
  | Not b ->
    compile_bexp env dict b @ [PUSH; CONST 0; CMP Eq]

let rec compile_stmt (env : var option list) (dict : (name * name) list) (s : stmt) : cmd list =
  match s with 
  | Block stmts -> List.concat (List.map (compile_stmt env dict) stmts)
  | Assgn (x, a) ->
    let ca = compile_aexp (None :: env) dict a in
    let (idx, cmds) = lookup_for_variable_addr env x 0 in
    [TOP] @ cmds @ [PUSH] @ ca @ [STORE idx]
  | If (b, s1, s2) ->
    let cb = compile_bexp env dict b in
    let cs1 = compile_stmt env dict s1 in
    let cs2 = compile_stmt env dict s2 in
    cb @ [BRANCH (cs1, cs2)]
  | While (b, s) ->
    let cb = compile_bexp env dict b in
    let cs = compile_stmt env dict s in
    [WHILE (cb, cs)]
  | Read x ->
    let (idx, cmds) = lookup_for_variable_addr env x 0 in
    [TOP] @ cmds @ [PUSH; READ; STORE idx]
  | Write a ->
    compile_aexp env dict a @ [WRITE]
  | Return a ->
    compile_aexp env dict a @ [RET]

let rec pop_at_return (local_vars : var list) (cmds : cmd list) : cmd list =
  match cmds with
  | [] -> []
  | RET :: cmds -> [LEAVE (List.length local_vars + 1); RET] @ pop_at_return local_vars cmds
  | WHILE (c1, c2) :: cmds ->
    WHILE(pop_at_return local_vars c1, pop_at_return local_vars c2) :: pop_at_return local_vars cmds
  | BRANCH (c1, c2) :: cmds ->
    BRANCH (pop_at_return local_vars c1, pop_at_return local_vars c2) :: pop_at_return local_vars cmds
  | c :: cmds -> c :: pop_at_return local_vars cmds

let rec apply_renaming (dict : (name * name) list) (cmds : cmd list) : cmd list =
  match cmds with
  | [] -> []
  | CALL f :: cmds ->
    (match List.assoc_opt f dict with
    | Some h -> CALL h
    | None   -> CALL f) :: apply_renaming dict cmds
  | WHILE (c1, c2) :: cmds ->
    WHILE(apply_renaming dict c1, apply_renaming dict c2) :: apply_renaming dict cmds
  | BRANCH (c1, c2) :: cmds ->
    BRANCH (apply_renaming dict c1, apply_renaming dict c2) :: apply_renaming dict cmds
  | c :: cmds -> c :: apply_renaming dict cmds

let get_fun_name (Func (fun_name, _, _, _, _)) : name = fun_name

let rec add_prefix_to_funs (prefix : name) (funs : func list) : (func list) =
  match funs with
  | [] -> []
  | Func (name, args, local_vars, local_funs, body) :: funs ->
    Func (prefix ^ name, args, local_vars, local_funs, body) :: add_prefix_to_funs prefix funs

let rec compile_funs (env : var option list) (dict : (name * name) list) (funs : func list) : (name * cmd list) list =
  match funs with
  | [] -> []
  | Func (name, args, local_vars, local_funs, body) :: funs ->
    let dict = (name, name) :: dict in
    let new_env = 
      [Some ("stack_pointer:" ^ name)] @ 
      (List.map Option.some local_vars) @ 
      [None] @ 
      (List.rev (List.map Option.some args)) @ 
      env
    in
    let new_dict = dict @ (List.map (fun f -> (get_fun_name f, name ^ "_" ^ get_fun_name f)) local_funs) in
    let local_funs = add_prefix_to_funs (name ^ "_") local_funs in
    let fun_code = compile_stmt new_env new_dict body in
    let fun_code = [ENTER (List.length local_vars); TOP; PUSH] @ fun_code @ [RET] in
    let fun_code = pop_at_return local_vars fun_code in
    let fun_code = apply_renaming new_dict fun_code in
    (name, fun_code) :: compile_funs new_env new_dict local_funs @ compile_funs env dict funs

let create_father_dict (father : name) (funs : func list) =
  let rec add_fathers father funs =
    List.fold_left (fun acc (Func (name, _, _, local_funs, _)) ->
      let local_funs = add_prefix_to_funs (name ^ "_") local_funs in
      (name, father) :: acc @ add_fathers name local_funs) [] funs
    in
  father_dict := add_fathers father funs

let compile_prog ((vars, funs, main_stmt) : prog) : vm_prog =
  let env = [Some "stack_pointer:main"] @ List.map Option.some vars in
  let dict = List.map (fun f -> (get_fun_name f, get_fun_name f)) funs in
  create_father_dict "main" funs;
  let compiled_funs = compile_funs env [] funs in
  let main_code = compile_stmt env dict main_stmt in
  ([ENTER (List.length vars); TOP; PUSH] @ main_code @ [LEAVE (List.length vars + 1)], compiled_funs)