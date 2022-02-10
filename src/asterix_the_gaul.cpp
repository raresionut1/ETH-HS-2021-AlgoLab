///4
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int n; // nr of moves
int m; // ammount of available magic potion (how many gulps)
long D; // Asterix distance to Panoramix
long T; // nr of seconds to reach Panoramix (for the romans)
vector<pair<long, long>> M; // vector of moves
vector<long> S; // vector of distances for i gulps of the potion

void read_input() {
  cin >> n >> m >> D >> T;
  
  vector<pair<long, long>> M_aux(n);
  M = M_aux;
  
  vector<long> S_aux(m + 1);
  S = S_aux;
  S[0] = 0; // 0 gulps => 0 boost in distance
  
  // read available movements
  for(int i = 0; i < n; i++) {
    long d; cin >> d;
    long t; cin >> t;
    pair<long, long> m_i(d, t);
    M[i] = m_i;
  }
  
  // read the distances for i gulps of the potion
  for(int i = 1; i <= m; i++) {
    long s; cin >> s;
    S[i] = s;
  }
}

void compute_subsets(int start, int end, long d, long t, int gulps, vector<pair<long, long>> &subsets) {
  // stop condition #1 - this subset of moves takes TOO MUCH time
  if (t >= T)
    return;
  
  // stop condition #2 - no more moves => memorize current subset
  if (start > end) {
    pair<long, long> p(d, t);
    subsets.push_back(p);
    return; 
  }
  
  // get current move
  pair<long, long> move = M[start];
  long move_d = move.first + S[gulps];
  long move_t = move.second;
  
  compute_subsets(start + 1, end, d + move_d, t + move_t, gulps, subsets); // use current move
  compute_subsets(start + 1, end, d, t, gulps, subsets); // DON'T use current move
}

bool custom_comparator(const pair<long, long> &a, const pair<long, long> &b) {
    return a.second != b.second
      ? a.second < b.second
      : a.first < b.first;
}

void preprocess(vector<pair<long, long>> &subsets) {
  // Sort subsets (ascending by time and descending by distance)
  sort(subsets.begin(), subsets.end(), custom_comparator);
  
  vector<pair<long, long>> subsets_new;
  subsets.push_back(make_pair(-1, -1)); // insert a dummy entry at the end
  long max_d = 0; // keeps track of maximum distance so far
  
  for (unsigned int i = 0; i < subsets.size() - 1; i++) {
    long curr_d = subsets[i].first;
    long curr_t = subsets[i].second;
    long next_t = subsets[i + 1].second;
    
    // Add new entry in the new vector of subsets
    if (curr_t != next_t) {
      max_d = max(max_d, curr_d);
      subsets_new.push_back(make_pair(max_d, curr_t));
    }
  }
  
  subsets = subsets_new;
}

bool check(int gulps) {
  // === SPLIT & LIST ===
  vector<pair<long, long>> subsets1, subsets2;
  compute_subsets(0, n / 2 - 1, 0, 0, gulps, subsets1);
  compute_subsets(n / 2, n - 1, 0, 0, gulps, subsets2);
  
  // Keep only the maximum distance achievable for each timestamp
  // (to be able to use binary search)
  preprocess(subsets1);
  preprocess(subsets2);
  
  // Try to find a pair that solves the problem
  // 1) Iterate through subset1 in order
  for (unsigned int i = 0; i < subsets1.size(); i++) {
    long d1 = subsets1[i].first;
    long t1 = subsets1[i].second;
    
    // 2) Use binary search to find a solution
    unsigned int l = 0; // left index
    unsigned int r = subsets2.size() - 1; // right index
    
    while(l <= r) {
      unsigned int mid = (l + r) / 2;
      
      long d2 = subsets2[mid].first;
      long t2 = subsets2[mid].second;
      
      // check if we found a solution
      if (t1 + t2 < T && d1 + d2 >= D)
        return true;
      
      // update window
      if (t1 + t2 < T)
        l = mid + 1; // go right
      else
        r = mid - 1; // go left
    }
  }
  
  return false;
}

void solve() {
  read_input();
  
  int best_s = -1; // lowest number of gulps to solve the problem (-1 means impossible)
  
  // Search for solution using binary search over the values of gulps
  int l = 0; // left index
  int r = m; // right index
  
  while(l <= r) {
    int mid = (l + r) / 2; // mid == number of gulps

    if (check(mid)) {
      best_s = mid;
      r = mid - 1; // go left
    }
    else {
      l = mid + 1; // go right
    }
  }
  
  cout << (best_s != -1 ? to_string(best_s) : "Panoramix captured") << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
}