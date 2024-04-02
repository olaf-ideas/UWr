(* 
    Olaf Surgut, Version 1.5.0 
*)

let ( let* ) xs ys = List.concat_map ys xs

let create_list n v = List.init n (fun _ -> v)

let build_row ps n =
    let rec help ps n =
        match ps, n with
        | _,  n when n < 0 ->
            []
        | [], n -> 
            [create_list n false]
        | p :: ps', _ ->
            let take = help ps' (n - 1 - p) in
            let dont = help ps  (n - 1) in
            (List.map (fun xs -> false :: ((create_list p true) @ xs)) take) @
            (List.map (fun xs -> false :: xs) dont)
    in
    List.map (
        fun xs -> List.tl xs
    ) (help ps (n + 1))

let rec build_candidate pss n =
    match pss with
    | [] ->
        failwith "pss cannot be empty"
    | ps :: [] ->
        List.map (fun x -> [x]) (build_row ps n)
    | ps :: pss' ->
        let* next_row  = build_row ps n in
        let* cand_rows = build_candidate pss' n in
            [next_row :: cand_rows]

let verify_row ps xs = 
    let rec help xs count =
        match xs with
        | [] -> 
            if count = 0 then [] else [count]
        | false :: xs' -> 
            if count = 0 then help xs' 0 else count :: help xs' 0
        | true :: xs' -> 
            help xs' (count + 1)
    in
    ps = (help xs 0)
    
let rec verify_rows pss xss =
    match pss, xss with
    | [], _ | _, [] -> 
        true
    | ps :: pss', xs :: xss' ->
        (verify_row ps xs) && (verify_rows pss' xss')
            
let transpose xss = 
    let rec help xss res = 
        match xss with
        | [] -> 
            List.rev res
        | [] :: xss' -> 
            help [] res
        | (x :: xs) :: xss' -> 
            help (List.map List.tl xss) ((x :: (List.map List.hd xss')) :: res)
    in
    help xss []

type nonogram_spec = {rows: int list list; cols: int list list}

let solve_nonogram nono =
    build_candidate (nono.rows) (List.length (nono.cols))
    |> List.filter (fun xss -> transpose xss |> verify_rows nono.cols)
