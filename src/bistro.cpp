#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> DT;
typedef K::Point_2 Point;

void solve(int n) {
  // n == nr of existing restaurants
  vector<Point> old_rest(n);
  // existing restaurants locations
  for (int i = 0; i < n; i++) {
    long x; cin >> x;
    long y; cin >> y;
    old_rest[i] = Point(x, y);
  }
  
  // Compute Delaunay Triangulation
  DT dt;
  dt.insert(old_rest.begin(), old_rest.end());
  
  // nr of possible locations for the new restaurant
  int m; cin >> m;
  // possible locations for the new restaurant
  for (int i = 0; i < m; i++) {
    long x; cin >> x;
    long y; cin >> y;
    Point new_rest = Point(x, y);
    
    // find the squared euclidean distance to the
    // closest existing restaurant
    Point nearest_rest = dt.nearest_vertex(new_rest)->point();
    long xi = nearest_rest.x();
    long yi = nearest_rest.y();
    long dist = pow(x - xi, 2) + pow(y - yi, 2);
    
    // print output
    cout << dist << endl;
  }
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  int n;
  while (cin >> n && n != 0)
    solve(n);
}