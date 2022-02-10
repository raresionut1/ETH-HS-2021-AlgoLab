///3
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

void insert_best_pos(VI &best_pos, int vi, int vj) {
  int d_floor = floor((((float) vi) + ((float) vj)) / 2);
  int d_ceil = ceil((((float) vi) + ((float) vj)) / 2);
  
  best_pos.push_back(d_floor);
  if (d_ceil != d_floor)
    best_pos.push_back(d_ceil);
}

void solve() {
  // Declarations
  int n; cin>>n; // nr of parasols
  VI v; // parasols locations
  
  // Read parasols locations
  for (int i=0; i<n; i++) {
    int vi; cin>>vi;
    v.push_back(vi);
  }
  
  sort(v.begin(), v.end()); // sort locations by value
  
  // We'll use a sliding window approach
  // Check all intervals of size <= 200
  // and keep track of the best positions
  int i = 0;
  int j = 0;
  int best_parasols = 0;
  int best_distance = 201;
  VI best_pos;
  while(j < n) {
    // i cannot be greater than j
    if (i > j) {
      j++;
      continue;
    }
    
    // if possible, increase j without losing a parasol
    if (j < (n - 1) && v[j+1] - v[i] <= 200) {
      j++;
      continue;
    }
    
    // if reached this point, it means that we can't
    // increase the interval without losing a parasol
    int current_parasols = j - i + 1;
    int current_distance = ceil((((float) v[j]) - ((float) v[i])) / 2);
    
    // case 1 -> the interval contains more parasols than the previous best
    // case 2 -> the interval contains same parasols as previous best, but
    //           the distance is smaller than the previous best
    if (current_parasols > best_parasols ||
        (current_parasols == best_parasols &&
         current_distance <= best_distance)) {
      // case 3 -> the interval contains same parasols as previous best and
      // the distance is the same as the previous best => don't clear best_pos
      if(current_parasols > best_parasols || current_distance < best_distance)
        best_pos.clear();
        
      best_parasols = current_parasols;
      best_distance = current_distance;
      insert_best_pos(best_pos, v[i], v[j]);
      i++;
      continue;
    }
    
    // increase the counter anyway to slide the window to the right
    i++;
  }
  
  // === print output ===
  // nr. parasols, best distance
  // best pos 1, best pos 2, ...
  cout << best_parasols << " " << best_distance << endl;
  for (int vi : best_pos)
    cout << vi << " ";
  cout << endl;
}

int main() {
  // Run tests
  int nr; cin>>nr;
  for (int i = 0; i < nr; i++) {
    solve();
  }
}