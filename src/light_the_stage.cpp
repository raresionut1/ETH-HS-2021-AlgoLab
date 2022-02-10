///2
#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> DT;
typedef K::Point_2 Point;

bool is_eliminated(Point &p1, int r1, Point &p2, int r2) {
  // squared distance between the centers
  double dist_sq = CGAL::squared_distance(p1, p2);
  // minimum distance between the centers so
  // that the player doesn't get eliminated
  double dist_sq_threshold = pow(r1 + r2, 2);
  
  return dist_sq < dist_sq_threshold;
}

void solve() {
  int m; cin >> m; // nr of participants
  int n; cin >> n; // nr of lamps
  
  vector<Point> p_coord(m); // the coordinates of the players
  vector<int> p_radius(m); // the radiuses of the players
  
  // read participants
  for (int i = 0; i < m; i++) {
    int x; cin >> x;
    int y; cin >> y;
    int r; cin >> r;
    
    p_coord[i] = Point(x, y);
    p_radius[i] = r;
  }
  
  int h; cin >> h; // the height of the lamps
  vector<Point> lamps(n); // the coordinates of the lamps
  
  // read lamps coordinated
  for (int i = 0; i < n; i++) {
    int x; cin >> x;
    int y; cin >> y;
    
    lamps[i] = Point(x, y);
  }
  
  // Keep track of the round when each player gets eliminated (n == not eliminated)
  vector<int> round_eliminated(m, n); // initially, no player is eliminated
  // Also keep track of the biggest such value
  int most_survived = -1;
  
  // Compute Delaunay Triangulation on lamps centers
  DT dt;
  dt.insert(lamps.begin(), lamps.end());
  
  // iterate through all players
  for (int i = 0; i < m; i++) {
    bool eliminated = is_eliminated(p_coord[i], // center of player
                                    p_radius[i], // radius of player
                                    dt.nearest_vertex(p_coord[i])->point(), // center of the nearest lamp
                                    h // radius of the nearest lamp
                                   );
    
    // --- EARLY STOP ---
    // if the player is NOT eliminated by the nearest lamp
    // => he will never be eliminated
    if (!eliminated) {
      // round_eliminated[i] = n;
      most_survived = n;
      continue;
    }
    
    // if the player is eliminated by the nearest lamp
    // => check when it gets eliminated (on which round)
    for (int j = 0; j < n; j++) {
      eliminated = is_eliminated(p_coord[i], // center of player
                                 p_radius[i], // radius of player
                                 lamps[j], // center of the current round lamp
                                 h // radius of the current round lamp
                                );
                                
      // if the player is eliminated on this round => stop searching
      if (eliminated) {
        round_eliminated[i] = j;
        most_survived = max(most_survived, j);
        break;
      }
    }
  }
  
  // -- print output --
  // check which players survived the most
  for (int i = 0; i < m; i++) {
    if (round_eliminated[i] == most_survived)
      cout << i << " ";
  }
  cout << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  int t; cin >> t;
  while (t-- > 0)
    solve();
}