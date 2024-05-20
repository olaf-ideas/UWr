#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Usage: " << argv[0] << " <jury> <participant>" << endl;
		return 1;
	}

	string jury_filename = argv[1];
	string part_filename = argv[2];

	ifstream jury_answer(jury_filename);
	ifstream part_answer(part_filename);

	if (!jury_answer.is_open()) {
		cout << "Cannot open: '" << jury_filename << endl;
		return 1;
	}

	if (!part_answer.is_open()) {
		cout << "Cannot open: '" << part_filename << endl;
		return 1;
	}

	auto gather_lines = [&](istream& input) {
		vector<vector<string>> lines;

		string line;
		while (getline(input, line)) {
			while (line.size() && isspace(line.back()))
				line.pop_back();

			string word;
			vector<string> words;

			for (char ch : line) {
				if (isspace(ch)) {
					if (word.size()) {
						words.push_back(word);
						word.clear();
					}
				}
				else {
					word += ch;
				}
			}

			if (word.size())
				words.push_back(word);

			lines.push_back(words);
		}

		while (lines.size() && lines.back().empty())
			lines.pop_back();

		return lines;
	};

	vector<vector<string>> jury_lines = gather_lines(jury_answer);
	vector<vector<string>> part_lines = gather_lines(part_answer);

	auto print_word = [&](const vector<vector<string>>& lines, int i, int j) {
		if (i >= (int) lines.size()) {
			cout << "" << '\n';
			return;
		}

		if (j >= (int) lines[i].size()) {
			cout << "" << '\n';
			return;
		}

		const string& word = lines[i][j];
		const int len = (int) word.size();

		if (len <= 30) {
			cout << word;
		}
		else {
			for (int a = 0; a < 15; a++)
				cout << word[a];
			cout << "...";
			for (int a = len - 15; a < len; a++)
				cout << word[a];
		}

		cout << '\n';
	};

	const int N = (int) jury_lines.size();
	const int M = (int) part_lines.size();

	for (int i = 0; i < max(N, M); i++) {
		if (i >= N || i >= M) {
			cout << "WRONG " << i << ' ' << 0 << '\n';
			print_word(jury_lines, i, 0);
			print_word(part_lines, i, 0);
			return 1;
		}

		const int n = (int) jury_lines[i].size();
		const int m = (int) part_lines[i].size();

		for (int j = 0; j < max(n, m); j++) {
			if (j >= n || j >= m) {
				cout << "WRONG " << i << ' ' << j << '\n';
				print_word(jury_lines, i, j);
				print_word(part_lines, i, j);
				return 1;
			}

			if (jury_lines[i][j] != part_lines[i][j]) {
				cout << "WRONG " << i << ' ' << j << '\n';
				print_word(jury_lines, i, j);
				print_word(part_lines, i, j);
				return 1;
			}
		}
	}

	cout << "OK\n";

	return 0;
}

