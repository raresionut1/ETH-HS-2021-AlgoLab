///
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

void solve() {
  // ================ READ INPUT ================ 
  int n; cin >> n; // nr of islands
  int k; cin >> k; // nr of ironborn men;
  int w; cin >> w; // nt of waterways
  
  vector<int> c(n); // nr of men required for each island
  for (int i = 0; i < n; i++)
    cin >> c[i];
    
  vector<int> ww_len(w); // waterways lengths
  vector<vector<int>> ww(w); // waterways islands
  vector<vector<int>> ww_c(w); // waterways costs (partial sums)
  
  for (int i = 0; i < w; i++) {
    cin >> ww_len[i];
    ww[i] = vector<int>(ww_len[i]);
    ww_c[i] = vector<int>(ww_len[i]);
    int s = 0;
    
    for (int j = 0; j < ww_len[i]; j++) {
      cin >> ww[i][j];
      s += c[ww[i][j]];
      ww_c[i][j] = s;
    }
  }
  
  // ================ SOLVE PROBLEM ================
  // -> keep track of maximum nr of islands
  int max_islands = 0;
  
  // 1) check individual waterways
  for (int i = 0; i < w; i++) {
    int s = c[0]; // sum
    int l = 0; // left index
    int r = 0; // right index
    
    // --- sliding window ---
    while(r < ww_len[i]) {
      // case 1: s == k --> update best solution and increment r
      if (s == k) {
        max_islands = max(max_islands, r - l + 1);
        if (++r < ww_len[i])
          s += c[ww[i][r]];
        continue;
      }
      
      // case 2: s < k OR l == r -> increment r
      if (l == r || s < k) {
        if (++r < ww_len[i])
          s += c[ww[i][r]];
        continue;
      }
      
      // case 3: s > k -> increment l
      s -= c[ww[i][l++]];
    }
  }
  
  // 1) check for combinations of 2 waterways
  unordered_map<int, int> sums_map; // keep a map: sums --> max nr of islands
  
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < ww_len[i]; j++) {
      int s = ww_c[i][j]; // get the partial sum
      // we don't care about sums greater than k
      if (s >= k)
        break;
        
      // check if there is a pairing match
      auto entry = sums_map.find(k - s + c[0]);
      if (entry == sums_map.end())
        continue;
        
      // update best solution
      max_islands = max(max_islands, j + entry->second);
    }
    
    // update map of sums (can't do this before)
    for (int j = 0; j < ww_len[i]; j++)
      sums_map[ww_c[i][j]] = max(sums_map[ww_c[i][j]], j + 1);
  }
  
  // ================ PRINT OUTPUT ================
  cout << max_islands << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t--)
    solve();
}