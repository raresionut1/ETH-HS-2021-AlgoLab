///3
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

void solve() {
  // Declarations
  int n; cin>>n; // nr of cards
  int k; cin>>k; // fav number
  vector<int> v; // cards value
  
  // Read cards
  for (int i=0; i<n; i++) {
    int vi; cin>>vi;
    v.push_back(vi);
  }
  
  // keep track of the best solution so far
  int best_score = abs(k - v[0]);
  int best_i = 0;
  int best_j = 0;
  
  // Sliding window approach -> O(n)
  int i = 0;
  int j = 0;
  int s = v[0];

  // Slide window until we find the optimal solution
  while (best_score != 0) {
    // i pointer cannot be bigger than j => increase j
    if (i > j) {
      j++;
      s += v[j];
    }
    
    // compute current score
    int score = abs(k - s);
    // update best variables if necessary
    if (score < best_score) {
      best_score = score;
      best_i = i;
      best_j = j;
    }
    
    // if current sum is bigger than k, increase the left counter
    if (s < k) {
      j++;
      if (j < n) {
        s += v[j];
      } else {
        break;
      }
    // if current sum is smaller than k, increase the right counter
    } else {
      i++;
      if (i < n) {
        s -= v[i - 1];
      } else {
        break;
      }
    }
  }
  
  cout << best_i << " " << best_j << endl;
}

int main() {
  // Run tests
  int nr; cin>>nr;
  for (int i = 0; i < nr; i++) {
    solve();
  }
}