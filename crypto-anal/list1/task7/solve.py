import sys
from math import *

sys.setrecursionlimit(1300)

def encode(plaintext, key):
	alphabet = list(key)
	alphabet.sort()
	encdic = dict(zip(alphabet, key))
	return ''.join(map(lambda c: encdic[c], plaintext))

def decode(plaintext, key):
	alphabet = list(key)
	alphabet.sort()
	encdic = dict(zip(alphabet, key))
	decdic = { encdic[c]: c for c in alphabet }
	return ''.join(map(lambda c: decdic[c], plaintext))

alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

cipher = 'KMFXLZMKDZVOXLZPXKYIPBKRRZFSVYDSVVFKMFXLZXLIYFAKYXVOXLZSKXZYPQZBKDZSVYDSVVFKMFDKMCDZMFIZFVOXLZSKXZYPQZBKNPZXLZCSZYZDKFZQIXXZY'

# https://gist.github.com/pozhidaevak/0dca594d6f0de367f232909fe21cdb2f
freq = {'E': 12.0036010803241, 'T': 9.102730819245775, 'A': 8.122436731019306, 'O': 7.682304691407423, 'I': 7.3121936580974305, 'N': 6.952085625687707, 'S': 6.281884565369612, 'R': 6.02180654196259, 'H': 5.921776532959889, 'D': 4.321296388916676, 'L': 3.981194358307493, 'U': 2.8808642592777836, 'C': 2.7108132439731922, 'M': 2.6107832349704916, 'F': 2.300690207062119, 'Y': 2.1106331899569875, 'W': 2.0906271881564473, 'G': 2.0306091827548265, 'P': 1.8205461638491551, 'B': 1.4904471341402423, 'V': 1.1103330999299794, 'K': 0.6902070621186357, 'X': 0.1700510153045914, 'Q': 0.11003300990297091, 'J': 0.10003000900270083, 'Z': 0.07002100630189059}
freq = {k: v / sum(freq.values()) for k, v in freq.items()}

cnt = {c: 0 for c in alphabet}
for c in cipher:
	cnt[c] += 1

for a, b in cnt.items():
	print(a, b / len(cipher), freq[a])

# exit(0)

def frozendict(d: dict):
    keys = sorted(d.keys())
    return tuple((k, d[k]) for k in keys)

class TrieNode:
	def __init__(self):
		self.sons = {}
		self.end = False

root = TrieNode()

di_freq = {a + b: 0 for a in alphabet for b in alphabet}

with open('popular.txt', 'r') as words:
	for word in words:
		word = word.upper()[:-1]

		ok = True
		for c in word:
			if c not in alphabet:
				ok = False
				break
  
		if ok:
			for i in range(len(word) - 1):
				di_freq[word[i] + word[i + 1]] += 1
      
			node = root

			for c in word:
				if c not in node.sons:
					node.sons[c] = TrieNode()
				node = node.sons[c]

			node.end = True

for c in alphabet:
	xx = 0
	for d in alphabet:
		xx += di_freq[c + d]

	xx = max(xx, 1)
	# print(c, end=' ')
	for d in alphabet:
		di_freq[c + d] /= xx
		# print(di_freq[c + d], end=' ')
	# print()

root.end = False
for c in alphabet:
	root.sons[c].end = False

for c in "AIO":
    root.sons[c].end = True

print("DICT LOADED")

decdic = {}

GAMMA = 0.05

word_end = [0 for i in range(len(cipher))]
words = 0

best_score = 0
answer = ''
answer_dic = {}

possible_chars = []

visited = set()

done_positions = set()

def hash_dic(dic):
	h = 0
	for c in alphabet:
		h *= len(alphabet)
		if c in dic:
			h += ord(dic[c])
	return h

def jazda(i, u):
	global best_score
	global words
 
	if i > 10 and i / words < 3:
		return False

	# if (i, u, hash_dic(decdic)) in done_positions:
	# 	return False

	# done_positions.add((i, u, hash_dic(decdic)))

	# if (i, frozendict(decdic)) in node.bad:
	# 	return False

	# print(i / max(1, words), words, i)
	# for j in range(i):
	# 	print(decdic[cipher[j]], end='')
	# print()
	# for j in range(i):
	# 	print(word_end[j], end='')
	# print()

	if i == len(cipher):
		score = 0

		plaintext = ''
		for j in range(len(cipher)):
			plaintext += decdic[cipher[j]]
			if word_end[j] == 1:
				plaintext += ' '

		for j in range(len(cipher) - 1):
			score += di_freq[decdic[cipher[j]] + decdic[cipher[j + 1]]]

		if plaintext not in visited:
			# print(cipher)
			visited.add(plaintext)
			
			if best_score < score:
				best_score = score
				print(plaintext, words, i / words, score)
				print(decdic)
	
		# if best_score < score:
		# 	best_score = score
		# 	answer_dic = decdic
   
		# 	key_inv = list()
		# 	for c in alphabet:
		# 		if c in decdic:
		# 			key_inv.append(decdic[c])
		# 		else:
		# 			key_inv.append('?')
		
		# 	for i in range(len(key_inv)):
		# 		if key_inv[i] == '?':
		# 			for c in alphabet:
		# 				if c not in key_inv:
		# 					key_inv[i] = c
		# 					break

		# 	print()
		# 	print(key_inv)
		# 	print("NEW BEST", best_score)

		# 	key = decode(alphabet, key_inv)

		# 	for j in range(len(cipher)):
		# 		print(word_end[j], end='')
		# 	print()

		# 	print(decode(cipher, key))
		# 	print("key:", ''.join(key))
		return True

	good = False

	if cipher[i] in decdic:
		c = decdic[cipher[i]]
  
		if c in u.sons:
			good |= jazda(i + 1, u.sons[c])
   
		if u.end and c in root.sons:
			word_end[i - 1] = 1
			words += 1
			good |= jazda(i + 1, root.sons[c])
			word_end[i - 1] = 0		
			words -= 1
	
		return good

	def get_key(c):
		# return abs(freq[c] - cnt[cipher[i]] / len(cipher)) - di_freq[decdic[cipher[i - 1]] + c]
		return - di_freq[decdic[cipher[i - 1]] + c]

	if i > 0:
		order = sorted(alphabet, key=get_key)
	else:
		order = sorted(alphabet, key=lambda c: abs(freq[c] - cnt[cipher[i]] / len(cipher)))
 
	for c in order:
		diff = freq[c] - cnt[cipher[i]] / len(cipher)
		if c not in decdic.values() and (c == order[0] or abs(diff) < GAMMA):
			if c in u.sons:
				decdic[cipher[i]] = c
				good |= jazda(i + 1, u.sons[c])
				decdic.pop(cipher[i])

	# potential space encouter (check at the end)
	if u.end:
		for c in order:
			diff = freq[c] - cnt[cipher[i]] / len(cipher)
			if c not in decdic.values() and (c == order[0] or abs(diff) < GAMMA):
				if c in root.sons:
					word_end[i - 1] = 1
					words += 1
					decdic[cipher[i]] = c
					good |= jazda(i + 1, root.sons[c])
					decdic.pop(cipher[i])
					word_end[i - 1] = 0
					words -= 1
    
	# if good == False:
	# 	node.bad.add((i, frozendict(decdic)))
 
	return good

hint = 'WATERS'

possible_decdic = []

for i in range(len(cipher) - len(hint) + 1):
	decdic = {}
 
	dic_score = 0
 
	for j in range(len(hint)):
		decdic[cipher[i + j]] = hint[j]
		
		diff = freq[hint[j]] - cnt[cipher[i + j]] / len(cipher)

		if abs(diff) > GAMMA:
			dic_score += 1000
		dic_score += diff * diff
	
	dd = ''
	for j in range(len(cipher)):
		if cipher[j] in decdic:
			dd += decdic[cipher[j]]
		else:
			dd += '?'

	
	di_score = 0
	di_cnt = 0
	for j in range(len(cipher) - 1):
		if dd[j] != '?' and dd[j + 1] != '?':
			di_score += di_freq[dd[j] + dd[j + 1]]
			di_cnt += 1

	dic_score -= di_score / di_cnt
 
	if len(decdic) != len(hint):
		dic_score += 100000

	possible_decdic.append((dic_score, decdic))

# apply possible characters for words with size >= 4
# CNT = 0
# def trie_search(i, node, depth):
# 	global CNT
 
# 	res = (depth >= 4 and node.end)
# 	if i == len(cipher):
# 		return res
 
# 	if cipher[i] in decdic:
# 		if decdic[cipher[i]] in node.sons:
# 			res |= trie_search(i + 1, node.sons[decdic[cipher[i]]], depth + 1)
# 	else:
# 		for c, son in node.sons.items():
			
# 			decdic[cipher[i]] = c
   
# 			if trie_search(i + 1, son, depth + 1):
# 				res = True			
# 				possible_chars[i][c] = possible_chars[i].get(c, 0) + 1
			
# 			decdic.pop(cipher[i])

# 	if res:
# 		CNT += 1
	
# 	return res

done = []
possible_decdic = sorted(possible_decdic, key=lambda x:x[0])

possible_decdic = possible_decdic[3:] # double WATERS occurence

for score, cur_decdic in possible_decdic:
	if cur_decdic in done:
		continue
	done.append(cur_decdic)

	decdic = cur_decdic
	ss = ''
	for i in range(len(cipher)):
		if cipher[i] in decdic:
			ss = ss + decdic[cipher[i]]
		else:
			ss = ss + '?'
	# print(ss)
 
  
	# print(decdic)

	# CNT = 0
	# ile = 0
	# possible_chars = [{} for i in range(len(cipher))]
	# for i in range(len(cipher)):
	# 	if cipher[i] in decdic:
	# 		ile += 1
	# 	trie_search(i, root, 0)
	# print(CNT / ile)

	decdic['O'] = 'F'
	decdic['B'] = 'C'
	print("JAZDA", score, decdic)

	# # exit(0)
	jazda(0, root)
	exit(0)

print(best_score)
print(answer_dic)
