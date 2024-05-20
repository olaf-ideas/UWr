#include <iostream>

using namespace std;

const int MAXR = 300000;

int n, a, b, dp[MAXR];

int dlg(int x) {
	int r = 0;
	while(dp[r] < x)	r++;
	r--;
	return r;
}

int main(){
	cin >> n >> a >> b;
	
	for(int i = 0; i < max(a,b); i++)		dp[i] = 1;
	for(int i = max(a,b); i < MAXR; i++)	dp[i] = dp[i-a] + dp[i-b];

	int l = 1, r = n+1, x = 0;
	while(r-l > 1){
		x = dlg(r-l+1);

		cout << "W " << l + dp[x - a] << endl;
		string res;
		cin >> res;

		if (res == "tak")	l = l+dp[x-a]+1;
		else				r = l+dp[x-a];
	}

	cout << "! " << l << endl;
}
