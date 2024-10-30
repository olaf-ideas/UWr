from itertools import cycle
alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
alphalen = len(alphabet)
num2char = dict(enumerate(alphabet))
char2num = { num2char[n]: n for n in num2char }

def encodeChar(c, k):
	return num2char[(char2num[c] + char2num[k]) % alphalen]

def encode(plaintext, key):
	return ''.join(map(encodeChar, plaintext, cycle(key)))

letterFrequency = {
	'E' : 12.0,
	'T' : 9.10,
	'A' : 8.12,
	'O' : 7.68,
	'I' : 7.31,
	'N' : 6.95,
	'S' : 6.28,
	'R' : 6.02,
	'H' : 5.92,
	'D' : 4.32,
	'L' : 3.98,
	'U' : 2.88,
	'C' : 2.71,
	'M' : 2.61,
	'F' : 2.30,
	'Y' : 2.11,
	'W' : 2.09,
	'G' : 2.03,
	'P' : 1.82,
	'B' : 1.49,
	'V' : 1.11,
	'K' : 0.69,
	'X' : 0.17,
	'Q' : 0.11,
	'J' : 0.10,
	'Z' : 0.07
}

cipher = 'XHQPMFTFSJBHAMEHGIGHISHLPHLJAECWRVSRJWXNQECBSIQSCQSRHERWTWSVLVMRVLJAECWRVSRJWXNQECBSIFIHCPKS'

def negate(x):
	return ''.join([num2char[-char2num[c] % alphalen] for c in x])

for key_len in range(1, 10):
	start = negate('ITMAY')
	while len(start) < key_len:
		start += 'A'
	start = start[:key_len]
	
	key = negate(encode(cipher, start))[:key_len]
	
	plaintext = encode(cipher, key)

	for i in range(len(plaintext)):
		if i % key_len >= 5:
			plaintext = plaintext[:i] + '-' + plaintext[i + 1:]
	
	cnt = {}
	suma = 0
	for i in range(0, len(plaintext), key_len):
		for j in range(0, 5):
			if i + j < len(plaintext):
				c = plaintext[i + j]
				if c != '-':
					# print(i + j, c)

					if c in cnt:
						cnt[c] += 1
					else:
						cnt[c] = 1
					suma += 1

	
	score = 0

	for c, p in cnt.items():
		diff = letterFrequency[c] - p / suma
		score += diff * diff
	
	print(round(score), key, plaintext)

# LMW ITMAYBEROUNDLYASSERTEDTHATHUMANINGENUITYCANNOTCONCOCTACIPHERWHICHHUMANINGENUITYCANNOTRESOLVE
