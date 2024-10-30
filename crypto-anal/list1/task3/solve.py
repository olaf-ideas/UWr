import random

alphabet = " AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŻŹ"
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

secret_text = "ŻWIREK KRĘCI Z MUCHOMORKIEM"
fake_text   = "PIWO W CZWARTEK O ZACHODZIE"

secret_key = ''.join(random.choice(alphabet) for _ in range(len(secret_text)))

cipher = encode(secret_text, secret_key)

fake_key = ''.join(map(decodeChar, cipher, fake_text))

print(cipher)
print(decode(cipher, secret_key))
print(decode(cipher, fake_key))
