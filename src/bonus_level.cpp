#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> VI;
typedef vector<vector<int>> VVI;
typedef vector<vector<vector<int>>> VVVI;

int dp(int d, int x1, int x2, int n, VVI &mat, VVVI &memo) {
  if (d >= 2*n-1 || x1 >= n || x2 >= n || d - x1 >= n || d - x2 >= n)
    return 0;
    
  if (memo[d][x1][x2] != -1)
    return memo[d][x1][x2];
    
  int val = x1 != x2
             ? mat[d - x1][x1] + mat[d - x2][x2]
             : mat[d - x1][x1];
             
  int reward = val + dp(d+1, x1, x2, n, mat, memo);
  reward = max(reward, val + dp(d+1, x1+1, x2, n, mat, memo));
  reward = max(reward, val + dp(d+1, x1, x2+1, n, mat, memo));
  reward = max(reward, val + dp(d+1, x1+1, x2+1, n, mat, memo));
  
  memo[d][x1][x2] = reward;
  
  return reward;
}

void solve() {
  int n; cin >> n;
  
  VVI mat(n, VI(n));
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      cin >> mat[i][j];
      
  VVVI memo(2*n-1, VVI(n, VI(n, -1)));
  int res = dp(0, 0, 0, n, mat, memo);
  cout << res << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while (t--)
    solve();
}