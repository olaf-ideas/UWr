def B(i,j):
    return 'B_%d_%d' % (i,j)

def domains(variables):
	return [v + ' in 0..1' for v in variables]

def get_col(j, n, m):
	return [B(i, j) for i in range(n)]

def get_row(i, n, m):
	return [B(i, j) for j in range(m)]

def get_3x1(i, j):
	return [B(i + x, j) for x in range(3)]

def get_1x3(i, j):
	return [B(i, j + y) for y in range(3)]

def get_2x2(i, j):
	return [B(i + x, j + y) for x in range(2) for y in range(2)]

def get_all_1x3(n, m):
	return [get_1x3(i, j) for i in range(n) for j in range(m - 2)]

def get_all_3x1(n, m):
	return [get_3x1(i, j) for i in range(n - 2) for j in range(m)]

def get_all_2x2(n, m):
	return [get_2x2(i, j) for i in range(n - 1) for j in range(m - 1)]

def correct_3x1(A, B, C):
	return [f'{A} + 2 * {B} + 4 * {C} #\= 2']

def correct_2x2(A, B, C, D):
	return [f'{A} + 2 * {B} + 4 * {C} + 8 * {D} #\= {x}' for x in [6, 7, 9, 11, 13, 14]]

def satisfy_rows(rows, n, m):
	return [' + '.join(get_row(i, n, m)) + f' #= {r}' for i, r in enumerate(rows)]

def satisfy_cols(cols, n, m):
	return [' + '.join(get_col(j, n, m)) + f' #= {c}' for j, c in enumerate(cols)]

def satisfy_rectangles(n, m):
	all_1x3 = get_all_1x3(n, m)
	all_3x1 = get_all_3x1(n, m)
	all_2x2 = get_all_2x2(n, m)

	res = []
	res += [correct_3x1(*b) for b in all_1x3]
	res += [correct_3x1(*b) for b in all_3x1]
	res += [correct_2x2(*b) for b in all_2x2]

	return [y for x in res for y in x]	

def set_filled(filled):
	return [f'{B(i, j)} #= {v}' for i, j, v in filled]

def storms(rows, cols, triples):
	n = len(rows)
	m = len(cols)

	bs = [B(i,j) for i in range(n) for j in range(m)]

	constraints = []
	constraints += domains(bs)
	constraints += set_filled(triples)
	constraints += satisfy_rows(rows, n, m)
	constraints += satisfy_cols(cols, n, m)
	constraints += satisfy_rectangles(n, m)
	
	writeln(':- use_module(library(clpfd)).')
	writeln('solve([' + ', '.join(bs) + ']) :- ')

	for c in constraints:
		writeln(c)
		writeln(',')
	
	writeln('    labeling([ff], [' +  ', '.join(bs) + ']).' )
	writeln('')
	writeln(":- tell('prolog_result.txt'), solve(X), write(X), nl, told.")

def writeln(s):
	output.write(s + '\n')

txt = open('zad_input.txt').readlines()
output = open('zad_output.txt', 'w')

rows = [int(r) for r in txt[0].split()]
cols = [int(c) for c in txt[1].split()]
triples = []

for i in range(2, len(txt)):
	if txt[i].strip():
		triples.append(tuple(map(int, txt[i].split())))

storms(rows, cols, triples)
