#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

void solve () {
  int n; cin >> n; // nr of bombs -- 2^i - 1
  vector<int> t(n); // explosion times for the bombs
  
  // read the exploding times for the bombs
  for (int i = 0; i < n; i++)
    cin >> t[i];
  
  // reduce the bombs exploding times according to the constraint:
  // bomb on top must be detonated AFTER bombs on bottom
  // => the exploding time for the bombs on bottom can be reduced to the 
  // exploding time of the bomb on top - 1
  for (int i = 0; i <= (n - 3) / 2; i++) {
    t[2 * i + 1] = min(t[2 * i + 1], t[i] - 1); 
    t[2 * i + 2] = min(t[2 * i + 2], t[i] - 1);
  }
  
  // sort the exploding times
  sort(t.begin(), t.end());
  
  // check if the bombs can be detonated safely
  for (int i = 0; i < n; i++) {
    if (i >= t[i]) {
      cout << "no" << endl;
      return;
    }
  }
  
  cout << "yes" << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0) {solve();}
  return 0;
}