import re
from nifs3 import display_spline
def read_data_from_file(filename):
	data_sets = []
	with open(filename, 'r') as file:
		for line in file:
			if line == "\n" : 
				continue
			data = {}
			# Find all key-value pairs using regex
			matches = re.findall(r'(\w+) := \[(.*?)\]', line)
			print("match:", matches, line)
			for key, value in matches:
				print("key:", key, "value: ", value)
				data[key] = list(map(float, value.split(',')))
			data_sets.append(data)
	return data_sets

filename = 'konkurs-I-345615-dane.txt'
data_sets = read_data_from_file(filename)

print("data_setes")
print(data_sets)

N, I, U = 0, 0, 0
for data in data_sets:
	pl = display_spline(data['x'],data['y'],data['t'],data['u'])
	N += 1
	I += len(data['x'])
	U += len(data['u'])
print(N, I, U)
pl.show()
