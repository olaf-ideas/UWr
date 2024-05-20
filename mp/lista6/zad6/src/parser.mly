%{
open Ast
%}

%token <int> INT
%token TIMES
%token DIV
%token PLUS
%token MINUS
%token LPAREN
%token RPAREN
%token EQ
%token LL
%token LQ
%token GG
%token GQ
%token DIFF
%token TRUE
%token FALSE
%token IF
%token THEN
%token ELSE
%token EOF

%start <Ast.expr> prog

%nonassoc ELSE
%nonassoc EQ
%left PLUS MINUS
%left TIMES DIV

%%

prog:
  | e = expr; EOF { e }
  ;

expr:
  | i = INT { Int i }
  | e1 = expr; PLUS; e2 = expr { Binop(Add, e1, e2) }
  | e1 = expr; MINUS; e2 = expr { Binop(Sub, e1, e2) }
  | e1 = expr; DIV; e2 = expr { Binop(Div, e1, e2) }
  | e1 = expr; TIMES; e2 = expr { Binop(Mult, e1, e2) }
  | LPAREN; e = expr; RPAREN { e }
  | TRUE { Bool true }
  | FALSE { Bool false }
  | e1 = expr; EQ; e2 = expr { Binop(Eq, e1, e2) }
  | e1 = expr; LL; e2 = expr { Binop(L, e1, e2) }
  | e1 = expr; LQ; e2 = expr { Binop(Lq, e1, e2) }
  | e1 = expr; GG; e2 = expr { Binop(G, e1, e2) }
  | e1 = expr; GQ; e2 = expr { Binop(Gq, e1, e2) }
  | e1 = expr; DIFF; e2 = expr { Binop(Diff, e1, e2) }
  | IF; e1 = expr; THEN; e2 = expr; ELSE; e3 = expr { If(e1, e2, e3) }
  ;
