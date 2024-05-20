/*

	Opis rozwiazania:
		Funkcja opt_dist dla kazdego okienka o dlugosci D
		liczy ile jest '0' w okienku oraz '1' poza okienkiem
		=> tyle dokladnie potrzeba zmienic aby to dane okienko
		stalo sie poprawne.
		
		Obliczanie ilosci '0' w okienku i '1' poza okienkiem
		jest znajdowane przez przesuwanie okienka od 0 do |s| - d
		i branie po tym maximum.
		Jedyne elementy ktore moga potencjalnie wplynac na liczbe zmian
		podczas przesuniecia okienka dalej sa na dwoch pozycjach. 
		s[i - d] oraz s[i]. Wystarczy sprawdzic jakie wartosci sie
		na nich znajduja i zmienic aktualna ilosc zmian w okienku
		zgodnie z ich wartoscia oraz sprawdzic czy nie jest to
		nowe najlepsze rozwiazanie.

		Zlozonosc: O(|s|)
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int opt_dist(std::string s, int d) {
	int now = 0;

	for (int i = 0; i < d; i++)
		if (s[i] == '0')
			now++;
	
	for (int i = d; i < (int) s.size(); i++)
		if (s[i] == '1')
			now++;

	int ans = now;
	
	for (int i = d; i < (int) s.size(); i++) {
		if (s[i - d] == '1')
			now++;
		else
			now--;

		if (s[i] == '0')
			now++;
		else
			now--;

		ans = std::min(ans, now);
	}

	return ans;
}

int main() {
	std::ifstream input("zad4_input.txt");
	std::ofstream output("zad4_output.txt");

	std::string text;
	while (std::getline(input, text)) {
		std::stringstream ss(text);

		std::string s; int d;

		ss >> s >> d;

		output << opt_dist(s, d) << '\n';
	}
}
