#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

#define INF INT_MAX

int n; // nr of cities (vertices)
vector<vector<int>> adj; // adjacent cities
vector<int> costs; // repairing costs (for vertices)
vector<vector<int>> memo; // memo table for DP

// last_repaired counter (tells what was the last city repaired)
// 0 --> this one
// 1 --> last one
// 2 --> second to last
long dp(int v, int last_repaired) {
  // stop condition --> we arrived at a destination
  if (adj[v].empty())
    return last_repaired == 1 ? 0 : costs[v];
  
  // check if result is already cached
  if (memo[v][last_repaired] != -1)
    return memo[v][last_repaired];
    
  long cost_sum = 0;
  switch (last_repaired) {
    // ========================================================
    // Case when we repair the current city
    case 0:
      // repair this city
      cost_sum = costs[v];
      
      for (int next : adj[v])
        cost_sum += min(dp(next, 0),
                        dp(next, 1));
                        
      break;
  
    // ========================================================
    // Case when we DON'T repair the current city
    // and the last city was repaired
    case 1:
      // don't repair this city
      cost_sum = 0;
      
      for (int next : adj[v])
        cost_sum += min(dp(next, 0),
                        dp(next, 2));
                        
      break;
      
    // ========================================================
    // Case when we DON'T repair the current city
    // and the last city was NOT repaired
    // => at least one of the next cities have to be repaired
    case 2:
      // don't repair this city
      cost_sum = 0;
      
      // we need to find the best decisions for next cities where at least
      // one repairs its streets
      long smallest_drawback = INF;
      
      for (int next : adj[v]) {
        long cost_w_repair = dp(next, 0);
        long cost_wo_repair = dp(next, 2);
        cost_sum += min(cost_w_repair, cost_wo_repair);
        smallest_drawback = min(smallest_drawback, cost_w_repair - cost_wo_repair);
      }
      
      if (smallest_drawback > 0)
        cost_sum += smallest_drawback;
  
      break;
  }
  
  memo[v][last_repaired] = cost_sum;
  return cost_sum;
}

void solve() {
  // ======================== READ INPUT ========================
  cin >> n;
  
  adj = vector<vector<int>>(n, vector<int>());
  for (int i = 0; i < n - 1; i++) {
    int v1; cin >> v1;
    int v2; cin >> v2;
    adj[v1].push_back(v2);
  }
  
  costs = vector<int>(n);
  for (int i = 0; i < n; i++) {
    cin >> costs[i];
  }
  
  // ======================== SOLVE PROBLEM ========================
  memo = vector<vector<int>>(n, vector<int>(3, -1));
  long lowest_cost = min(dp(0, 0), dp(0, 2));
  
  // ======================== PRINT OUTPUT ========================
  cout << lowest_cost << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t--)
    solve();
}