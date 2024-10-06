import numpy as np

## pl_50k.txt from https://github.com/hermitdave/FrequencyWords/blob/master/content/2018/pl/pl_50k.txt

alphabet = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"
alphasize = len(alphabet)
num2char = dict(enumerate(alphabet))
char2num = {num2char[n]: n for n in num2char}

class Node:
	def __init__(self, par_len):
		self.sons = {}
		self.end = False
		self.len = par_len + 1

root = Node(-1)

with open('pl_50k.txt', 'r', encoding='utf-8') as file:
	for line in file.read().splitlines():
		word, cnt = line.split()
		word = word.upper()

		ok = True
		for c in word:
			if c not in alphabet:
				ok = False
				break

		if ok == False:
			continue

		if len(word) <= 1:
			print("word:", word)

		node = root
		for ch in word:
			ch = char2num[ch]
			if ch not in node.sons:
				node.sons[ch] = Node(node.len)
			node = node.sons[ch]
		node.end = True

cipher1 = "ĘĘKGCATDUJXNYYVXWÓWUĄVŹGJĘCŃŁQŃJAŁGEJWXNYĆŃKXŁMJMOWŃAVVCUBWLŃARJHÓŁVBIONKFQJMEĄŃŹJAEHĆQEŁRIŁŚŹFMŹNŚŁFAĘXTOPŚŹUĆŃPBUŚAGWYZGHUHRWUGKFŚKĄYV"
cipher2 = "ŁUXBUQLĄNUAÓĘCŃŁĆŚDNŁVSSGĘGKĆPLIRDWVÓDŃŚŁÓĘŃNWWOŚZFĆŚSŹBŻUSIKUĆHHUĆUĘŁVILCWRŹBAYMYBKŹŁFRKŃŃCIŹUŻDSTHEOOLLOÓIŁZVPJŁŻWGDŃĆFŹFQKOAĄSZAVĆŻŁŚJADLCŹDWEŁHBĆQZXPĘINLĘYXYAŻTLQMNĄĆŻWU"

A = np.array(list(map(char2num.get, cipher1)))
B = np.array(list(map(char2num.get, cipher2)))

while len(A) < len(B):
	B = np.delete(B, -1)

now = list()
K = list()
best = -1
score = 0
ans = list()

def jazda(i, u, v):
	global score, best

	if i == len(A):
		if best < score:
			best = score
			ans = K
			print("score: ", score)
			print(*now)
		return

	for char in range(alphasize):
		aa = (A[i] - char) % alphasize
		bb = (B[i] - char) % alphasize
		if aa in u.sons and bb in v.sons:
			K.append(char)
			now.append(num2char[(A[i] - char) % alphasize])

			uu = u.sons[aa]
			vv = v.sons[bb]
			score += uu.len ** 2
			score += vv.len ** 2

			jazda(i + 1, uu, vv)

			score -= uu.len ** 2
			score -= vv.len ** 2

			K.pop()
			now.pop()

	for char in range(alphasize):
		aa = (A[i] - char) % alphasize
		bb = (B[i] - char) % alphasize
		if aa in u.sons and bb in v.sons:
			K.append(char)
			now.append(num2char[(A[i] - char) % alphasize])

			uu = u.sons[aa]
			vv = v.sons[bb]
			score += uu.len ** 2
			score += vv.len ** 2

			if uu.end:
				jazda(i + 1, root, vv)
			
			if vv.end:
				jazda(i + 1, uu, vv)

			if uu.end and vv.end:
				jazda(i + 1, root, root)
			
			score -= uu.len ** 2
			score -= vv.len ** 2

			K.pop()
			now.pop()

jazda(0, root, root)
