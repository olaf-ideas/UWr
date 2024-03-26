/*
	Połączenie A* i heurystyk z Zadania 2.

	Co 50k krokow A* biore najlepsza sciezke minimalizujaca 
	niepewnosci i dodaje do odpowiedzi. Zapuszczam jeszcze raz
	ten sam algorytm co 50k doputy nie znajde odpowiedzi.
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
vector<pair<int, int>> konce;

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

int admissible_heuristic(vector<pair<int, int>> komandosi) {
	int farthest = 0;

	for (auto [x, y] : komandosi) {
		int best = 1000;
		for (auto [p, q] : konce) {
			best = min(best, dist[x][y][p][q]);
		}

		farthest = max(farthest, best);
	}

	return farthest;
}

const int INADMISSIBILITY_DEGREE = 3;

int heuristic(vector<pair<int, int>> komandosi) {

	int heura = admissible_heuristic(komandosi);
	
	for (auto [x, y] : komandosi) {
		int best = 1000;
		int xx = -1;
		int yy = -1;

		for (auto [p, q] : konce) {
			if (best > dist[x][y][p][q]) {
				best = dist[x][y][p][q];

				xx = p;
				yy = q;
			}
		}

		heura += abs(x - xx) + abs(y - yy);
	}

	return min(heura, admissible_heuristic(komandosi) + 
					   INADMISSIBILITY_DEGREE);
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
		for (int j = 0; j < m; j++) {
			if (board[i][j] == 'S' || board[i][j] == 'B')
				komandosi.emplace_back(i, j);

			if (board[i][j] == 'G' || board[i][j] == 'B')
				konce.emplace_back(i, j);
		}

	string answer;

	typedef pair<int, vector<pair<int, int>>> State;
	
	while (true) {

		// debug(komandosi);

		set<State> pq;

		map<vector<pair<int, int>>, int> distance;
		map<vector<pair<int, int>>, string> visited;

		visited[komandosi] = "";
		pq.emplace(make_pair(0 + heuristic(komandosi), komandosi));
		distance[komandosi] = 0;

		for (int rep = 0; rep < 50000 && pq.size(); rep++) {
			auto [d, u] = *pq.begin();
			pq.erase(pq.begin());

			if (distance[u] + heuristic(u) != d)
				continue;

			if (is_terminal(u)) {
				answer += visited[u];

				output << answer << '\n';
				
				return 0;
			}

			for (int dir = 0; dir < 4; dir++) {
				auto v = make_move(u, dir);

				int dd = distance[u] + 1;

				if (visited.count(v) == 0 ||
					distance[v] > dd) {
					visited[v] = visited[u] + "LURD"[dir];
					distance[v] = dd;

					pq.emplace(dd + heuristic(v), v);
				}
			}
		}

		string best_move = "";
		for (auto [v, moves] : visited) {
			if (komandosi.size() > v.size() ||
			   (komandosi.size() == v.size() && best_move.size() > moves.size())) {
				komandosi = v;
				best_move = moves;
			}
		}

		answer += best_move;
	}

	cerr << "Failed\n";

	return 0;
}
