morse = [
	0b01100000,
	0b10001000,
	0b10101000,
	0b10010000,
	0b01000000,
	0b00101000,
	0b11010000,
	0b00001000,
	0b00100000,
	0b01111000,
	0b10110000,
	0b01001000,
	0b11100000,
	0b10100000,
	0b11110000,
	0b01101000,
	0b11011000,
	0b01010000,
	0b00010000,
	0b11000000,
	0b00110000,
	0b00011000,
	0b01110000,
	0b10011000,
	0b10111000,
	0b11001000
]

print(len(morse))

for i in morse:
	bits = []
	ok = 0
	for j in range(8):
		if ok:
			bits.append(i >> j & 1)
		if i >> j & 1:
			ok = 1
	#print(i, bits)
	# bits = list(reversed(bits))
	#print(i, bits)
	bits.append(1)
	while len(bits) < 8:
		bits.append(0)
	#print(i, bits)
	res = 0
	for x in bits:
		res *= 2
		res += x
	print(hex(res),end=',')
print()
