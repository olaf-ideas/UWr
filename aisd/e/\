#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node {
	Node *l;
	Node *r;

	int size;
	int rnk;
	int val;
	long long sum;
};

int size(Node *u) {
	return u ? u->size : 0;
}

void pull(Node *u) {
	u->sum = u->val;
	u->size = 1;
	if (u->l) {
		u->sum += u->l->sum;
		u->size += u->l->size;
	}
	if (u->r) {
		u->sum += u->r->sum;
		u->size += u->r->size;
	}
}

Node* merge(Node *l, Node *r) {
	if (!l || !r)
		return l ? l : r;
	if (l->rnk > r->rnk)
		return l->r = merge(l->r, r), pull(l), l;
	else
		return r->l = merge(l, r->l), pull(r), r;
}

void split(Node *u, int k, Node **l, Node **r) {
	if (!u) {
		*l = *r = 0;
		return;
	}
	if (size(u->l) >= k)
		split(u->l, k, l, &u->l), *r = u;
	else
		split(u->r, k - size(u->l) - 1, &u->r, r), *l = u;
	pull(u);
}

Node pool[1000000];
int n;

int main() {
	scanf("%d", &n);

	Node *nxt = pool;
	Node *root = 0;

	while (n--) {
		char op; int p;
		scanf(" %c %d", &op, &p);
		
		if (op == 'I') {
			int x;
			scanf("%d", &x);
			
			Node *u = nxt++;
			u->size = 1;
			u->rnk = rand();
			u->val = x;
			u->sum = x;
			
			Node *l, *r;
			split(root, p, &l, &r);
		
			root = merge(merge(l, u), r);
		}
		else
		if (op == 'D') {
			Node *l, *u, *r;
			split(root, p - 1, &l, &r);
			split(r, 1, &u, &r);
			root = merge(l, r);
		}
		else { // 'S'
			int q;
			scanf("%d", &q);

			Node *l, *m, *r;
			split(root, p - 1, &l, &r);
			split(r, q - p + 1, &m, &r);
			
			printf("%lld\n", m->sum);

			root = merge(merge(l, m), r);
		}
	}

	return 0;
}
