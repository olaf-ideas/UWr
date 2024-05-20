/*
 	TODO
*/

#include <bits/stdc++.h>

using namespace std;

mt19937 rng(2342432);

int randint(int a, int b) {
	return uniform_int_distribution<int>(a, b)(rng);
}

const int N = 50;

ifstream input("zad_input.txt");
ofstream output("zad_output.txt");

int n, m;
string board[N];

int dist[N][N][N][N];

const int dx[4] = {0, -1, 0, +1};
const int dy[4] = {-1, 0, +1, 0};

void build_dist() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (board[i][j] == '#')
				continue;

			vector<pair<int, int>> que;
			que.emplace_back(make_pair(i, j));

			dist[i][j][i][j] = 1;

			for (int z = 0; z < (int) que.size(); z++) {
				int x = que[z].first;
				int y = que[z].second;
				
				for (int dir = 0; dir < 4; dir++) {
					int nx = x + dx[dir];
					int ny = y + dy[dir];

					if (board[nx][ny] != '#' &&
						dist[i][j][nx][ny] == 0) {
						dist[i][j][nx][ny] = dist[i][j][x][y] + 1;
						que.emplace_back(nx, ny);
					}
				}
			}
		}
	}
}

bool is_terminal(const vector<pair<int, int>> &komandosi) {
	for (auto [x, y] : komandosi)
		if (board[x][y] != 'G' && board[x][y] != 'B')
			return false;

	return true;
}

vector<pair<int, int>> make_move(const vector<pair<int, int>> &komandosi, int dir) {
	vector<pair<int, int>> result;
	for (auto [x, y] : komandosi) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];

		if (board[nx][ny] == '#') {
			result.emplace_back(x, y);
		}
		else {
			result.emplace_back(nx, ny);
		}
	}

	sort(result.begin(), result.end());
	result.erase(unique(result.begin(), result.end()), result.end());
	
	return result;
}

int niepewnosc(vector<pair<int, int>> komandosi, bool debug = false) {
	int result = komandosi.size();

	for (int i = 0; i < (int) komandosi.size(); i++) {
		
		auto [xx, yy] = komandosi[i];

		int best = 1000, xxx = -1, yyy = -1;
		for (int x = 0; x < n; x++) {
			for (int y = 0; y < m; y++) {
				if (board[x][y] == 'B' || board[x][y] == 'G') {
					if (best > dist[xx][yy][x][y]) {
						best = dist[xx][yy][x][y];
						xxx = x;
						yyy = y;
					}
				}
			}
		}

		if (debug) {
			cerr << "<" << xx << ',' << yy << "> = " << best << '\n';
			cerr << "   $ " << xxx << ' ' << yyy << '\n';
		}

		result += best;

		for (int j = i + 1; j < (int) komandosi.size(); j++) {
			auto [x1, y1] = komandosi[i];
			auto [x2, y2] = komandosi[j];

			result += dist[x1][y1][x2][y2] / 5;
		}
	}

	return result;
}

void debug(vector<pair<int, int>> pts) {
	cerr << "pts: ";
	for (auto [x, y] : pts)
		cerr << "[" << x << ',' << y << "], ";
	cerr << '\n';

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			bool inside = false;
			for (auto [x, y] : pts) {
				if (i == x && y == j)
					inside = true;
			}

			if (inside)
				cout << 'S';
			else
			if (board[i][j] != 'S')
				cout << board[i][j];
			else
				cout << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
}

int main() {
	
	while (getline(input, board[n]))
		n++;
	
	m = (int) board[0].size();
	
	build_dist();
	
	vector<pair<int, int>> komandosi;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (board[i][j] == 'S')
				komandosi.emplace_back(i, j);

	string answer;

	// /*

	// redukcja stanu
	for (int dir = 0; dir < 4; dir++) {
		for (int rep = 0; rep < 15; rep++) {
			answer += "LURD"[dir];

			komandosi = make_move(komandosi, dir);
		}
	}
	// */

	// debug(komandosi);

	// return 0;

	while (true) {

		// niepewnosc(komandosi, true);
		// debug(komandosi);

		vector<vector<pair<int, int>>> que {komandosi};
		map<vector<pair<int, int>>, string> visited;
		
		visited[komandosi] = "";

		for (int rep = 0; rep < (int) que.size() && rep < 50000; rep++) {
			auto u = que[rep];

			if (is_terminal(u)) {
				answer += visited[u];
				
				output << answer << '\n';

				return 0;
			}
		
			for (int dir = 0; dir < 4; dir++) {
				auto v = make_move(u, dir);

				if (visited.count(v) == 0) {
					visited[v] = visited[u] + "LURD"[dir];
					que.push_back(v);
				}
			}
		}

		string best_moves;

		for (auto [v, res] : visited) {
			if (komandosi.size() > v.size() ||
			   (komandosi.size() == v.size() &&
			    niepewnosc(komandosi) > niepewnosc(v))) {
				komandosi = v;
				best_moves = res;
			}
		}


		answer += best_moves;
	}

	return 0;
}
