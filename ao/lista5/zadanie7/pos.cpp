#include <iostream>

using namespace std;

int n, a, b;
int l, r;

int main() {
	cin >> n >> a >> b;
	
	l = 1; r = n;
	
	// Szuka Bajtyny
	while(l < r) {
		int p = (l + r) / 2;
		
		cout << "W " << p << endl;
		string res;
		cin >> res;
		
		if(res == "tak") l = p + 1; else r = p;
	}
	
	// Udziela odpowiedzi
	cout << "! " << l << endl;
	
	return 0;
}
