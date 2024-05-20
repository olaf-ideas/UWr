#include "poslib.h"

int max(int a, int b){return a>b?a:b;}

const int MAXR = 300000;

int n, a, b, dp[MAXR];

int main(){
	inicjuj(&n, &a, &b);
	
	for(int i = 0; i < max(a,b); i++)		dp[i] = 1;
	for(int i = max(a,b); i < MAXR; i++)	dp[i] = dp[i-a] + dp[i-b];

	int l = 1, r = n, dlg = 0;

	while(dp[dlg] < n)	dlg++;

	while(l < r){
		while(dp[dlg] >= r-l+1)	dlg--;
		dlg++;

		if(pytaj('N',l+dp[dlg-a]))	r = l+dp[dlg-a]-1, dlg -= a;
		else						l = l+dp[dlg-a],   dlg -= b;
	}
	odpowiedz(l);
}
