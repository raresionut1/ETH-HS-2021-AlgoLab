#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> DT;
typedef K::Point_2 Point;

void solve(int n) {
  // n == nr of bacteria
  int l, b, r, t;
  cin >> l >> b >> r >> t; // dish bounds
  
  vector<Point> bacteria(n);
  // bacteria positions (centers)
  for (int i = 0; i < n; i++) {
    long x; cin >> x;
    long y; cin >> y;
    bacteria[i] = Point(x, y);
  }
  
  // dying times of each bacteria
  vector<double> dying_times;
  
  // Compute Delaunay Triangulation
  DT dt;
  dt.insert(bacteria.begin(), bacteria.end());
  
  // Iterate through all vertices (centers of bacterias)
  for (auto v1 = dt.vertices_begin(); v1 != dt.vertices_end(); v1++) {
    Point p1 = v1->point();
  
    // the radius of the bacteria when it dies
    double radius = -1;
    
    // constrain radius by nearest neighbor
    auto v2 = CGAL::nearest_neighbor(dt, v1->handle());
    if (v2 != NULL) {
      Point p2 = v2->point();
      double dist_sq = CGAL::squared_distance(p1, p2);
      double dist = sqrt(dist_sq);
      radius = dist / 2;
    }
    
    // constrain radius by the dish bounds
    radius = radius == -1 
             ? abs(p1.x() - l) 
             : min(radius, (double) abs(p1.x() - l));
    radius = min(radius, (double) abs(p1.x() - r));
    radius = min(radius, (double) abs(p1.y() - t));
    radius = min(radius, (double) abs(p1.y() - b));
    
    // since radius = t^2 + 0.5
    // => t = sqrt(radius - 0.5)
    double dying_time = sqrt(max(radius - 0.5, 0.0));
    dying_times.push_back(dying_time);
  }
  
  // sort dying times in ascending order
  sort(dying_times.begin(), dying_times.end());
  
  // compute requested dying times
  long time_f = ceil(dying_times[0]);
  long time_m = ceil(dying_times[n / 2]);
  long time_l = ceil(dying_times[n - 1]);
  
  // print output
  cout << time_f << " " << time_m << " " << time_l << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  int n;
  while (cin >> n && n != 0)
    solve(n);
}