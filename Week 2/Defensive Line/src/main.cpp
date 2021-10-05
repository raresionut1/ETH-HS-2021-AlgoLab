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
void compute_scores(int n, int m, VI &intervals, VVI &scores, VVI &att_used) {
  // iterate for all number of maximum attackers
  for (int a = 1; a <= m; a++) {
    // iterate for all starting positions i
    for (int i = n-1; i >= 0; i--) {
      // update the current score with the previous best score
      scores[a][i] = scores[a][i+1];
      att_used[a][i] = att_used[a][i+1];
      
      // get the end of the interval
      int j = intervals[i];
      // if there is an interval of sum k starting in i
      // and the best score of it + best score after the interval
      // is better than the current best score
      // => update the current best score
      //    obs: also increase the attackers counter
      if (j != -1 && scores[a-1][j+1] + (j - i + 1) > scores[a][i]) {
        scores[a][i] = scores[a-1][j+1] + (j - i + 1);
        att_used[a][i] = att_used[a-1][j+1] + 1;
      }
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
  VVI scores(m+1, VI(n+1, 0));
  
  // used to memorize number of attackers used
  // -- att_used[a][i] => number of attackers used to obtain
  //    the score score[a][i]
  VVI att_used(m+1, VI(n+1, 0));
  
  // compute the best scores
  compute_scores(n, m, intervals, scores, att_used);
  
  cout << (att_used[m][0] == m ? to_string(scores[m][0]) : "fail") << endl;
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