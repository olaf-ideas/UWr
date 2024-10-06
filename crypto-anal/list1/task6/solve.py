import sys

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

alphabet = '*ABCDEFGHIJKLMNOPQRSTUVWXYZ'

cipher = 'LPMUQEMQMGKLWDPMCYRHMHQXMLBMQSKLRMQBHMPDVMCRYCJEMUVKVMEPKLJLBWMPDLKPVVBMULBEPYBMEILPDMDLEMCDLBMBAZZRVHMLBPYMDLEMGKVQEPMLBMQBMVOOYKPMPYMVECQSVMPDVM*LRVMULBHMERLSSVHMTALCJRXMPDKYAWDMPDVMWRQEEMHYYKEMYOM*LCPYKXMIQBELYBEMPDYAWDMBYPMTALCJRXMVBYAWDMPYMSKV*VBPMQMEULKRMYOMWKLPPXMHAEPMOKYIMVBPVKLBWMQRYBWMULPDMDLIMPDVMDQRRUQXMEIVRPMYOMGYLRVHMCQGGQWVMQBHMYRHMKQWMIQPEMQPMYBVMVBHMYOMLPMQMCYRYAKVHMSYEPVKMPYYMRQKWVMOYKMLBHYYKMHLESRQXMDQHMGVVBMPQCJVHMPYMPDVMUQRRMLPMHVSLCPVHMELISRXMQBMVBYKIYAEMOQCVMIYKVMPDQBMQMIVPKVMULHVMPDVMOQCVMYOMQMIQBMYOMQGYAPMOYKPXMOL*VMULPDMQMDVQ*XMGRQCJMIYAEPQCDVMQBHMKAWWVHRXMDQBHEYIVMOVQPAKVEMULBEPYBMIQHVMOYKMPDVMEPQLKEMLPMUQEMBYMAEVMPKXLBWMPDVMRLOPMV*VBMQPMPDVMGVEPMYOMPLIVEMLPMUQEMEVRHYIMUYKJLBWMQBHMQPMSKVEVBPMPDVMVRVCPKLCMCAKKVBPMUQEMCAPMYOOMHAKLBWMHQXRLWDPMDYAKEMLPMUQEMSQKPMYOMPDVMVCYBYIXMHKL*VMLBMSKVSQKQPLYBMOYKMDQPVMUVVJMPDVMORQPMUQEMEV*VBMORLWDPEMASMQBHMULBEPYBMUDYMUQEMPDLKPXMBLBVMQBHMDQHMQM*QKLCYEVMARCVKMQGY*VMDLEMKLWDPMQBJRVMUVBPMERYURXMKVEPLBWMEV*VKQRMPLIVEMYBMPDVMUQXMYBMVQCDMRQBHLBWMYSSYELPVMPDVMRLOPMEDQOPMPDVMSYEPVKMULPDMPDVMVBYKIYAEMOQCVMWQZVHMOKYIMPDVMUQRRMLPMUQEMYBVMYOMPDYEVMSLCPAKVEMUDLCDMQKVMEYMCYBPKL*VHMPDQPMPDVMVXVEMOYRRYUMXYAMQGYAPMUDVBMXYAMIY*VMGLWMGKYPDVKMLEMUQPCDLBWMXYAMPDVMCQSPLYBMGVBVQPDMLPMKQBM'

# https://gist.github.com/pozhidaevak/0dca594d6f0de367f232909fe21cdb2f
# added '*'
# does not sum up to 100
freq = {'*': 22.327768906953113, 'E': 12.0036010803241, 'T': 9.102730819245775, 'A': 8.122436731019306, 'O': 7.682304691407423, 'I': 7.3121936580974305, 'N': 6.952085625687707, 'S': 6.281884565369612, 'R': 6.02180654196259, 'H': 5.921776532959889, 'D': 4.321296388916676, 'L': 3.981194358307493, 'U': 2.8808642592777836, 'C': 2.7108132439731922, 'M': 2.6107832349704916, 'F': 2.300690207062119, 'Y': 2.1106331899569875, 'W': 2.0906271881564473, 'G': 2.0306091827548265, 'P': 1.8205461638491551, 'B': 1.4904471341402423, 'V': 1.1103330999299794, 'K': 0.6902070621186357, 'X': 0.1700510153045914, 'Q': 0.11003300990297091, 'J': 0.10003000900270083, 'Z': 0.07002100630189059}

freq = {k: v / sum(freq.values()) for k, v in freq.items()}

cnt = {c: 0 for c in alphabet}
for c in cipher:
	cnt[c] += 1

for a, b in cnt.items():
	print(a, b / len(cipher))

'''

for c in cipher:
	if c == 'M':
		print(' ', end='')
	else:
		print(c, end='')
print()

print(freq)

exit(0)

print(freq)
'''

class TrieNode:
	def __init__(self):
		self.sons = {}
		self.end = False

root = TrieNode()

# https://github.com/dwyl/english-words/blob/master/words.txt
with open('words.txt', 'r') as words:
	for word in words:
		word = word.upper()[:-1]

		ok = True
		for c in word:
			if c not in alphabet:
				ok = False
				break
		
		if ok:
			node = root

			for c in word:
				if c not in node.sons:
					node.sons[c] = TrieNode()
				node = node.sons[c]

			node.end = True

print("DICT LOADED")

decdic = {'M': '*'}

def jazda(i, u):
	#print(i, decdic)
	for j in range(i):
		print(decdic[cipher[j]], end='')
	print()

	if i == len(cipher):
		print("SUCCESS")
		key_inv = list()
		for c in alphabet:
			if c in decdic:
				key_inv.append(decdic[c])
			else:
				key_inv.append('?')
		
		for i in range(len(key_inv)):
			if key_inv[i] == '?':
				for c in alphabet:
					if c not in key_inv:
						key_inv[i] = c
						break

		print()
		print(key_inv)

		key = decode(alphabet, key_inv)

		print(decode(cipher, key))
		print("key:", ''.join(key))
		exit(0)

	if cipher[i] in decdic:
		c = decdic[cipher[i]]

		if c == '*':
			if u.end:
				jazda(i + 1, root)
		elif c in u.sons:
			jazda(i + 1, u.sons[c])

		return

	order = sorted(alphabet, key=lambda c: abs(freq[c] - cnt[cipher[i]] / len(cipher)))
	for c in order:
		diff = freq[c] - cnt[cipher[i]] / len(cipher)
		#print(c, diff, cipher[i], freq[c], cnt[cipher[i]] / len(cipher))
		if c not in decdic.values() and \
			c in u.sons and \
			(c == order[0] or abs(diff) < 0.01):
			decdic[cipher[i]] = c
			jazda(i + 1, u.sons[c])
			decdic.pop(cipher[i])


jazda(0, root)
