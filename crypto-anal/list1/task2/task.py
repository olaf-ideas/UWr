import numpy as np
import heapq
from math import *

## pl_50k.txt from https://github.com/hermitdave/FrequencyWords/blob/master/content/2018/pl/pl_50k.txt

alphabet = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"
alphasize = len(alphabet)
num2char = dict(enumerate(alphabet))
char2num = {num2char[n]: n for n in num2char}

def encodeChar(c, k):
	return num2char[(char2num[c] + char2num[k]) % alphasize]

def encode(plaintext, key):
	return ''.join(map(encodeChar, plaintext, key))

def decodeChar(c, k):
	return num2char[(char2num[c] - char2num[k]) % alphasize]

def decode(plaintext, key):
	return ''.join(map(decodeChar, plaintext, key))

class Node:
	def __init__(self):
		self.sons = {}
		self.end = False
		self.len = 0
		self.cnt = 0

root = Node()
trie_size = 1

freq = {c: 0 for c in alphabet}
di_freq = {c + d: 0 for c in alphabet for d in alphabet}

with open('pl_50k.txt', 'r', encoding='utf-8') as file:
	for line in file.read().splitlines():
		word, cnt = line.split()
		cnt = int(cnt)
		word = word.upper()
	
		ok = True
		for c in word:
			if c not in alphabet:
				ok = False
				break

		if ok == False:
			continue

		if len(word) <= 1 and word not in "AIOUWZ":
			print("word:", word)
			continue

		for c in word:
			freq[c] += cnt

		for i in range(len(word) - 1):
			di_freq[word[i] + word[i + 1]] += cnt

		node = root
		for ch in word:
			if ch not in node.sons:
				trie_size += 1
				node.sons[ch] = Node()
				node.sons[ch].len = node.len + 1
			node = node.sons[ch]

		assert node.end == False
		node.end = True
		node.cnt = log(cnt) + node.len / 4 - 1

sums = sum(freq.values())
freq = {c: v / sums for c, v in freq.items()}

for c in alphabet:
	s = 0
	for d in alphabet:
		s += di_freq[c + d]
	s = max(s, 1)

	for d in alphabet:
		di_freq[c + d] /= s

#for c in "AIOUWZ":
#	if c not in root.sons:
#		print("?", c)
#	assert c in root.sons
#
#	root.sons[c].end = True

root.end = False

cipher1 = "ĘĘKGCATDUJXNYYVXWÓWUĄVŹGJĘCŃŁQŃJAŁGEJWXNYĆŃKXŁMJMOWŃAVVCUBWLŃARJHÓŁVBIONKFQJMEĄŃŹJAEHĆQEŁRIŁŚŹFMŹNŚŁFAĘXTOPŚŹUĆŃPBUŚAGWYZGHUHRWUGKFŚKĄYV"
cipher2 = "ŁUXBUQLĄNUAÓĘCŃŁĆŚDNŁVSSGĘGKĆPLIRDWVÓDŃŚŁÓĘŃNWWOŚZFĆŚSŹBŻUSIKUĆHHUĆUĘŁVILCWRŹBAYMYBKŹŁFRKŃŃCIŹUŻDSTHEOOLLOÓIŁZVPJŁŻWGDŃĆFŹFQKOAĄSZAVĆŻŁŚJADLCŹDWEŁHBĆQZXPĘINLĘYXYAŻTLQMNĄĆŻWU"

A = np.array(list(map(char2num.get, cipher1)))
B = np.array(list(map(char2num.get, cipher2)))

while len(A) < len(B):
	B = np.delete(B, -1)

class Krowa:
	def __init__(self, max_size=10000):
		self.max_size = max_size
		self.queue = []
		self.counter = 0
		self.done = set()

	def push(self, value, item):
		if item in self.done:
			return
		self.done.add(item)

		heapq.heappush(self.queue, (value, self.counter, item))
		self.counter += 1

		if len(self.queue) > self.max_size:
			heapq.heappop(self.queue)

starts = [Krowa() for i in range(len(A))]

starts[0].push(0, (root, root, ''))

current_score = 0
current_key = []
def jazda(i, u1, u2):
	global current_score

	# print(i, ''.join(current_key), current_score)

	if u1.end and u2.end:
		assert u1.cnt > 0
		assert u2.cnt > 0
		starts[i].push(current_score, (root, root, ''.join(current_key)))

	if u1.end:
		assert u1.cnt > 0
		starts[i].push(current_score, (root, u2, ''.join(current_key)))

	if u2.end:
		assert u2.cnt > 0
		starts[i].push(current_score, (u1, root, ''.join(current_key)))

	for c in alphabet:
		c1 = decodeChar(cipher1[i], c)
		c2 = decodeChar(cipher2[i], c)

		if c1 in u1.sons and c2 in u2.sons:
			if i > 0:
				current_score -= di_freq[decodeChar(cipher1[i - 1], current_key[-1]) + c1]
				current_score -= di_freq[decodeChar(cipher2[i - 1], current_key[-1]) + c2]
			
			current_score -= freq[c1]
			current_score -= freq[c2]

			current_key.append(c)
			jazda(i + 1, u1.sons[c1], u2.sons[c2])
			current_key.pop()

			if i > 0:
				current_score += di_freq[decodeChar(cipher1[i - 1], current_key[-1]) + c1]
				current_score += di_freq[decodeChar(cipher2[i - 1], current_key[-1]) + c2]
			
			current_score += freq[c1]
			current_score += freq[c2]

print("TRIE SIZE:", trie_size)

for i in range(len(starts)):	

	candidates = starts[i].queue

	print("testing", i, "candidates:", len(candidates))
	key_str = candidates[0][2][2]
	for i in range(min(5, len(candidates))):
		print("top", i + 1, " = ", candidates[i][0], candidates[i][2][2])
	print(decode(cipher1[:len(key_str)], key_str))
	print(decode(cipher2[:len(key_str)], key_str))
	print("key:", key_str)

	for score, _, (u1, u2, maybe_key) in candidates:
		current_key = [c for c in maybe_key]
		currnet_score = score
		jazda(i, u1, u2)
