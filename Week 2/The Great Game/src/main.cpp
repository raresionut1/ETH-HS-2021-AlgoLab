///1
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

#define INF std::numeric_limits<int>::max()
#define NEG_INF std::numeric_limits<int>::min()

typedef vector<int> VI;
typedef vector<VI> VVI;

int moves_to_target(int u, int n, bool good, VVI &t, VI &memo_good, VI &memo_bad) {
  if (u == n)
    return 0;
    
  if (t[u].empty())
    return INF; 
  
  if (good) {
    // Case when the move is good
    // Sherlock - Red
    // Moriarty - Black
    if (memo_good[u] == -1) {
      int nr_moves = INF;
      for (int v : t[u]) {
        nr_moves = min(nr_moves, 1 + moves_to_target(v, n, !good, t, memo_good, memo_bad));
      }
      memo_good[u] = nr_moves;
    }
    return memo_good[u];
    
  } else {
    // Case when the move is bad
    // Sherlock - Black
    // Moriarty - Red
    if (memo_bad[u] == -1) {
      int nr_moves = NEG_INF;
      for (int v : t[u]) {
        nr_moves = max(nr_moves, 1 + moves_to_target(v, n, !good, t, memo_good, memo_bad));
      }
      memo_bad[u] = nr_moves;
    }
    return memo_bad[u];
  }
}

void solve() {
  // Declarations
  int n; cin>>n; // nr of positions
  int m; cin>>m; // nr of transitions
  int r; cin>>r; // position of red meeple
  int b; cin>>b; // position of black meeple
  VVI t(n+1, VI(0, 0)); // transitions
  
  // Read transitions
  for (int i=0; i<m; i++) {
    int u; cin>>u;
    int v; cin>> v;
    t[u].push_back(v);
  }
  
  VI memo_good(n+1, -1);
  VI memo_bad(n+1, -1);
  
  // Simulate game to get shortest moves for both red and black meeples
  moves_to_target(r, n, true, t, memo_good, memo_bad);
  moves_to_target(b, n, true, t, memo_good, memo_bad);
  
  // Case when the red meeple can get to the target faster
  if (memo_good[r] < memo_good[b]) {
    cout << 0 << endl;
    return;
  }
  
  // Case when both red and black meeples can get to thr target
  // using the same number of moves
  // => Because the moves are SR, MB, SB, MR, if Sherlock can get the R (Red)
  // meeple to the target in an odd number of moves => he wins
  if(memo_good[r] == memo_good[b]) {
    cout << (memo_good[r] % 2 == 1 ? 0 : 1) << endl;
    return;
  }
        
  // Case when the black meeple can get to the target faster
  cout << 1 << endl;
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