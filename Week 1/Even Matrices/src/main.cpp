///3
#include <iostream>
#include <vector>

using namespace std;

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

void solve() {
  // Declarations
  int n; cin>>n;
  VVI mat(n+1, VI(n+1, 0));
  VVI dp2(n+1, VI(n+1, 0));
  
  // Solve
  for (int i=1; i<=n; i++) {
    for (int j=1; j<=n; j++) {
      int k; cin>>k;
      mat[i][j] = k;
      
      dp2[i][j] = dp2[i][j-1] + dp2[i-1][j] - dp2[i-1][j-1] + k;
    }
  }
  
  // Print matrix
  // for (int i=0; i<n; i++) {
  //   for (int j=0; j<n; j++) {
  //     cout << dp[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // cout << endl;
  
  int count = 0;
  
  for (int i1 = 1; i1 <= n; i1++) {
    for (int i2 = i1; i2 <= n; i2++) {
      // Now reduce problem to even pairs
      VI v(n+1, 0);  // We do Even Pairs on array S.
      VI dp1(n+1, 0); // pS contains partial sums of S.
      
      for (int j = 1; j <= n; j++) {
        int diff = dp2[i2][j-1] + dp2[i1-1][j] - dp2[i1-1][j-1];
        v[j] = dp2[i2][j] - diff;
        dp1[j] = dp1[j-1] + v[j];
      }
      
      int evens = 0;
      int odds = 0;
      for (int j = 1; j <= n; ++j){
        if (dp1[j] % 2 == 0)
            evens++;
        else
            odds++;
      }
      
      count += evens * (evens - 1) / 2 + odds * (odds - 1) / 2 + evens;
    }
  }
  
  cout << count << endl;
}

int main() {
  // Run tests
  int nr; cin>>nr;
  for (int i = 0; i < nr; i++) {
    solve();
  }
}