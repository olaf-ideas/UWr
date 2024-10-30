import os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
import binascii

secret_key = os.urandom(16)
secret_iv = os.urandom(16)

def Encrypt(text):
	global secret_key
	global secret_iv

	ciphertext = []

	iv = secret_iv
	for block in text:
		cipher = Cipher(algorithms.AES128(secret_key), mode=modes.CBC(iv))
		encryptor = cipher.encryptor()

		ciphertext.append(encryptor.update(block) + encryptor.finalize())
		iv = ciphertext[-1]

	return ciphertext

def Decrypt(ciphertext):
	global secret_iv

	iv = secret_iv
	text = []
	for block in ciphertext:
		cipher = Cipher(algorithms.AES128(secret_key), mode = modes.CBC(iv))
		decryptor = cipher.decryptor()
		
		text.append(decryptor.update(block) + decryptor.finalize())
		iv = block
	return text

assert Decrypt(Encrypt([b'abababababababab'])) == [b'abababababababab']

def combine(A, B):
	return bytes(a ^ b for a, b in zip(A, B))

# znany plaintext
p = [
	b'aaaaaaaaaaaaaaaa',
	b'aaaaaaaaaaaaaaaa',
	b'aaaaaaaaaaaaaaaa',
	b'aaaaaaaaaaaaaaaa',
	b'aaaaaaaaaaaaaaaa',
	b'aaaaaaaaaaaaaaaa',
	b'aaaaaaaaaaaaaaaa',
	b'aaaaaaaaaaaaaaaa',
]

p_ = [
	b'bbbbbbbbbbbbbbbb',
	b'bbbbbbbbbbbbbbbb',
	b'bbbbbbbbbbbbbbbb',
	b'bbbbbbbbbbbbbbbb',
	b'bbbbbbbbbbbbbbbb',
	b'bbbbbbbbbbbbbbbb',
	b'bbbbbbbbbbbbbbbb',
	b'bbbbbbbbbbbbbbbb',
]

c = Encrypt(p)
c_ = c

for i in range(len(p) // 2):
	c_[2 * i] = combine(c[2 * i], combine(p[2 * i + 1], p_[2 * i]))

print(*Decrypt(c_))
