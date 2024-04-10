
(* The type of tokens. *)

type token = 
  | TRUE
  | TIMES
  | THEN
  | RPAREN
  | PLUS
  | MINUS
  | LQ
  | LPAREN
  | LL
  | INT of (int)
  | IF
  | GQ
  | GG
  | FALSE
  | EQ
  | EOF
  | ELSE
  | DIV
  | DIFF

(* This exception is raised by the monolithic API functions. *)

exception Error

(* The monolithic API. *)

val prog: (Lexing.lexbuf -> token) -> Lexing.lexbuf -> (Ast.expr)
