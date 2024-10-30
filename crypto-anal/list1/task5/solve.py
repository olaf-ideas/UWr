from itertools import cycle
alphabet = 'AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ'
alphalen = len(alphabet)
num2char = dict(enumerate(alphabet))
char2num = { num2char[n]: n for n in num2char }

def encodeChar(c, k):
	return num2char[(char2num[c] + char2num[k]) % alphalen]

def decodeChar(c, k):	
	return num2char[(char2num[c] - char2num[k]) % alphalen]

def encode(plaintext, key):
	return ''.join(map(encodeChar, plaintext, cycle(key)))

def decode(cipher, key):
	return ''.join(map(decodeChar, cipher, cycle(key)))

cipher = 'ŃABQYĆVLHĄOHQKĆNVODGQVŹŁFIXDWUJOZQNXŃNMRHZŹVSUJDFCWUFKRYĘSUÓLIWZEĆFHĄZNŚĘRKNYRZXCSIKQKĘVZZSHAMĆULNBSHJTÓHINQDOWŹKŁGSDATFŹCJDNMYAABCWXQOŻKĘYXTCXŹGZQMĆBZIĘZEXSŃLĄFŹPDVNYNVŹZCIOFSQŻZKGAPXÓŁRUŃŃIQRUÓŁNZSYHQPBLIMŻAOPĄWLNBCEŚXSZĄÓOCUAZXTXCUJUĆIHĄNAGGIUNMBCBDXRQLOBIQOBYPIPZXXĘZĘONOTŹQIULUXHRNAKSHBŹĘTFXWZSĘĘÓBVIBJRAÓĘWWQŹZEĆDTNŹNŚ'

def negate(x):
	return ''.join([num2char[-char2num[c] % alphalen] for c in x])

frequency = {
	'A':	8.37,
	'Ą':	0.79,
	'B':	1.93,
	'C':	3.89,
	'Ć':	0.60,
	'D':	3.35,
	'E':	8.68,
	'Ę':	1.13,
	'F':	0.26,
	'G':	1.46,
	'H':	1.25,
	'I':	8.83,
	'J':	2.28,
	'K':	3.01,
	'L':	2.24,
	'Ł':	2.38,
	'M':	2.81,
	'N':	5.69,
	'Ń':	0.16,
	'O': 	7.53,
	'Ó':	0.79,
	'P':	2.87,
	'R':	4.15,
	'S':	4.13,
	'Ś':	0.72,
	'T':	3.85,
	'U':	2.06,
	'W':	4.11,
	'Y':	4.03,
	'Z':	5.33,
	'Ź':	0.08,
	'Ż':	0.93,

	'Q':	0.00,
	'V':	0.00,
	'X':	0.00
}

bad = "X"

hint = 'KULTURY'

best = ''
best_score = 1e9

xd = []

for key_len in range(3, 14):
	print("key_len: ", key_len)
	for i in range(len(cipher) - len(hint) + 1):

		key = ['?' for j in range(key_len)]

		for j in range(len(hint)):
			key[(i + j) % key_len] = decode(cipher[i + j], hint[j])

		original_key = key.copy()

		for j in range(len(key)):
			if key[j] == '?':
				key[j] = 'A'
		
		plaintext = decode(cipher, key)

		ok = False
		for c in bad:
			for j in range(len(plaintext)):
				if original_key[j % key_len] != '?' and \
					plaintext[j] == c:
					#print('fail', c, j, plaintext[j], original_key[j % key_len])
					ok = False
		
		score = 0
		cnt = [{c: 0 for c in alphabet} for i in range(key_len)]
		suma = [0 for i in range(key_len)]
		for i in range(len(plaintext)):
			cnt[i % key_len][plaintext[i]] += 1
			suma[i % key_len] += 1

		good = 0
		for i in range(key_len):
			if original_key[i] == '?':
				continue

			good += 1
			
			for c, ile in cnt[i].items():
				diff = ile / suma[i] - frequency[c]
				score += diff * diff

		score /= good

		if best_score > score:
			best_score = score
			best = key
			print(">", score, key_len, key)
			print(decode(cipher, key))

		#print(key_len, key, original_key)
		# print(hint in plaintext, ok, key, score)
		xd.append((score, key))

		if ok:
			print(score, key)
			print(plaintext)

print("best_score: ", best_score)
print("best: ", best)
print(decode(cipher, best))


key = 'PAMIĘTNIK'
plaintext = 'ZAPISKICZŁOWIEKANEOGENUSTANOWIĄJEDENZNAJCENNIEJSZYCHZABYTKÓWZAMIERZCHŁEJPRZESZŁOŚCIZIEMIPOCHODZĄZESCHYŁKOWEGOOKRESUKULTURYPRECHAOTYCKIEJKTÓRAPOPRZEDZIŁAWIELKIROZPADJESTIRONICZNYMPARADOKSEMDZIEJÓWŻEOCYWILIZACJACHWCZESNEGONEOGENUOPRAKULTURACHASYRIIEGIPTUGRECJIWIEMYDALEKOWIĘCEJANIŻELIOCZASACHPRAATOMISTYKIIASTROGACJIPIERWOTNEJ'
print(decode(cipher, key))
