/*
	Algorytm z poprzedniej pracowni został zmieniony następująco:
		-> opt_dist teraz liczy dp[i][j] -> minimalna liczba zmian dla prefiksu i uzywajac j pierwszych przedzialow
		-> jesli juz policzylem odpowiedz dla pewnej maski bitowej to cachuje ja w tablicy dla przyspieszania liczenia wartosci opt_dist
 	
	Reszta zostala jak byla
 */
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>

std::mt19937 rng(2137);

const int N = 20;

int cache[2 * N][1 << N];
int opt_dist(int s, int n, const std::vector<int> &d, int idx) {
	auto key = s | (1 << n);
	if (cache[idx][key] != -1) {	
		/*
		std::cerr << "CACHED\n";
		for (int i = 0; i < n; i++)
			if (s & (1 << i))
				std::cout << '1';
			else
				std::cout << '0';

		std::cerr << " d: ";
		for (int x : d)
			std::cerr << x << ',';
		std::cerr << " => " << cache[key] << '\n';
		*/
		return cache[idx][key];
	}

	const int m = (int) d.size();
	
	std::vector pref(n + 1, 0);
	for (int i = 1; i <= n; i++) {
		pref[i] = pref[i - 1];
		if ((s & (1 << (i - 1))) == 0) {
			pref[i]++;
		}
	}
	
	std::vector dp(n + 1, std::vector(m + 1, 1000));

	dp[0][0] = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j <= m; j++) {
			dp[i][j] = dp[i - 1][j];

			if (s & (1 << (i - 1)))
				dp[i][j]++;

			if (j == 1) {
				if (i - d[0] >= 0) {
					int cost = pref[i] - pref[i - d[0]];
					dp[i][j] = std::min(dp[i][j], dp[i - d[0]][0] + cost);
				}
			}
			else
			if (j > 1) {
				if (i - d[j - 1] - 1 >= 0) {
					int cost = pref[i] - pref[i - d[j - 1]];
					
					if (s & (1 << (i - d[j - 1] - 1)))
						cost++;
					
					dp[i][j] = std::min(dp[i][j], dp[i - d[j - 1] - 1][j - 1] + cost);
				}
			}
		}
	}

	/*
	for (int i = 0; i < n; i++)
		if (s & (1 << i))
			std::cout << '1';
		else
			std::cout << '0';
	std::cerr << " d: ";
	for (int x : d)
		std::cerr << x << ',';
	std::cerr << " => ";
	std::cerr << dp[n][m] << '\n';
	// */

	return cache[idx][key] = dp[n][m];
}

int main() {

	for (int i = 0; i < 2 * N; i++)
		for (int j = 0; j < (1 << N); j++)
			cache[i][j] = -1;

	std::ifstream input("zad_input.txt");
	std::ofstream output("zad_output.txt");

	int n, m;
	input >> n >> m;
	input.ignore();

	std::vector board_rows(n, 0);
	std::vector board_cols(m, 0);
	
	std::vector<std::vector<int>> row_vals(n), col_vals(m);

	for (int i = 0; i < n; i++) {
		std::string line;
		std::getline(input, line);
		std::stringstream ss(line);

		int value;
		while (ss >> value) {
			row_vals[i].push_back(value);
		}
	}
	
	for (int i = 0; i < m; i++) {
		std::string line;
		std::getline(input, line);
		std::stringstream ss(line);

		int value;
		while (ss >> value) {
			col_vals[i].push_back(value);
		}
	}
	
	for (int REP = 0; REP < 1000; REP++) {
		for (int i = 0; i < n; i++)
			board_rows[i] = 0;

		for (int j = 0; j < m; j++)
			board_cols[j] = 0;

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (rng() & 1) {
					board_rows[i] |= (1 << j);
					board_cols[j] |= (1 << i);
				}
			}
		}

		std::cerr << "REP\n";
		for (int rep = 0; rep < 100000; rep++) {
			if (rng() % 100 < 10) { // 5% for random change
				int i = rng() % n;
				int j = rng() % m;

				board_rows[i] ^= (1 << j);
				board_cols[j] ^= (1 << i);
			}
			else {
				int best_change = 1e9;
				int x = -1, y = -1;
	
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < m; j++) {
						int old_score = opt_dist(board_rows[i], m, row_vals[i], i + 0) +
										opt_dist(board_cols[j], n, col_vals[j], n + j);
					
						board_rows[i] ^= (1 << j);
						board_cols[j] ^= (1 << i);

						int change_score = opt_dist(board_rows[i], m, row_vals[i], i + 0) +
										   opt_dist(board_cols[j], n, col_vals[j], n + j);

						change_score -= old_score;

						if (best_change > change_score || (best_change == change_score && rng() % 2 == 0)) {
							best_change = change_score;
							x = i;
							y = j;
						}
						
						board_rows[i] ^= (1 << j);
						board_cols[j] ^= (1 << i);
					}
				}

				board_rows[x] ^= (1 << y);
				board_cols[y] ^= (1 << x);
			}
			
			bool solved = true;

			for (int i = 0; i < n && solved; i++) {
				if (opt_dist(board_rows[i], m, row_vals[i], i + 0) != 0) {
					solved = false;
				}
			}
			
			for (int i = 0; i < m && solved; i++) {
				if (opt_dist(board_cols[i], n, col_vals[i], n + i) != 0) {
					solved = false;
				}
			}
				
			if (solved) {
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < m; j++) {
						if (board_rows[i] & (1 << j))
							std::cerr << '#';
						else
							std::cerr << '.';
					}
					std::cerr << '\n';
				}

				for (int i = 0; i < n; i++) {
					for (int j = 0; j < m; j++) {
						if (board_rows[i] & (1 << j))
							output << '#';
						else
							output << '.';
					}

					output << '\n';
				}

				return 0;
			}
		}

		/*
		std::cerr << "row: ";
		for (int i = 0; i < n; i++) {
			std::cerr << opt_dist(board_rows[i], m, row_vals[i], i + 0) << ' ';
		}
		std::cerr << '\n';

		std::cerr << "col: ";
		for (int i = 0; i < m; i++) {
			std::cerr << opt_dist(board_cols[i], n, col_vals[i], n + i) << ' ';
		}
		std::cerr << '\n';

		std::cerr << "board:\n";
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (board_rows[i] & (1 << j))
					std::cerr << '#';
				else
					std::cerr << '.';
			}
			std::cerr << '\n';
		}
		std::cerr << '\n';
		// */
	}

	std::cerr << "FAILED, that's not your lucky seed\n";

	return 0;
}
