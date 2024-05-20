# -*- coding: utf-8 -*-

'''
	Opis rozwiazania:
		Zapisuje caly slownik do struktury danych trie w celu 
		szybkiego wykrywania wystapien slow w tekscie

		Nastepnie rozwiazuje reszte zadania przy pomocy
		programowania dymanicznego. 

		DP[i] = najwieksza wartosc pociecia suffixu i, i + 1, ...
					przy pomocy slow ze slownika

		DP[i] = max DP[k] + (k - i + 1)**2  gdy slowy [i,k] wystepuje w slowniku

		Oprocz takiej tablicy DP zapisuje w PV[i] optymalne k dla suffixu i

		Po policzeniu DP i PV budujemy pociecie tekstu przy ich pomocy.
'''

import codecs

class Node:
	def __init__(self):
		self.sons = dict()
		self.is_end = False	

trie = [Node()]

polish_words = codecs.open('polish_words.txt', 'r', 'utf-8')
answer = open('zad2_output.txt', 'w')

for word in polish_words.readlines():
	curr_node = 0

	for i in range(len(word) - 1):
		if word[i] not in trie[curr_node].sons:
			trie[curr_node].sons[word[i]] = len(trie)
			curr_node = len(trie)
			trie.append(Node())
		else:
			curr_node = trie[curr_node].sons[word[i]]
	
	trie[curr_node].is_end = True

for text in open('zad2_input.txt', 'r').readlines():
	text = text[:-1] # pop the endline

	dp = [0 for i in range(len(text) + 1)]
	pv = [-1 for i in range(len(text) + 1)]

	pv[len(text)] = 0
	for i in range(len(text) - 1, -1, -1):
		curr_node = 0

		for j in range(i, len(text)):
			if text[j] not in trie[curr_node].sons:
				break
			
			curr_node = trie[curr_node].sons[text[j]]
			
			if trie[curr_node].is_end and \
				pv[j + 1] != -1 and \
				dp[i] < dp[j + 1] + (j - i + 1) ** 2:
				
				dp[i] = dp[j + 1] + (j - i + 1) ** 2
				pv[i] = j

	idx = 0
	while idx < len(text):
		answer.write(text[idx : pv[idx] + 1] + ' ')
		idx = pv[idx] + 1
	answer.write('\n')

answer.close()
