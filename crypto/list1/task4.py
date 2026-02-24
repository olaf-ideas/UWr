
def euclid(a, b):
  if b == 0:
    return a, 1, 0
  d, y, x = euclid(b, a % b)
  return d, x, y - a // b * x

def inverse(v, mod):
  g, _, inv = euclid(mod, v)
  if g != 1:
    print(f"trying to find inverse of: {v} mod: {mod}")
  assert g == 1
  return inv % mod

def crt(a, m):
  r, M = a[0], m[0]
  for a_i, mod in zip(a[1:], m[1:]):
    g, x, y = euclid(M, mod)
   
    print(">", r, M, a_i, mod) 
    assert (a_i - r) % g == 0

    x *= (a_i - r)
    y *= (a_i - r)

    r += M * x
    M *= mod
    r %= M

  for a_i, mod in zip(a, m):
    assert r % mod == a_i
  return r

def solve_matrix_mod(A, b, mod):
  n, m, rank = len(A), len(A[0]), 0
  col = list(range(m))

  for i in range(n):
    for j in range(m):
      A[i][j] %= mod

  for i in range(len(b)):
    b[i] %= mod

  for i in range(n):
    v, br, bc = 0, 0, 0
    for r in range(i, n):
      for c in range(i, m):
        if A[r][c] > 0:
          v, br, bc = A[r][c], r, c 

    #print("i:", i, "A:")
    #for r in range(n):
    #  for c in range(m):
    #    print(A[r][c], end=' ')
    #  print()
    #print()

    if v == 0:
      for j in range(i, n):
        if b[j] > 0:
          return None
      break

    A[i], A[br] = A[br], A[i]
    b[i], b[br] = b[br], b[i]
    col[i], col[bc] = col[bc], col[i]
    for j in range(0, n):
      A[j][i], A[j][bc] = A[j][bc], A[j][i]

    bv = inverse(A[i][i], mod)
    #print("? ", A[i][i], bv, A[i][i] * bv % mod)
    assert A[i][i] * bv % mod == 1

    for j in range(i + 1, n):
      fac = A[j][i] * bv
      b[j] = (b[j] - fac * b[i]) % mod
      for k in range(i + 1, m):
        A[j][k] = (A[j][k] - fac * A[i][k]) % mod
  
    rank += 1

  x = [0] * m
  for i in range(rank - 1, -1, -1):
    b[i] = b[i] * inverse(A[i][i], mod) % mod
    x[col[i]] = b[i]
    for j in range(0, i):
      b[j] = (b[j] - A[j][i] * b[i]) % mod
  return x

def mul(A, v, mod):
  #print("?", A, v)
  w = [0 for i in range(len(A))]
  for i in range(len(A)):
    for j in range(len(v)):
      w[i] = (w[i] + A[i][j] * v[j]) % mod
  return w

t = 'BREATHTAKING'
c = 'RUPOTENTOIFV'

assert len(t) == len(c)

t_vals = [ord(x) - ord('A') for x in t]
c_vals = [ord(x) - ord('A') for x in c]

print(t_vals)
print(c_vals)

for m in [2,3]: #range(1, len(t)):
  if m * m > (len(t) // m) * m or len(t) % m != 0:
    continue
  
  A = []
  b = []
  for i in range(0, m*m, m):
    for j in range(m):
      v = [0 for _ in range(m*m)]
      for k in range(m):
        v[j * m + k] = t_vals[i + k]
      A.append(v)
      b.append(c_vals[i + j])

  print(f"m = {m}")
  print(A)
  print(b)

  try:
    M_mod2 = solve_matrix_mod([row[:] for row in A], b[:], 2)
    M_mod13 = solve_matrix_mod([row[:] for row in A], b[:], 13)

    if M_mod2 != None and M_mod13 != None:
      print("Solution found: ", m)
      print(">>>", M_mod2, M_mod13)
      
      B = [[0 for i in range(m)] for j in range(m)]
      #B = [[3,4,6],[21,15,14],[20,23,5]]

      for i in range(m):
        for j in range(m):
          a = [M_mod2[i*m+j], M_mod13[i*m+j]]
      
          B[i][j] = crt(a, [2, 13])  
    
      print(f"B: {B}")
      print(t_vals[:m]) 
      print(mul(B, t_vals[:m], 26), c_vals[:m])
      exit(0)
    else:
      print(f"Failed for: {m}")
  except AssertionError:
    print(f"Inverse error")
    continue
