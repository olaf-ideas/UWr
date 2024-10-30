import os

def my_randint(i):
	k = 0
	while 2 ** k < i:
		k += 1
 
	while True:
		j = int.from_bytes(os.urandom((k + 7) >> 3)) & ((1 << k) - 1)
		if j < i:
			return j

def generate_key():
	key = bytearray(256)

	for i in range(256):
		key[i] = i
	
	for i in range(256 - 1, 0, -1):
		j = my_randint(i + 1)
		key[i], key[j] = key[j], key[i]
	
	return bytes(key)

def encrypt_block(block, key):
	return bytearray(key)[block]

def decrypt_block(block, inv_key):
	return bytearray(inv_key)[block]

def encrypt_ecb(text, key):
	cipher = bytearray()

	for block in text:
		cipher.append(encrypt_block(block, key))

	return cipher

def decrypt_ecb(cipher, key):
	inv_key = bytearray(256)
	for i in range(256):
		inv_key[key[i]] = i
	
	text = bytearray()
	for block in cipher:
		text.append(decrypt_block(block, inv_key))
	return text

def encrypt_cbc(text, key, iv):
	cipher = bytearray()

	iv = int.from_bytes(iv)
	for block in text:
		cipher.append(encrypt_block(block ^ iv, key))
		iv = cipher[-1]
	return cipher

def decrypt_cbc(cipher, key, iv):
	inv_key = bytearray(256)
	for i in range(256):
		inv_key[key[i]] = i
	
	text = bytearray()
	iv = int.from_bytes(iv)
	for block in cipher:
		text.append(decrypt_block(block, inv_key) ^ iv)
		iv = block

	return text

iv = os.urandom(1)
key = generate_key()

message = b'Tekst do przetestowania zadania 10'

# przy 8 bitowym rozmiarze bloku szansa na kolizje jest bardzo duza
# jest duza szansa ze te same blocki zaszyfruja sie do tego samej wartosci

# ecb -> kijowe bo podatne na zwykla czestotliwosc literek
# W ofb cykl jest dosc maly ok 16 wiec mozna atakowac tekst czestoscia literek

assert decrypt_ecb(encrypt_ecb(message, key), key) == message
assert decrypt_cbc(encrypt_cbc(message, key, iv), key, iv) == message
