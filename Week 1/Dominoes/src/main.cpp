///1
#include <iostream>
#include <vector>

using namespace std;

void solve() {
  // Declarations
  int n; cin>>n; // number of dominoes
  int fall = 1; // position of the last fallen domino
  int ok = 1; // stop condition
  int count = 0;
  
  // Solve
  for (int i = 0; i < n; i++) { // i is the position of the current domino
    int h; cin>>h; // height of the current domino
    
    if (fall <= i)
      ok = 0;
      
    if (ok == 0)
      continue;
      
    count++;
    
    if (i + h > fall)
      fall = i + h;
  }
  
  cout << min(n, count) << endl;
}

int main() {
  // Run tests
  int nr; cin>>nr;
  for (int i = 0; i < nr; i++) {
    solve();
  }
}