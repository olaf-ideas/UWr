let rec get_funs_names funs dict fname=
   match funs with
   | [] -> dict
   | Func(name,_,_,_,_) :: funs -> get_funs_names funs (M.add name (fname^"_"^name) dict) fname

let rec rename_body body dict = 
  match body with
  |[] -> []
  |CALL name :: body -> (match M.find_opt name dict with 
                        |Some name -> CALL (name) :: rename_body body dict
                        | None -> CALL name :: rename_body body dict  )
  |WHILE (x1,x2) :: body-> WHILE(rename_body x1 dict,rename_body x2 dict) :: rename_body body dict
  |BRANCH (x1,x2) :: body-> BRANCH(rename_body x1 dict,rename_body x2 dict) :: rename_body body dict
  |x  :: body -> x :: rename_body body dict

let rec rename_funs inflist fname = 
  match inflist with
  |[] -> []
  |Func(name,v1,v2,v3,v4)  :: inflist -> Func(fname ^ "_" ^ name,v1,v2,v3,v4) :: rename_funs inflist fname

let rec compile_funs (flist:func list) stack dict : (name * cmd list) list=
  match flist with
  | [] -> []
  | Func(name, args, vlist,inflist, body) :: flist -> 
    let stack = declare_vars vlist @ None :: (List.rev (declare_vars args)) @  stack in
    let body = [ENTER (List.length vlist)] @ (compile_stmt body stack ) @ [LEAVE (List.length vlist)] @ [RET] in
    let newdict = (get_funs_names inflist dict name) in
    let body = rename_body body newdict in 
    let inflist =  rename_funs inflist name in
    (* print_stack stack;print_endline (""); *)
    (name , body) :: compile_funs inflist stack newdict @ compile_funs flist stack dict

let compile_prog (prog : prog) : vm_prog = 
  match prog with
  | vlist, funs , prog -> 
    let stack = declare_vars vlist  in
    [ENTER (List.length vlist)] @ (compile_stmt prog stack),compile_funs funs stack M.empty