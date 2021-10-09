///2
#include <iostream>
#include <vector>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;

bool is_point_in_triangle(Point &p, vector<Point> &q) {
  // Check if the point is on the correct side compared to each edge
  // RIGHT_TURN -> OK
  // COLLINEAR -> OK
  // LEFT_TURN -> NOT OK
  return !(CGAL::left_turn(q[0], q[1], p) ||
           CGAL::left_turn(q[2], q[3], p) ||
           CGAL::left_turn(q[4], q[5], p));
}

void solve () {
  int m; cin >> m; // nr of legs = m - 1
  int n; cin >> n; // nr of map parts
  
  // =============== READ AND PARSE INPUT =============== 
  // read hike checkpoints -> legs are constructed of adjacent checkpoints
  vector<Point> points;
  for(int i = 0; i < m; i++) {
    int x; cin >> x;
    int y; cin >> y;
    Point p(x, y);
    
    points.push_back(p);
  }
  
  // read map parts (triangles)
  vector<vector<Point>> maps;
  for(int i = 0; i < n; i++) {
    // vector for the 6 triangle points
    vector<Point> q;
    
    for (int k = 0; k < 6; k++) {
      int qk_0, qk_1; cin >> qk_0 >> qk_1;
      Point qk(qk_0, qk_1);
      q.push_back(qk);
    }
    
    // check orientation of edges -> swap points if necessary
    if (CGAL::left_turn(q[0], q[1], q[2])) {
      Point aux = q[0];
      q[0] = q[1];
      q[1] = aux;
    }
    if (CGAL::left_turn(q[2], q[3], q[0])) {
      Point aux = q[2];
      q[2] = q[3];
      q[3] = aux;
    }
    if (CGAL::left_turn(q[4], q[5], q[0])) {
      Point aux = q[4];
      q[4] = q[5];
      q[5] = aux;
    }
    
    maps.push_back(q);
  }
  
  // =============== SOLVE THE PROBLEM =============== 
  // Best map parts sequence indexes
  int best_i = 0;
  int best_j = n;
  // Counts how many legs are covered by the current map parts
  int counter_legs_covered = 0;
  // Vector that counts for each leg how many map parts include it
  // (in the current sequence)
  VI covered(m - 1, 0);
  // Queue for keeping track of the covered legs (for each explored map part)
  queue<VI> queue_maps;
  
  // Sliding window approach for finding the best sequence
  int i = 0;
  int j = -1;
  while(i < n && j < n) {
    // cout << "i: " << i << " | j: " << j << endl;
    
    // Check if all the legs are covered
    if (counter_legs_covered == m - 1) {
      // Check if best solution is found
      if (j + 1 - i < best_j - best_i) {
        best_i = i;
        best_j = j + 1;
      }
        
      // Increase i counter
      // -> check legs covered by triangle i and decrease counters
      VI legs_covered = queue_maps.front();
      queue_maps.pop();
      
      for (int k : legs_covered) {
        covered[k]--;
        // if there are no more maps that cover leg k -> decrease counter
        if (covered[k] == 0) {
          counter_legs_covered--;
        }
      }
      
      i++;
      continue;
    }
    
    // If not all the legs are covered
    // Increase j counter
    if (j + 1 == n)
      break;
    // -> check legs covered by triangle new_j and increase counters
    VI legs_covered;
    for (int k = 0; k < m - 1; k++) {
      if(is_point_in_triangle(points[k], maps[j + 1]) && 
         is_point_in_triangle(points[k + 1], maps[j + 1])) {
        covered[k]++;
        legs_covered.push_back(k);
        // if there are no more maps that cover leg k -> decrease counter
        if (covered[k] == 1) {
          counter_legs_covered++;
        }
      }
    }
    
    queue_maps.push(legs_covered);
    
    j++;
  }
  
  // compute best cost
  int best_cost = best_j - best_i;
  
  cout << best_cost << endl; 
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  for (int i = 0; i < t; i ++) {
    solve();
  }
}