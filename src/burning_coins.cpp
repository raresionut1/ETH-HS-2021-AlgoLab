///3
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

int reward(VI &v, int i, int j, bool my_turn, VVI &memo) {
  if (i == j) 
    return my_turn ? v[i] : 0;
    
  if (memo[i][j] == -1)
    memo[i][j] = my_turn ? max (v[i] + reward(v, i+1, j, !my_turn, memo),
                                v[j] + reward(v, i, j-1, !my_turn, memo))
                         : min (reward(v, i+1, j, !my_turn, memo),
                                reward(v, i, j-1, !my_turn, memo));
                      
  return memo[i][j];
}

void solve() {
  // Declarations
  int n; cin>>n; // nr of coins
  VI v(n, -1); // coins value
  VVI memo(n, VI(n, -1));
  
  // Read coins
  for (int i=0; i<n; i++) {
    int vi; cin>>vi;
    v[i] = vi;
  }

  cout << reward(v, 0, n - 1, true, memo) << endl;
}

int main() {
  // Run tests
  int nr; cin>>nr;
  for (int i = 0; i < nr; i++) {
    solve();
  }
}