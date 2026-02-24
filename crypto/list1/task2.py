
answer = 0
for a in range(26):
  for b in range(26):
    for c in range(26):
      for d in range(26):
        if a * d - b * c != 0:
          answer += 1
print(answer)