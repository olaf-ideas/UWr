
# ABCDEFG
segments = [
	0b1111110, #0
	0b0110000, #1
	0b1101101, #2
	0b1111001, #3
	0b0110011, #4
	0b1011011, #5
	0b1011111, #6
	0b1110000, #7
	0b1111111, #8
	0b1111011, #9
]

for i in segments:
	bits = []
	for j in range(7):
		bits.append(i >> j & 1)
	#print(i, bits)
	bits = list(reversed(bits))
	#print(i, bits)
	res = 0
	for j in bits:
		res *= 2
		res += j
	print(hex(res),end=',')
print()
