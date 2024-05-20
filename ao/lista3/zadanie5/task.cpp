#include <bits/stdc++.h>

using namespace std;

vector<tuple<int, int, int>> lines = {
	make_tuple(1, 2, 3),
	make_tuple(4, 5, 6),
	make_tuple(7, 8, 9),
	make_tuple(1, 4, 7),
	make_tuple(2, 5, 8),
	make_tuple(3, 6, 9),
	make_tuple(1, 5, 9),
	make_tuple(3, 5, 7), // BUG: missclick, powinno być (3, 5, 7)
};

bool three_in_a_row(string board) {
	for (auto [i, j, k] : lines) {
		if (board[i] == board[j] &&
			board[j] == board[k] &&
			board[k] != '.')
			return true;
	}

	return false;
}

map<string, string> best_moves;

int search(string board, int moves_left) {
	if (three_in_a_row(board))
		return -1;
	
	if (moves_left == 0)
		return 0;

	int value = -1;

	for (int i = 1; i <= 9; i++) {
		if (board[i] == '.') {
			if (moves_left % 2 == 1)
				board[i] = 'O';
			else
				board[i] = 'X';
			
			int score = -search(board, moves_left - 1);

			board[i] = '.';

			if (value < score) {
				value = score;
				best_moves[board] = string(1, char(i + '0'));
			}
			else
			if (value == score) {
				best_moves[board] += char(i + '0');
			}

		}
	}

	return value;
}

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	string board(10, '.');
	int moves_left = 0;

	for (int i = 1; i <= 9; i++) {
		cin >> board[i];
		
		if (board[i] == '.')
			moves_left++;
	}

	int game_value = search(board, moves_left);

	if (game_value == -1) {
		cout << "NIE\n";
	}
	else
	if (game_value == 0) {
		cout << "REMIS\n";
		cout << best_moves[board] << '\n';
	}
	else
	if (game_value == +1) {
		cout << "TAK\n";
		cout << best_moves[board] << '\n';
	}
	else {
		assert(false);
	}
	
	return 0;
}
