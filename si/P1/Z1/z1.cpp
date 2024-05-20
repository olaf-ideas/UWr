/*
	Opis rozwiazania:
		Przedstawiam stan gry jako trojke pozycji - bialego krola, czarnego krola i bialej wiezy.
		Przeszukuje drzewo stanow zwyklym BFS'em oraz zapamietuje ostatni ruch ktory umozliwil mi
		dojscie do danej pozycji. Stanow gry jest mniej niz 64**3 wiec caly program powinniem bardzo
		szybko dzialac dla dowolnego przypadku	
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <queue>
#include <map>

const bool DEBUG = true;

enum Turn {
	WHITE,
	BLACK
};

const int king_dx[8] = {+1, +1, +1,  0, -1, -1, -1,  0};
const int king_dy[8] = {+1,  0, -1, +1, +1,  0, -1, -1};
const int rook_dx[4] = {+1,  0, -1,  0};
const int rook_dy[4] = { 0, -1,  0, +1};

bool inside(int col, int row) {
	return 'a' <= col && col <= 'h' &&
		   '1' <= row && row <= '8';
}

int distance(int c1, int r1, int c2, int r2) {
	return std::max(abs(c1 - c2), abs(r1 - r2));
}

struct State {
	int k_col, k_row;
	int K_col, K_row;
	int R_col, R_row;
	Turn turn;

	int index() const {
		int idx = 0;

		idx = (idx | k_col) << 3;
		idx = (idx | k_row) << 3;
		idx = (idx | K_col) << 3;
		idx = (idx | K_row) << 3;
		idx = (idx | R_col) << 3;
		idx = (idx | R_row) << 3;
		idx = (idx | turn);

		return idx;
	}

	void debug() const {
		std::cerr << "> Turn: " << (turn == WHITE ? "WHITE" : "BLACK") << '\n';

		std::cerr << "   ";
		
		for (int col = 'a'; col <= 'h'; col++)
			std::cerr << char(col) << ' ';
		std::cerr << '\n';

		for (int row = '1'; row <= '8'; row++) {
			for (int col = 'a'; col <= 'h'; col++) {
				if (col == 'a')
					std::cerr << char(row) << ": ";

				if (k_col == col && k_row == row)
					std::cerr << 'k';
				else
				if (K_col == col && K_row == row)
					std::cerr << 'K';
				else
				if (R_col == col && R_row == row)
					std::cerr << 'R';
				else
					std::cerr << '.';
				std::cerr << ' ';
			}
			std::cerr << '\n';
		}
	}

	bool is_file_under_attack(int col, int row) const {
		for (int dir = 0; dir < 4; dir++) {
			int rook_col = R_col;
			int rook_row = R_row;

			while (true) {
				// file under the rook is NOT under the attack (enables capture by the black king)
				rook_col += rook_dx[dir];
				rook_row += rook_dy[dir];

				if (inside(rook_col, rook_row) == false ||
					(rook_col == K_col && rook_row == K_row))
					break;

				if (rook_col == col &&
					rook_row == row)
					return true;
			}
		}

		return false;
	}

	std::pair<int, std::vector<std::pair<std::string, State>>> generate_next_states() const {
		std::vector<std::pair<std::string, State>> next_states;

		// black king captured the white rook, stalemate is inevitable
		if (k_col == R_col && k_row == R_row) {
			return std::make_pair(2, next_states);
		}

		if (turn == WHITE) {
			for (int dir = 0; dir < 8; dir++) {
				int new_col = K_col + king_dx[dir];
				int new_row = K_row + king_dy[dir];

				if (inside(new_col, new_row) == false ||
					distance(new_col, new_row, k_col, k_row) <= 1 ||
					(new_col == R_col && new_row == R_row))
					continue;

				next_states.emplace_back(
					std::string("K") + char(new_col) + char(new_row),
					State {
						k_col, k_row,
						new_col, new_row,
						R_col, R_row,
						BLACK
					}
				);
			}

			for (int dir = 0; dir < 4; dir++) {
				int new_col = R_col;
				int new_row = R_row;

				while (true) {
					new_col += rook_dx[dir];
					new_row += rook_dy[dir];
				
					if (inside(new_col, new_row) == false ||
						(new_col == k_col && new_row == k_row) ||
						(new_col == K_col && new_row == K_row))
						break;

					next_states.emplace_back(
						std::string("R") + char(new_col) + char(new_row),
						State {
							k_col, k_row,
							K_col, K_row,
							new_col, new_row,
							BLACK
						}
					);
				}
			}
		}
		else 
		if (turn == BLACK) {
			for (int dir = 0; dir < 8; dir++) {
				int new_col = k_col + king_dx[dir];
				int new_row = k_row + king_dy[dir];

				if (inside(new_col, new_row) == false ||
					distance(new_col, new_row, K_col, K_row) <= 1 ||
					is_file_under_attack(new_col, new_row))
					continue;

				next_states.emplace_back(
					std::string("k") + char(new_col) + char(new_row),
					State {
						new_col, new_row,
						K_col, K_row,
						R_col, R_row,
						WHITE
					}
				);
			}

			// mate
			if (next_states.empty() && is_file_under_attack(k_col, k_row)) {
				return std::make_pair(1, next_states);
			}
		}
		else {
			assert(false);
		}

		// stalemate
		if (next_states.empty())
			return std::make_pair(2, next_states);
		
		return std::make_pair(0, next_states);
	}
};

bool operator< (const State &A, const State &B) {
	return std::tie(A.k_col, A.k_row, A.K_col, A.K_row, A.R_col, A.R_row, A.turn) <
           std::tie(B.k_col, B.k_row, B.K_col, B.K_row, B.R_col, B.R_row, B.turn);
}

bool operator!= (const State &A, const State &B) {
	return (A < B) || (B < A);
}

int main() {
	
	std::ifstream input("zad1_input.txt");
	std::ofstream output("zad1_output.txt");

	std::string line;
	while (std::getline(input, line)) {

		State start;
		
		start.turn = (line[0] == 'w' ? WHITE : BLACK);

		start.K_col = line[6];
		start.K_row = line[7];

		start.R_col = line[9];
		start.R_row = line[10];

		start.k_col = line[12];
		start.k_row = line[13];

		std::map<State, std::pair<State, std::string>> previous;
		std::queue<std::pair<int, State>> queue;
		
		queue.emplace(0, start);

		int answer = -1;
		State last_state;

		while (queue.size()) {
			auto [moves, curr_state] = queue.front();
			queue.pop();

			auto [is_end, next_states] = curr_state.generate_next_states();
			
			if (is_end == 0) {
				for (auto &[move_name, next_state] : next_states) {
					if (previous.find(next_state) == previous.end()) {
						previous[next_state] = 
							std::make_pair(curr_state, 
										   move_name);

						queue.emplace(moves + 1, next_state);
					}
				}
			}
			else
			if (is_end == 1) {
				answer = moves;
				last_state = curr_state;
				break;
			}
			else
			if (is_end == 2) {
				continue;
			}
		}

		std::vector<std::string> moves;
		std::vector<State> states;
		while (last_state != start) {
			auto [prev_state, move] = previous[last_state];

			moves.push_back(move);
			states.push_back(last_state);
			last_state = prev_state;
		}
		states.push_back(last_state);

		std::reverse(moves.begin(), moves.end());
		std::reverse(states.begin(), states.end());

		if (answer == -1)
			output << "INF\n";
		else {
			output << answer << '\n';

			if (DEBUG) {
				for (int i = 0; i < (int) moves.size(); i++) {
					states[i].debug();
					std::cerr << moves[i] << '\n';
				}
				states.back().debug();
			}
		}
	}
}
