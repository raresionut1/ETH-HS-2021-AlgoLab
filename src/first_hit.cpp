///1
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <typeinfo>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;
typedef K::Segment_2 Segment;

double floor_to_double(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

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
  Segment s_ray;
  Point p;
  
  bool intersect = false;
  vector<Segment> segments;
  
  // obstacle segments
  for (int i = 0; i < n; i++) {
    long rr, ss, tt, uu;
    cin >> rr >> ss; // one end
    cin >> tt >> uu; // the other end
    
    Point s_p1(rr, ss);
    Point s_p2(tt, uu);
    Segment s(s_p1, s_p2);
    
    segments.push_back(s);
  }
  
  // shuffle the segments
  random_shuffle(segments.begin(), segments.end());
  
  for (int i = 0; i < n; i++) {
    bool intersect_curr;
    
    if(!intersect)
      intersect_curr = CGAL::do_intersect(r, segments[i]);
    else
      intersect_curr = CGAL::do_intersect(s_ray, segments[i]);
      
    if (intersect_curr) {
      auto o = CGAL::intersection(r, segments[i]);
      
      // check if the intersection is a point or a segment
      if (const Point* op = boost::get<Point>(&*o))
        p = *op;
      else if (const Segment* os = boost::get<Segment>(&*o))
        // Check which end of the segment to take
        p = (CGAL::squared_distance(r_p1, os->source()) < 
             CGAL::squared_distance(r_p1, os->target()))
                ? os->source()
                : os->target();
      else
        cout << "WTF?!" << endl;
        
      intersect = true;
      s_ray = Segment(r_p1, p);
    }
  }
  
  if (!intersect)
    cout << "no" << endl;
  else
    cout << floor_to_double(p.x()) << " " << floor_to_double(p.y()) << endl;
  
  return true;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cout << setiosflags(ios::fixed) << setprecision(0);
  while(solve()) {}
}