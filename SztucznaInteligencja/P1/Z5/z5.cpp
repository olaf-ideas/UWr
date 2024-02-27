/*
	Opis rozwiazania:
		Przeklejam funkcje zadania 4 i aplikuje algorytm z tresci
 		Program co 10k krokow probuje dwie polepszenia:
			-> z 5% szansy zmieniam losowa komorke na przeciwna
			-> z 95% szansy szuka komorki ktorej zmiana najbardziej zmniejszy koszt funkcji opt_dist
		Co zmiane sprawdzam czy nie rozwiazalem juz zadania przez odpytywanie kazdego wiersza i 
		kolumny o wartosci opt_dist. Jesli wszystkie sa 0 to mam rozwiazanie.
 */
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>

std::mt19937 rng(2137);

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

	std::ifstream input("zad5_input.txt");
	std::ofstream output("zad5_output.txt");

	int n, m;
	input >> n >> m;

	std::vector<std::string> board(n, std::string(m, '0'));
	std::vector<int> row_val(n), col_val(m);

	for (int &x : row_val)
		input >> x;
	
	for (int &x : col_val)
		input >> x;
	
	for (int REP = 0; REP < 1000; REP++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (rng() & 1)
					board[i][j] = '0';
				else
					board[i][j] = '1';
			}
		}

		for (int rep = 0; rep < 10000; rep++) {
			if (rng() % 100 < 5) { // 5% for random change
				board[rng() % n][rng() % m] ^= 1;
			}
			else {
				int best_change = 1e9;
				int x = -1, y = -1;

				for (int i = 0; i < n; i++) {
					for (int j = 0; j < m; j++) {
						board[i][j] ^= 1;

						std::string row, col;

						for (int k = 0; k < m; k++)
							row += board[i][k];
						
						for (int k = 0; k < n; k++)
							col += board[k][j];

						int change_score = opt_dist(row, row_val[i]) +
										   opt_dist(col, col_val[j]);

						if (best_change > change_score) {
							best_change = change_score;
							x = i;
							y = j;
						}

						board[i][j] ^= 1;
					}
				}

				board[x][y] ^= 1;
			}
			
			bool solved = true;

			for (int i = 0; i < n; i++) {
				if (opt_dist(board[i], row_val[i]) != 0) {
					solved = false;
					break;
				}
			}
			
			for (int i = 0; i < m; i++) {
				std::string col;

				for (int j = 0; j < n; j++)
					col += board[j][i];

				if (opt_dist(col, col_val[i]) != 0) {
					solved = false;
					break;
				}
			}
				
			if (solved) {
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < m; j++) {
						if (board[i][j] == '0')
							output << '.';
						else
							output << '#';
					}

					output << '\n';
				}

				return 0;
			}
		}
	}

	std::cerr << "FAILED, that's not your lucky seed\n";

	return 0;
}
