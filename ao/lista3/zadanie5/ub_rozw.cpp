#include <bits/stdc++.h>

using namespace std;

struct State {
	int o, x;

	void read() {
		for (int i = 0; i < 3; i++) {
			string row;
			cin >> row;

			for (int j = 0; j < 3; j++) {
				int bit = i * 3 + j;

				if (row[j] == 'O')
					o |= 1U << bit;
				else
				if (row[j] == 'X')
					x |= 1U << bit;
			}
		}
	}

	// 0 -> O turn, 1 -> X turn
	int turn() const {
		return (__builtin_popcount(o) + 
				__builtin_popcount(x)) % 2;
	}
	
	int is_terminal() const {

		for (int mask : {0b111'000'000,
						 0b000'111'000,
						 0b000'000'111,
						 0b100'100'100,
						 0b010'010'010,
						 0b001'001'001,
						 0b100'010'001,
						 0b001'010'100}) {
			if ((o & mask) == mask)
				return -1;

			if ((x & mask) == mask)
				return -1;
		}
		
		if ((o | x) == 0b111'111'111)
			return 0;

		return 2;
	}

	void make_move(int bit) {
		if (turn() == 0)
			o |= 1 << bit;
		else
			x |= 1 << bit;
	}

	void undo_move(int bit) {
		if (turn() == 0)
			x &= ~(1 << bit);
		else
			o &= ~(1 << bit);
	}

	vector<int> generate_moves() const {
		vector<int> moves;

		for (int i = 0; i < 9; i++)
			if (((o >> i) & 1) == 0 &&
				((x >> i) & 1) == 0)
				moves.push_back(i);

		return moves;
	}
};

pair<int, string> search(State state, int depth = 0) {
	int result = state.is_terminal();

	if (result != 2) {
		return {result, ""};
	}

	pair<int, string> best = make_pair(-2, "");

	for (int move : state.generate_moves()) {
		state.make_move(move);

		int value = -search(state, depth + 1).first;

		if (best.first < value) {
			best.first = value;
			best.second = string(1, char(move + '1'));
		}
		else
		if (best.first == value) {
			best.second += char(move + '1');
		}

		state.undo_move(move);
	}

	return best;
}

int main() {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	State start; // BUG: brak wyzerowanych wartosci w strukcie
	start.read();

	auto [best_value, best_moves] = search(start);

	if (best_value == -1) {
		cout << "NIE\n";
	}
	else
	if (best_value == 0) {
		cout << "REMIS\n";
		cout << best_moves << '\n';
	}
	else
	if (best_value == +1) {
		cout << "TAK\n";
		cout << best_moves << '\n';
	}
	else {
		assert(false);
	}
	
	return 0;
}
