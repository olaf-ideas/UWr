from itertools import cycle
alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
alphalen = len(alphabet)
num2char = dict(enumerate(alphabet))
char2num = { num2char[n]: n for n in num2char }

def encodeChar(c, k):
	return num2char[(char2num[c] + char2num[k]) % alphalen]

def encode(plaintext, key):
	return ''.join(map(encodeChar, plaintext, cycle(key)))

cipher = 'FDGEFYQUMYMODKBFASDMBTQD'

for ch in alphabet:
	print(encode(cipher, str(ch)), num2char[-char2num[ch] % alphalen])

plaintext = 'TRUSTMEIAMACRYPTOGRAPHER'
key = 'M'

assert encode(plaintext, key) == cipher
