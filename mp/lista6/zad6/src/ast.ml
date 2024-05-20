(* abstract syntax tree *)

type bop = Mult | Div | Add | Sub | Eq | L | Lq | G | Gq | Diff

type expr =
  | Int of int
  | Bool of bool
  | Binop of bop * expr * expr
  | If of expr * expr * expr
                               
