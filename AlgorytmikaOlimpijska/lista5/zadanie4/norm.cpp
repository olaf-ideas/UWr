#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
	ios::sync_with_stdio(false), cin.tie(nullptr);

	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <file>\n";
		return 1;
	}

	string filename = argv[1];
	fstream input(filename);

	if (!input.is_open()) {
		cout << "Cannot open: '" << filename << "'\n";
		return 1;
	}

	bool empty = true;

	string line;
	while (getline(input, line)) {

		string norm_line;

		int i = 0;
		while (i < (int) line.size()) {
			char ch = line[i++];

			if (isspace(ch))
				continue;

			norm_line += ch;
			
			bool space = false;
			while (i < (int) line.size() &&
				   isspace(line[i])) {
				space = true;
				i++;
			}

			if (space)
				norm_line += ' ';
		}

		line = norm_line;

		while (line.size() && isspace(line.back()))
			line.pop_back();

		if (line.empty())
			continue;

		empty = false;
		cout << line << '\n';
	}

	if (empty)
		cout << '\n';

	return 0;
}
