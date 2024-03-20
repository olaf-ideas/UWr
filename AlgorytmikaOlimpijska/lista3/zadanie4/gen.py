from random import *

n = 500000
print(n)

p = [i + 1 for i in range(n)]
p = reversed(p)
#shuffle(p)

print(*p)
