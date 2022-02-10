///3
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

void solve() {
  int n; cin >> n; // nr of friends
  int m; cin >> m; // nr of boxes
  vector<int> strengths(n); // vector of friends strength
  vector<int> boxes(m); // vector of boxes weights
  
  // read the strength of friends
  for(int i = 0; i < n; i++) {
    int s; cin >> s;
    strengths[i] = s;
  }
  
  // read the weight of boxes
  for(int i = 0; i < m; i++) {
    int w; cin >> w;
    boxes[i] = w;
  }
  
  // sort the strengths ascending by strength
  sort(strengths.begin(), strengths.end());
  // sort the boxes ascending by weights
  sort(boxes.begin(), boxes.end());
  
  // check if boxes can be moved by the friends or not
  if (boxes[m - 1] > strengths[n - 1]) {
    cout << "impossible" << endl;
    return;
  }
  
  // box counter - counts how many boxes have been distributed
  int current_box = 0;
  // maximum number of boxes carried by a friend
  int max_boxes_carried = 0;
  // vector that keeps count of how many friends take i boxes
  vector<int> nr_friends(m + 1, 0);
  
  // iterate over all friends
  for (int i = 0; i < n; i++) {
    int boxes_carried = 0;
  
    while(current_box < m) {
      if (strengths[i] < boxes[current_box])
        break;
      
      boxes_carried++;
      current_box++;
    }
    
    // if not taking enough boxes
    // => take some of the previously taken boxes
    while (boxes_carried < max_boxes_carried) {
      // case when current friend can take one more box
      if (nr_friends[max_boxes_carried] > 0) {
        nr_friends[max_boxes_carried]--;
        nr_friends[max_boxes_carried - 1]++;
        boxes_carried++;
        continue;
      }
      
      // case when nr_friends[max_boxes_carried] becomes 0
      max_boxes_carried--;
    }
    
    // update the maximum boxes carried
    max_boxes_carried = max(max_boxes_carried, boxes_carried);
    nr_friends[boxes_carried]++;
  }
  
  // calculate time taken to move all the books
  int time_taken = max_boxes_carried * 3 - 1;
  
  // print output
  cout << time_taken << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
}