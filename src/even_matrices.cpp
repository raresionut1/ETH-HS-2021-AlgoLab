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

  int count = 0;

  // read input
  for (int i=1; i<=n; i++)
  for (int j=1; j<=n; j++)
    cin >> mat[i][j];
  
  // Solve
  for (int k=1; k<=n; k++) {
  VVI dp(n + 1, VI(n + 1, 0));
  for (int i=k; i<=n; i++)
    for (int j=k; j<=n; j++)
      dp[i][j] = dp[i][j-1] + dp[i-1][j] - dp[i-1][j-1] + mat[i][j];

  for (int i=k; i<=n; i++) {
    // EVEN PAIRS (for rows and columns)
    int even1 = 0; int odd1 = 0;
    int even2 = 0; int odd2 = 0;

    for (int j=k; j<=n; j++) {
      // ROW
      if (dp[i][j] % 2 == 0)
        even1++;
      else
        odd1++;

      // COLUMN
      if (dp[j][i] % 2 == 0)
        even2++;
      else
        odd2++;
    }

    count += even1 * (even1 + 1) / 2 + odd1 * (odd1 - 1) / 2;
    count += even2 * (even2 - 1) / 2 + odd2 * (odd2 - 1) / 2;
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