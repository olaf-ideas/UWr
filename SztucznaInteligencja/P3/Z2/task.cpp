#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <random>

class Solver {

private:

	int n, m;
	std::vector<std::vector<int>> rows_cons;
	std::vector<std::vector<int>> cols_cons;

	std::vector<std::vector<int>> rows;
	std::vector<std::vector<int>> cols;

	std::queue<int> Q;

public:

	bool Inference(std::vector<int>, std::vector<int>&);
	
	Solver(int,
		   int,
		   const std::vector<std::vector<int>>&,
		   const std::vector<std::vector<int>>&);

	bool Deduce();
	bool Backtrack(int, int);
	bool Solve();

	std::string ToString() const;
};

Solver::Solver(int _n, 
			   int _m, 
			   const std::vector<std::vector<int>>& _rows_cons,
			   const std::vector<std::vector<int>>& _cols_cons) :
	n(_n), m(_m), rows_cons(_rows_cons), cols_cons(_cols_cons),
	rows(n, std::vector<int>(m, 0)), cols(m, std::vector<int>(n, 0)) {
	
}

bool Solver::Inference(std::vector<int> cons, std::vector<int>& line) {

	const int N = (int) line.size();
	const int M = (int) cons.size();

	std::vector dp(2, std::vector<std::vector<bool>>(N + 1, std::vector<bool>(M + 1)));
	// dp[0][i][j] -> czy da sie ustawic j pierwszych przedzialow cons na prefixie line[0..(i-1)]
	// dp[1][i][j] -> -||-, ale dla odwroconych cons i line
	
	// to rozwiazanie nie wywnioskuje dla kazdego wiersza poprawnego rozwiazania, ale
	// dla prostego wnioskowania wystarczy

	for (int rep : {0, 1}) {
		
		dp[rep][0][0] = true;

		for (int i = 1; i <= N; i++) {
			if (line[i - 1] == +1)
				dp[rep][i][0] = false;
			else
				dp[rep][i][0] = dp[rep][i - 1][0];
		}
		
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= M; j++) {
				if (line[i - 1] != +1 && dp[rep][i - 1][j]) {
					dp[rep][i][j] = true;
				}
				
				int c = cons[j - 1];
				if (i - c - (j == 1 ? 0 : 1) < 0) {
					continue;
				}

				bool possible = true;

				if (j != 1 && line[i - c - 1] == +1) {
					possible = false;
				}

				for (int k = i - c; k < i; k++) {
					if (line[k] == -1) {
						possible = false;
					}
				}

				if (possible && dp[rep][i - c - (j == 1 ? 0 : 1)][j - 1]) {
					dp[rep][i][j] = true;
				}
			}
		}

		std::reverse(cons.begin(), cons.end());
		std::reverse(line.begin(), line.end());
	}

	auto IsEmpty = [&](int l, int r) {
		if (l - 1 >= 0 && line[l - 1] == +1)
			return false;

		if (r + 1 < N && line[r + 1] == +1)
			return false;

		for (int i = l; i <= r; i++)
			if (line[i] == -1)
				return false;

		return true;
	};

	auto Apply = [&](int &old, int now) {
		if (old != 0 && old != now)
			return false;

		old = now;
		return true;
	};

	std::vector<bool> is_empty(N, true);

	auto GoodPrefix = [&](int i, int j) -> bool {
		if (i <= 0)	return j <= 0;
		return dp[0][i][j];
	};

	auto GoodSuffix = [&](int i, int j) -> bool {
		if (i <= 0)	return j <= 0;
		return dp[1][i][j];
	};

	for (int i = 0; i < M; i++) {
		int c = cons[i];

		std::vector<int> possible(N);
		int all = 0;

		for (int j = 0; j + c - 1 < N; j++) {
			if (GoodPrefix(j - 1, i) && 
				GoodSuffix(N - j - c - 1, M - 1 - i) &&
				IsEmpty(j, j + c - 1)) {
				for (int k = 0; k < c; k++) {
					possible[j + k]++;
				}
				all++;
			}
		}

		for (int j = 0; j < N; j++) {
			if (possible[j] > 0) {
				is_empty[j] = false;
			}

			if (possible[j] == all && 
				!Apply(line[j], +1)) {
				return false;
			}
		}

		if (all == 0) {
			return false;
		}
	}

	for (int i = 0; i < N; i++) {
		if (is_empty[i] && 
			!Apply(line[i], -1)) {
			return false;
		}
	}
	
	/*
	if (line != brute_result) {
		std::cerr << "cons:\n";
		for (int x : cons)
			std::cerr << x << ' ';
		std::cerr << '\n';

		std::cerr << "start:\n";
		for (int x : start)
			std::cerr << x << ' ';
		std::cerr << '\n';

		std::cerr << "line:\n";
		for (int x : line)
			std::cerr << x << ' ';
		std::cerr << '\n';

		std::cerr << "brute:\n";
		for (int x : brute_result)
			std::cerr << x << ' ';
		std::cerr << '\n';

		std::cerr << "brute_failed: " << brute_failed << '\n';

		assert(false);
	}
	*/

	return true;
}

bool Solver::Deduce() {
	while (!Q.empty()) {
		int u = Q.front();
		Q.pop();

		if (u < n) {
			// row
			if (!Inference(rows_cons[u], rows[u])) {
				return false;
			}

			for (int j = 0; j < m; j++) {
				if (cols[j][u] != rows[u][j]) {
					cols[j][u] = rows[u][j];
					Q.push(j + n);
				}
			}
		}
		else {
			u -= n;
			// column
			if (!Inference(cols_cons[u], cols[u])) {
				return false;
			}

			for (int i = 0; i < n; i++) {
				if (rows[i][u] != cols[u][i]) {
					rows[i][u] = cols[u][i];
					Q.push(i);
				}
			}
		}
	}

	return true;
}

bool Solver::Backtrack(int x, int y) {
	if (!Deduce()) {
		return false;
	}

	if (y == m) {
		return Backtrack(x + 1, 0);
	}

	if (x == n) {
		return true;
	}

	if (rows[x][y] != 0) {
		return Backtrack(x, y + 1);
	}

	auto old_rows = rows;
	auto old_cols = cols;
	
	for (int val : {-1, +1}) {
		rows[x][y] = val;
		cols[y][x] = val;

		// std::cerr << "Guessing: " << x << ' ' << y << " => " << val << '\n';
		// std::cerr << ToString() << '\n';

		Q.push(x);
		Q.push(y + n);

		if (Backtrack(x, y + 1)) {
			return true;
		}

		rows = old_rows;
		cols = old_cols;
	}

	return false;
}

bool Solver::Solve() {
	for (int i = 0; i < n + m; i++)
		Q.push(i);
	
	return Backtrack(0, 0);
}

std::string Solver::ToString() const {
	std::string res;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (rows[i][j] == +1) {
				res += '#';
			}
			else
			if (rows[i][j] == -1) {
				res += '.';
			}
			else
			if (rows[i][j] == 0) {
				res += '?';
			}
			else {
				assert(false);
			}
		}

		res += '\n';
	}

	return res;
}

std::ifstream input("zad_input.txt");
std::ofstream output("zad_output.txt");

int main() {

	int n, m;
	input >> n >> m; input.ignore();

	std::vector<std::vector<int>> rows_cons(n);
	std::vector<std::vector<int>> cols_cons(m);

	for (auto &v : rows_cons) {
		std::string line;
		std::getline(input, line);
		std::istringstream ss(line);

		int x;
		while (ss >> x) {
			v.push_back(x);
		}
	}
	
	for (auto &v : cols_cons) {
		std::string line;
		std::getline(input, line);
		std::istringstream ss(line);

		int x;
		while (ss >> x) {
			v.push_back(x);
		}
	}

	/*
	std::cerr << "n: " << n << '\n';
	std::cerr << "m: " << m << '\n';
	
	for (auto v : rows_cons) {
		std::cerr << v.size() << " => ";
		for (int x : v)
			std::cerr << x << ' ';
		std::cerr << '\n';
	}

	for (auto v : cols_cons) {
		std::cerr << v.size() << " => ";
		for (int x : v)
			std::cerr << x << ' ';
		std::cerr << '\n';
	}
	// */

	Solver solver(n, m, rows_cons, cols_cons);

	/*
	while (true) {
		std::cerr << "HERE\n";

		int xx = rand() % 3 + 1;

		std::vector<int> cons;

		while (xx--)
			cons.push_back(rand() % 4 + 1);
		
		std::vector<int> line(rand() % 7 + 1);

		for (int &x : line)
			if (rand() % 2 == 0)
				x = 0;
			else
			if (rand() % 2 == 0)
				x = +1;
			else
				x = -1;

		solver.Inference(cons, line);
	}
	*/
	
	assert(solver.Solve());
	
	output << solver.ToString() << '\n';

	return 0;
}
