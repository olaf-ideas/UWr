#include <stdio.h>

#define N 1000000
#define X 10000001

struct Node {
	Node* l;
	Node* r;

	int rank;

	int x, y, i;
} pool[N];

bool cmp(int x1, int y1, int x2, int y2) {
	return y1 < y2 || (y1 == y2 && x1 < x2);
}

void split(Node* u, int x, int y, Node* &l, Node* &r) {
	if (!u)
		return void(l = r = 0);

	if (!cmp(u->x, u->y, x, y))
		split(u->l, x, y, l, u->l), r = u;
	else
		split(u->r, x, y, u->r, r), l = u;
}

Node* merge(Node* l, Node* r) {
	if (!l || !r)
		return l ? l : r;
	if (l->rank > r->rank)
		return l->r = merge(l->r, r);
	else
		return r->l = merge(l, r->l);
}

int n, x[N], y[N];

long long ans = 1e18;
int p = -1, q = -1;

void update_ans(Node* u, int j) {
	if (!u)
		return;

	int dx = u->x - x[j];
	int dy = u->y - y[j];
	long long dist = (long long) dx * dx + (long long) dy * dy;

	if (ans > dist) {
		ans = dist;
		p = u->i;
		q = j;
	}

	update_ans(u->l, j);
	update_ans(u->r, j);
}

int order[N], tmp[N];
void sort(int l, int r) {
	if (l + 1 == r)
		return;

	int m = (l + r) >> 1;
	sort(l, m);
	sort(m, r);
}

int main() {
	
	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		scanf("%d%d", x + i, y + i);
	}

	sort(0, n);
	
	Node* root;

	Node *l, *r, *l2, *r2;
	for (int i = 0, j = 0; i < n; i++) {
		int d = (int)ceil(sqrt(ans));
		while (x[i] - x[j] >= d) {
			split(root, x[j], y[j], l, r);
			split(r, x[j], y[j] + 1, l2, r2);
			root = merge(l, r2);
			j++;
		}

		Node *l, *r, *l2, *r2;
		split(root, -X, y[i] - d, l, r);
		split(r, +X, y[i] + d, l2, r2);
		root = merge(merge(l, l2), r2);

		update_ans(l2, i);
		
		split(root, x[i], y[i], l, r);
		
		Node* u = pool[i];
		u->rank = rand();
		u->x = x[i];
		u->y = y[i];
		u->i = i;

		root = merge(l, merge(get_node(i), r));
	}

	printf("%d %d\n%d %d\n", x[p], y[p], x[q], y[q]);
	
	return 0;
}
