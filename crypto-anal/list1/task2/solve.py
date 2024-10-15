import numpy as np
import heapq
from collections import defaultdict
from math import *

from transformers import GPT2LMHeadModel, GPT2Tokenizer
import torch

tokenizer = GPT2Tokenizer.from_pretrained('gpt2')
model = GPT2LMHeadModel.from_pretrained('gpt2')

def score_sentence(sequence):
    inputs = tokenizer(sequence, return_tensors="pt")
    with torch.no_grad():
        outputs = model(**inputs, labels=inputs["input_ids"])
        log_likelihood = outputs.loss.item() 
    return torch.exp(torch.tensor(-log_likelihood)).item()

## pl_50k.txt from https://github.com/hermitdave/FrequencyWords/blob/master/content/2018/pl/pl_50k.txt

alphabet = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"
alphasize = len(alphabet)
num2char = dict(enumerate(alphabet))
char2num = {num2char[n]: n for n in num2char}

print(len(alphabet))

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

class MarekLancuch:
	def __init__(self):
		self.model = defaultdict(lambda: defaultdict(float))

	def trenuj_bicek(self, text):
		for i in range(len(text) - 2):
			self.model[text[i] + text[i + 1]][text[i + 2]] += 1

	def fix(self):
		for c in alphabet:
			for d in alphabet:
				if c + d in self.model:
					total = sum(self.model[c + d].values())
					for e in alphabet:
						if e in self.model[c + d]:
							self.model[c + d][e] /= total
						else:
							self.model[c + d][e] = 0

	def p(self, last_two, next_char):
		if last_two in self.model:
			return self.model[last_two][next_char]
		return 0.0

marek = MarekLancuch()

with open('train.txt', 'r', encoding='utf-8') as file:
	text = []
	for line in file.read().splitlines():
		for c in line:
			c = c.upper()
			if c in alphabet:
				text.append(c)
	
	print("treining on:", len(text), "data set")
	print(text[:150])
	marek.trenuj_bicek(text)
	marek.fix()
	#exit(0)

#for c, p in marek.model['NI'].items():
#	print(c, p)
#exit(0)

AVG = 0
AVG_SUM = 0
WORD_CNT = 0
with open('pl_50k.txt', 'r', encoding='utf-8') as file:
	for line in file.read().splitlines():
		word, cnt = line.split()
		cnt = int(cnt)
		print("wrod:", word)
		word = word.upper()
	

		ok = True
		for c in word:
			if c not in alphabet:
				ok = False
				break

		if ok == False:
			continue
		
		if 'X' in word or 'Q' in word or 'v' in word:
			continue

		if len(word) <= 1 and word not in "AIOUWZ":
			print("word:", word)
			continue

		WORD_CNT += 1
		if WORD_CNT >= 1000:
			break

		AVG += len(word) * cnt
		AVG_SUM += cnt

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
		node.cnt = 0 #log(cnt) + node.len / 4

AVG /= AVG_SUM

print("AVG:", AVG)
# exit(0)

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

queue = []
counter = 0
dist = {}

def push_on_queue(value, item):
	global counter

	if item in dist and dist[item] < value - 2:
		return
	elif item not in dist or dist[item] > value:
		dist[item] = value

	heapq.heappush(queue, (value, counter, item))
	counter += 1

	# RAM safety
	if len(queue) > 5 * 10**6:
		heapq.heappop(queue)

push_on_queue(-1, (0, root, root, ''))

current_score = 0
current_key = []
current_i = -1
def jazda(i, u1, u2):
	global current_score
	global current_i

	if current_i != i:
		if u1.end and u2.end:
			push_on_queue(current_score, (i, root, root, ''.join(current_key)))

		if u1.end:
			push_on_queue(current_score, (i, root, u2, ''.join(current_key)))

		if u2.end:
			push_on_queue(current_score, (i, u1, root, ''.join(current_key)))

	if i == len(cipher1):
		return

	for c in alphabet:
		c1 = decodeChar(cipher1[i], c)
		c2 = decodeChar(cipher2[i], c)

		if c1 in u1.sons and c2 in u2.sons:
			old_score = current_score

			if i >= 2:
				p1 = marek.p(
					decodeChar(cipher1[i - 2], current_key[-2]) +
					decodeChar(cipher1[i - 1], current_key[-1]),
					c1);
				
				p2 = marek.p(
					decodeChar(cipher2[i - 2], current_key[-2]) +
					decodeChar(cipher2[i - 1], current_key[-1]),
					c2);

				#print(p1, p2, i, current_score)
				current_score -= p1 * p2
				current_score -= p1 * p1
				current_score -= p2 * p2
			else:
				current_score -= 1

			current_key.append(c)
			jazda(i + 1, u1.sons[c1], u2.sons[c2])
			current_key.pop()
			
			current_score = old_score

print("TRIE SIZE:", trie_size)

best = -1

debug = 0
while True:
	score, _, (i, u1, u2, maybe_key) = heapq.heappop(queue)

	debug += 1

	if debug % 500 == 0:
		print("score:", score)
		print(i, decode(cipher1[:len(maybe_key)], maybe_key), decode(cipher2[:len(maybe_key)], maybe_key))
		print(len(queue), len(dist))

	if i == len(cipher1):
		p1 = score_sentence(decode(cipher1, maybe_key))
		p2 = score_sentence(decode(cipher2, maybe_key))
		
		now = p1 * p2
		if best < now:
			best = now
			print("best_best:", best, p1, p2)
			print(maybe_key)
			print(decode(cipher1, maybe_key))
			print(decode(cipher2, maybe_key))
			print()
	
	current_score = score	
	current_key = [c for c in maybe_key]
	current_i = i
	jazda(i, u1, u2)
