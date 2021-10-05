///1
#include <iostream>
#include <vector>

using namespace std;

void solve() {
  // Declarations
  int n; cin>>n;
  vector<int> dp;
  int s = 0;
  int evens = 0;
  int odds = 0;
  
  // Solve
  for (int i=0; i<n; i++) {
    int k; cin>>k;
    s += k;
    dp.push_back(s);
  }
  
  for (int i=0; i<n; i++) {
    if (dp[i] % 2 == 0)
      evens++;
    else
      odds++;
  }
  
  int res = ((evens * (evens - 1) / 2) + (odds * (odds - 1) / 2) + evens);
  
  cout << res << endl;
}

int main() {
  // Run tests
  int nr; cin>>nr;
  for (int i = 0; i < nr; i++) {
    solve();
  }
}