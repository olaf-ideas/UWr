#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <cassert>
#include <algorithm>
#include <sstream>

class DeductionSolver {

private:

	int n, m;
	std::vector<std::vector<int>> rows_cons;
	std::vector<std::vector<int>> cols_cons;

	std::vector<std::vector<int>> rows;
	std::vector<std::vector<int>> cols;

	std::queue<int> Q;

	bool Inference(std::vector<int>, std::vector<int>&);

public:

	DeductionSolver(int,
					int,
					const std::vector<std::vector<int>>&,
					const std::vector<std::vector<int>>&);

	bool Solve();

	std::string ToString() const;
};

DeductionSolver::DeductionSolver(int _n, 
								 int _m, 
								 const std::vector<std::vector<int>>& _rows_cons,
								 const std::vector<std::vector<int>>& _cols_cons) :
	n(_n), m(_m), rows_cons(_rows_cons), cols_cons(_cols_cons),
	rows(n, std::vector<int>(m, 0)), cols(m, std::vector<int>(n, 0)) {
	
}

// For purpose of testing
std::vector<int> BruteInference(const std::vector<int>& cons, const std::vector<int>& line) {
	const int N = (int) line.size();

	std::vector<int> possible(N);
	int all = 0;

	for (int mask = 0; mask < (1 << N); mask++) {
		bool match = true;

		std::vector<int> mask_cons;
		int count = 0;

		for (int i = 0; i < N; i++) {
			int bit = (mask >> i) & 1;

			if (bit == 1) {
				count++;
			}
			else
			if (count > 0) {
				mask_cons.push_back(count);
				count = 0;
			}

			if (line[i] == +1 && bit == 0)
				match = false;

			if (line[i] == -1 && bit == 1)
				match = false;
		}

		if (count > 0) {
			mask_cons.push_back(count);
		}

		if (match == false)
			continue;

		if (mask_cons == cons) {
			for (int i = 0; i < N; i++) {
				int bit = (mask >> i) & 1;

				possible[i] += bit;
			}
			all++;
		}
	}

	if (all == 0) {
		assert(false); // your previous inferences does not match
	}

	std::vector<int> ans = line;
	for (int i = 0; i < N; i++) {
		if (possible[i] == 0) {
			ans[i] = -1;
		}
		else
		if (possible[i] == all)
			ans[i] = +1;
	}

	return ans;
}

bool DeductionSolver::Inference(std::vector<int> cons, std::vector<int>& line) {

	// auto brute_result = BruteInference(cons, line);

	/*
	std::cerr << "Inference(";
	for (int x : cons)
		std::cerr << x << ',';
	std::cerr << " | ";
	for (int x : line)
		std::cerr << x << ',';
	std::cerr << ")\n";
	*/

	const int N = (int) line.size();
	const int M = (int) cons.size();

	std::vector dp(2, std::vector<std::vector<bool>>(N + 1, std::vector<bool>(M + 1)));
	
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

	/*
	for (int rep : {0, 1}) {
		std::cerr << "REP: " << rep << '\n';
		for (int i = 0; i <= N; i++) {
			for (int j = 0; j <= M; j++) {
				std::cerr << dp[rep][i][j];
			}
			std::cerr << "\n";
		}
		std::cerr << "\n";
	}
	*/

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
	
	/*
	std::cerr << "GoodPrefix:\n";
	for (int i = 0; i < M; i++) {
		int c = cons[i];
		for (int j = 0; j + c - 1 < N; j++) {
			std::cerr << GoodPrefix(j - 1, i) << ' ';
		}
		std::cerr << '\n';
	}
	std::cerr << '\n';

	std::cerr << "GoodSuffix:\n";
	for (int i = 0; i < M; i++) {
		int c = cons[i];
		for (int j = 0; j + c - 1 < N; j++) {
			std::cerr << GoodSuffix(N - j - c - 1, M - 1 - i) << ' ';
		}
		std::cerr << '\n';
	}
	std::cerr << '\n';

	std::cerr << "Good Prefix & Suffix:\n";
	for (int i = 0; i < M; i++) {
		int c = cons[i];
		for (int j = 0; j + c - 1 < N; j++) {
			std::cerr << (GoodPrefix(j - 1, i) && GoodSuffix(N - j - c - 1, M - 1 - i)) << ' ';
		}
		std::cerr << '\n';
	}
	std::cerr << '\n';
	*/

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

		/*
		std::cerr << i << " all: " << all << " possible: ";
		for (int x : possible)
			std::cerr << x << ' ';
		std::cerr << '\n';
		*/

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
		std::cerr << "Brute result: ";
		for (int x : brute_result)
			std::cerr << x << ' ';
		std::cerr << '\n';
		std::cerr << "My    result: ";
		for (int x : line)
			std::cerr << x << ' ';
		std::cerr << '\n';

		assert(false);
	}
	*/

	return true;
}

bool DeductionSolver::Solve() {
	for (int i = 0; i < n + m; i++)
		Q.push(i);
	
	while (!Q.empty()) {
		int u = Q.front();
		Q.pop();

		// std::cerr << "Board:\n" << ToString() << '\n';

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

std::string DeductionSolver::ToString() const {
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
	*/

	DeductionSolver solver(n, m, rows_cons, cols_cons);

	assert(solver.Solve());
	
	output << solver.ToString() << '\n';

	return 0;
}
