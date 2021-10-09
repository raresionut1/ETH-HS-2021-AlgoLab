#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;
typedef K::Segment_2 Segment;

bool solve () {
  int n; cin >> n; // nr of obstacle segments
  if (n == 0)
    return false;
  
  long xx, yy, aa, bb; 
  cin >> xx >> yy; // starting point
  cin >> aa >> bb; // point on the ray
  
  Point r_p1(xx, yy);
  Point r_p2(aa, bb);
  Ray r(r_p1, r_p2);
  
  bool intersect = false;
  
  // obstacle segments
  for (int i = 0; i < n; i++) {
    long rr, ss, tt, uu;
    cin >> rr >> ss; // one end
    cin >> tt >> uu; // the other end
    
    Point s_p1(rr, ss);
    Point s_p2(tt, uu);
    Segment s(s_p1, s_p2);
    
    if (CGAL::do_intersect(r, s)) {
      intersect = true;
      for (++i; i < n; i++)
        cin >> rr >> ss >> tt >> uu;
    }
  }
  
  cout << (intersect ? "yes" : "no") << endl;
  return true;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  while(solve()) {}
}