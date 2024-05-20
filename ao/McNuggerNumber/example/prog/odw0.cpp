#include <bits/stdc++.h>
using namespace std;

string s;
int p = 0;

struct Node {
  bool reversed;
  char ch;
  deque<Node*> children;

  Node(bool reversed, char ch):
    reversed(reversed),
    ch(ch) {}

  void read() {
    while (true) {
      char x = (p >= s.length()) ? '-' : s[p];
      p++;

      if (x == '(') {
        if (!reversed) {
          children.push_back(new Node(!reversed, '-'));
          children.back()->read();
        }
        else {
          children.push_front(new Node(!reversed, '-'));
          children.front()->read();
        }
      }
      else if ((x == '-') || (x == ')')) {
        break;
      }
      else {
        if (!reversed)
          children.push_back(new Node(!reversed, x));
        else
          children.push_front(new Node(!reversed, x));
      }
    }
  }

  void print() {
    for (Node* x : children) {
      if (x->ch != '-')
        cout << x->ch;
      x->print();
    }
  }
};

int main() {
  cin >> s;

  Node* root = new Node(false, '-');
  root->read();
  root->print();
  cout << "\n";

  return 0;
}
