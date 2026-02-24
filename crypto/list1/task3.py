
answer = 0
for a in range(26):
  for b in range(26):
    for c in range(26):
      for d in range(26):
        if a * d - b * c != 0 and \
          a * a + b * c == 1 and\
          a * b + b * d == 0 and\
          a * c + c * d == 0 and\
          b * c + d * d == 1:
          print(a, b, c, d)
          answer += 1
print(answer)