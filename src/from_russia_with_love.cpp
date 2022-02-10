#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

int score(int n, int m, int k, int i, int j, VI &v, VVI &memo) {
  // Check if it is first round or not
  int turns_passed = i == 0 && j == (n - 1) ? k : m - 1;
  
  // No more coins left for person k => stop
  if (i > j - turns_passed)
    return 0;
    
  // Check if the current state is cached
  if (memo[i][j] != -1)
    return memo[i][j];
    
  int best_score = -1;
    
  for (int x = 0; x <= turns_passed; x++) {
    int i_new = i + x;
    int j_new = j - (turns_passed - x);
    
    // Compute local best score
    int best_curr_score = max(v[i_new] + score(n, m, k, i_new + 1, j_new, v, memo),
                              v[j_new] + score(n, m, k, i_new, j_new - 1, v, memo));
           
    // Compute global best score                   
    best_score = best_score == -1 
      ? best_curr_score
      : min(best_score, best_curr_score);
  }
  
  memo[i][j] = best_score;
  
  return memo[i][j];
}

void solve() {
  // Declarations
  int n; cin>>n; // nr of sovereigns
  int m; cin>>m; // nr of passangers
  int k; cin>>k; // index of the passanger
  VI v(n, 0); // transitions
  
  // Read transitions
  for (int i=0; i<n; i++) {
    int vi; cin>>vi;
    v[i] = vi;
  }
  
  VVI memo(n, VI(n, -1));
  
  cout << score(n, m, k, 0, n - 1, v, memo) << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  // Run tests
  int nr; cin>>nr;
  for (int i = 0; i < nr; i++) {
    solve();
  }
} 