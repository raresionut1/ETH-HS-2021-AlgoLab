///1
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct Boat {
  int l; // boat length
  int p; // ring position
  
  Boat () {}
  Boat (int l, int p) : l(l), p(p) {}
  
  bool operator<(Boat &b2) {
    return p < b2.p;
  }
  
  // function that returns minimum possible ending point
  // of the boat or -1 if it cannot be anchored
  int f(int last) {
    if (last == -1)
      return p;
    
    return last <= p
           ? max(p, last + l)
           : -1;
  }
};

void solve() {
  int n; cin >> n;
  vector<Boat> boats(n);
  
  // read the boats
  for(int i = 0; i < n; i++) {
    int l, p;
    cin >> l >> p;
    boats[i] = Boat(l, p);
  }
  
  // sort the boats by their ring position
  sort(boats.begin(), boats.end());
  
  int counter = 0;
  int i = 0;
  int last = -1;
  
  while (i < n) {
    int f_best = -1;
    int k_best = -1;
    
    // greedy - check the best next boat
    for (int k = i; k < n; k++) {
      // early break
      if ((f_best != -1) && (f_best < boats[k].p))
        break;
      
      // calculate ending point
      int f = boats[k].f(last);
      
      // skip if it cannot be anchored
      if (f == -1)
        continue;
        
      // check if no other boat was found
      if (f_best == -1) {
        f_best = f;
        k_best = k;
        continue;
      }
      
      // check if this boat is better than the previous best
      if (f < f_best) {
        f_best = f;
        k_best = k;
      }
    }
    
    // if there is no other boat that can be anchored -> stop
    if (k_best == -1) {
      break;
    }
    else {
      i = k_best + 1;
      last = f_best;
      counter++;
    }
  }
  
  cout << counter << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
}