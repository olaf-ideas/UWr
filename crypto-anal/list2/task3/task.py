import os
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
import binascii

secret_key = os.urandom(16)

def BobToBank():
	global secret_key

	iv = os.urandom(16)

	plaintext = [
		b'Send $100 to Bob',
		b' from my account',
		b'.               '
	]

	message = [iv]

	for line in plaintext:
		cipher = Cipher(algorithms.AES128(secret_key), mode = modes.CBC(iv))		
		encryptor = cipher.encryptor()
		
		message.append(encryptor.update(line) + encryptor.finalize())
		iv = message[-1]

	return message

def DecryptMessage(message):
	iv = message[0]
	cipher = message[1:]	

	for line in cipher:
		cipher = Cipher(algorithms.AES128(secret_key), mode = modes.CBC(iv))
		decryptor = cipher.decryptor()
		
		text = decryptor.update(line) + decryptor.finalize()
		print(text.decode('ascii'))
		iv = line

message = BobToBank()

print("original message:")
DecryptMessage(message)
print("")

hack = bytes(a ^ b for a, b in zip(b'Send $100 to Bob', b'Send $100 to Eve'))

# edytujemy IV na nasza korzysc
message[0] = bytes(a ^ b for a, b in zip(message[0], hack))

# zamiana IV na E_k(IV) zapobiega manipulacją IV. E_k(IV) jest losowe i nie sposob nim znalesc wymaganej kolizji.

DecryptMessage(message)
