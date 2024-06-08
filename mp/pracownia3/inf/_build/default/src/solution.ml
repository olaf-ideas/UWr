open Ast

let rec print_env env =
  match env with
  | e :: env ->
    (match e with
    | Some x -> Printf.printf "%s " x
    | None -> Printf.printf "None ");
    print_env env
  | [] -> Printf.printf ";\n"

(* let rec lookup_for_variable (env : var option list) (x : var) (index : int) : (int * cmd list) =
  match env with
  | [] -> failwith ("Variable " ^ x ^ " not found")
  | Some y :: _ when y = x -> [LOAD index]
  | Some "env_pointer" :: env -> LOAD index :: lookup_for_variable env x 0
  | _ :: env -> lookup_for_variable env x (index + 1) *)

let rec lookup_for_variable_addr (env : var option list) (x : var) (index : int) : (int * cmd list) =
  match env with
  | [] -> failwith ("Variable " ^ x ^ " not found")
  | Some y :: _ when y = x -> (index, [])
  | Some y :: env when (String.starts_with ~prefix:"env_pointer" y) -> 
    let (idx, cmds) = lookup_for_variable_addr env x 0 in
    (idx, LOAD index :: cmds)
  | _ :: env -> lookup_for_variable_addr env x (index + 1)

let rec lookup_for_function_env (env : var option list) (father_name : name) (index : int) : cmd list =
  Printf.printf "Father name: %s Current env: " father_name; print_env env;
  match env with
  | [] -> failwith ("Can't find function with father_name: " ^ father_name)
  | Some y :: _ when y = ("env_pointer:" ^ father_name) -> Printf.printf "Found father_name\n"; [LOAD index]
  | Some y :: env when (String.starts_with ~prefix:"env_pointer" y) ->
    Printf.printf "Found other env_pointer: %s\n" y;
    LOAD index :: lookup_for_function_env env father_name 0
  | _ :: env -> lookup_for_function_env env father_name (index + 1)

let rec compile_aexp (env : var option list) (father_dict : (name * name) list) (a : aexp) : cmd list =
  match a with
  | Int n -> [CONST n]
  | Var x ->
    (* Printf.printf "Looking up for variable %s in: " x; print_env env; *)
    let (idx, cmds) = lookup_for_variable_addr env x 0 in
    [TOP] @ cmds @ [LOAD idx]
  (* | Var x -> [TOP] @ lookup_for_variable env x 0 *)
  | Binop (op, a1, a2) ->
      let c1 = compile_aexp env father_dict a1 in
      let c2 = compile_aexp (None :: env) father_dict a2 in
      c1 @ [PUSH] @ c2 @ [PRIM op]
  | Call (f, args) ->
    let rec prepare_args env args =
      match args with
      | [] -> []
      | a :: args -> compile_aexp env father_dict a @ [PUSH] @ prepare_args (None :: env) args in
    Printf.printf "Is there %s in father_dict?\n" f;
    let cmds = lookup_for_function_env env (List.assoc f father_dict) 0 in
    [TOP] @ cmds @ [PUSH] @ (prepare_args env args) @ [CALL f] @ [LEAVE (List.length args + 1)]

let rec compile_bexp (env : var option list) (father_dict : (name * name) list) (b : bexp) : cmd list =
  match b with
  | Bool true  -> [CONST 1]
  | Bool false -> [CONST 0]
  | Cmp (op, a1, a2) ->
    let c1 = compile_aexp env father_dict a1 in
    let c2 = compile_aexp (None :: env) father_dict a2 in
    c1 @ [PUSH] @ c2 @ [CMP op]
    (* compile_aexp env a1 @ [PUSH] @ compile_aexp (None :: env) a2 @ [CMP op] *)
  | And (b1, b2) ->
    let c1 = compile_bexp env father_dict b1 in
    let c2 = compile_bexp env father_dict b2 in
    c1 @ [BRANCH (c2, [CONST 0])]
  | Or (b1, b2) ->
    let c1 = compile_bexp env father_dict b1 in
    let c2 = compile_bexp env father_dict b2 in
    c1 @ [BRANCH ([CONST 1], c2)]
  | Not b ->
    compile_bexp env father_dict b @ [PUSH; CONST 0; CMP Eq]

let rec compile_stmt (env : var option list) (father_dict : (name * name) list) (s : stmt) : cmd list =
  match s with 
  | Block stmts -> List.concat (List.map (compile_stmt env father_dict) stmts)
  | Assgn (x, a) ->
    (* Printf.printf "Assign up for variable %s in: " x; print_env env; *)
    let ca = compile_aexp (None :: env) father_dict a in
    let (idx, cmds) = lookup_for_variable_addr env x 0 in
    [TOP] @ cmds @ [PUSH] @ ca @ [STORE idx]
    (* let ca = compile_aexp (None :: env) a in
    (match List.find_index (fun y -> y = Some x) env with
    | Some i -> [TOP; PUSH] @ ca @ [STORE i]
    | None   -> failwith ("Variable " ^ x ^ " not found")) *)
  | If (b, s1, s2) ->
    let cb = compile_bexp env father_dict b in
    let cs1 = compile_stmt env father_dict s1 in
    let cs2 = compile_stmt env father_dict s2 in
    cb @ [BRANCH (cs1, cs2)]
  | While (b, s) ->
    let cb = compile_bexp env father_dict b in
    let cs = compile_stmt env father_dict s in
    [WHILE (cb, cs)]
  | Read x ->
    let (idx, cmds) = lookup_for_variable_addr env x 0 in
    [TOP] @ cmds @ [PUSH; READ; STORE idx]
    (* (match List.find_index (fun y -> y = Some x) env with
    | Some i -> [TOP; PUSH; READ; STORE i]
    | None   -> failwith ("Variable" ^ x ^ "not found")) *)
  | Write a ->
    compile_aexp env father_dict a @ [WRITE]
  | Return a ->
    compile_aexp env father_dict a @ [RET]

let rec add_leave_at_return (local_vars : var list) (cmds : cmd list) : cmd list =
  match cmds with
  | [] -> []
  | RET :: cmds -> [LEAVE (List.length local_vars + 1); RET] @ add_leave_at_return local_vars cmds
  | WHILE (c1, c2) :: cmds ->
    WHILE(add_leave_at_return local_vars c1, add_leave_at_return local_vars c2) :: add_leave_at_return local_vars cmds
  | BRANCH (c1, c2) :: cmds ->
    BRANCH (add_leave_at_return local_vars c1, add_leave_at_return local_vars c2) :: add_leave_at_return local_vars cmds
  | c :: cmds -> c :: add_leave_at_return local_vars cmds

(* let rec print_dict (dict : (name * name) list) =
  match dict with
  | (a, b) :: dict -> Printf.printf "%s %s\n" a b; print_dict dict
  | _ -> ()

let rec print_local_funs local_funs =
  match local_funs with
  | Func (name, _, _, _, _) :: local_funs -> 
    Printf.printf "%s " name; print_local_funs local_funs
  | _ -> Printf.printf "\n" *)

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

let rec add_prefix_to_funs (prefix : name) (funs : func list) : (func list) =
  match funs with
  | [] -> []
  | Func (name, args, local_vars, local_funs, body) :: funs ->
    Func (prefix ^ name, args, local_vars, local_funs, body) :: add_prefix_to_funs prefix funs

let rec compile_funs (env : var option list) (father_dict : (name * name) list) (dict : (name * name) list) (funs : func list) : (name * cmd list) list =
  match funs with
  | [] -> []
  | Func (name, args, local_vars, local_funs, body) :: funs ->
    let new_env = [Some ("env_pointer:" ^ name)] @ (List.map Option.some local_vars) @ [None] @ (List.rev (List.map Option.some args)) @ env in
    let new_dict = dict @ (List.map (fun (Func (fun_name, _, _, _, _)) -> (fun_name, name ^ "_" ^ fun_name)) local_funs) in
    let local_funs = add_prefix_to_funs (name ^ "_") local_funs in
    let fun_code = compile_stmt new_env father_dict body in
    let fun_code = [ENTER (List.length local_vars); TOP; PUSH] @ fun_code @ [RET] in
    let fun_code = add_leave_at_return local_vars fun_code in
    let fun_code = apply_renaming new_dict fun_code in
    (name, fun_code) :: compile_funs new_env father_dict new_dict local_funs @ compile_funs env father_dict dict funs

let rec create_father_dict (father : name) (funs : func list) : ((name * name) list) =
  match funs with
  | [] -> []
  | Func (name, _, _, local_funs, _) :: funs ->
    let local_funs = add_prefix_to_funs (name ^ "_") local_funs in
    Printf.printf "adding to father_dict: %s -> %s\n" name father; 
    (name, father) :: create_father_dict name local_funs @ create_father_dict father funs

let compile_prog ((vars, funs, main_stmt) : prog) : vm_prog =
  let env = [Some "env_pointer:main"] @ List.map Option.some vars in
  let father_dict = create_father_dict "main" funs in
  let compiled_funs = compile_funs env father_dict [] funs in
  let main_code = compile_stmt env father_dict main_stmt in
  ([ENTER (List.length vars); TOP; PUSH] @ main_code @ [LEAVE (List.length vars + 1)], compiled_funs)
