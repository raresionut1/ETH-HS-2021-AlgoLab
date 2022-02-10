///5
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

// Sliding window -> Get intervals == k
void compute_intervals(int n, int k, VI &v, VI &intervals) {
  int i = 0;
  int j = 0;
  int s = v[0];
  while(i < n && j < n) {
    // I can't be greater than j (OR)
    // The sum is smaller than k
    if (i > j || s < k) {
      s += v[++j];
      continue;
    }
    
    // The sum is greater than k
    if (s > k) {
      s -= v[i++];
      continue;
    }
    
    // The sum is equal to k
    // => mark the end of the interval
    // -- index == start (i)
    // -- value == end (j)
    intervals[i] = j;
    s -= v[i++];
  }
}

// bottom-up dp -> compute best scores
void compute_scores(int n, int m, VI &intervals, vector<vector<pair<int, int>>> &dp) {
  // iterate for all number of maximum attackers
  for (int a = 1; a <= m; a++) {
    // iterate for all starting positions i
    for (int i = n-1; i >= 0; i--) {
      // update the current score with the previous best score
      dp[a][i] = dp[a][i+1];
      
      // get the end of the interval
      int j = intervals[i];
      // if there is an interval of sum k starting in i
      // and the best score of it + best score after the interval
      // is better than the current best score
      // => update the current best score
      //    obs: also increase the attackers counter
      if (j != -1 && dp[a-1][j+1].first + (j - i + 1) > dp[a][i].first)
        dp[a][i] = {dp[a-1][j+1].first + (j - i + 1), dp[a-1][j+1].second + 1};
    }
  }
}

void solve() {
  // Declarations
  int n; cin>>n; // nr of defenders
  int m; cin>>m; // nr of attackers
  int k; cin>>k; // attack strength
  VI v(n + 1, 0); // defense values
  
  // Read defense values
  for (int i=0; i<n; i++) {
    int vi; cin>> vi;
    v[i] = vi;
  }
  
  // used to memorize intervals of sum k
  VI intervals(n, -1);
  
  // compute the intervals of sum k
  compute_intervals(n, k, v, intervals);
  
  // used to memorize best scores
  // -- scores[a][i] => best score using at most a attackers and
  //     using only defenders starting from index i
  vector<vector<pair<int, int>>> dp(m+1, vector<pair<int, int>>(n+1, {0, 0}));
  
  // compute the best scores
  compute_scores(n, m, intervals, dp);
  
  cout << (dp[m][0].second == m ? to_string(dp[m][0].first) : "fail") << endl;
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