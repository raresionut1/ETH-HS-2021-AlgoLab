///1
#include <iostream>
#include <vector>

using namespace std;

void solve() {
  // Declarations
  int n; cin>>n;
  int s = 0;
  
  // Solve
  for (int i=0; i<n; i++) {
    int k; cin>>k;
    s += k;
  }
  
  cout << s << endl;
}

int main() {
  // Run tests
  int nr; cin>>nr;
  for (int i = 0; i < nr; i++) {
    solve();
  }
}