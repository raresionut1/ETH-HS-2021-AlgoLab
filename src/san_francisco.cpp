///2
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void solve() {
  int n; cin >> n; // nr of nodes
  int m; cin >> m; // nr of edges
  long x; cin >> x; // score to beat
  int k; cin >> k; // turns to beat
  
  vector<bool> ending(n, true);
  
  vector<int> edges_u(m);
  vector<int> edges_v(m);
  vector<int> edges_w(m);
  
  // read edges
  for (int i = 0; i < m; i++) {
    int u; cin >> u;
    int v; cin >> v;
    int w; cin >> w;
    
    ending[u] = false;
    edges_u[i] = u;
    edges_v[i] = v;
    edges_w[i] = w;
  }
  
  // adjust v for edges
  for (int i = 0; i < m; i++) {
    int v = edges_v[i];
    if (ending[v])
      edges_v[i] = 0;
  }
  
  // Bottom-Up DP
  vector<long> best(n, 0);
  
  for (int turn = 1; turn <= k; turn++) {
    vector<long> next_best(n, 0);
    
    for (int i = 0; i < m; i++) {
      int u = edges_u[i];
      int v = edges_v[i];
      int w = edges_w[i];
      
      next_best[u] = max(next_best[u], w + best[v]);
    }
    
    best = next_best;
    
    if (best[0] >= x) {
      cout << turn << endl;
      return;
    }
  }
  
  cout << "Impossible" << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t--)
    solve();
}  