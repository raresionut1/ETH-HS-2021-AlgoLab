#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <typeinfo>

using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef K::Point_2 Point;
typedef CGAL::Min_circle_2_traits_2<K> Min_circle_traits;
typedef CGAL::Min_circle_2<Min_circle_traits> Min_circle;

double ceil_to_double(const K::FT& x) {
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

bool solve () {
  int n; cin >> n; // nr of points
  if (n == 0)
    return false;
  
  vector<Point> points;
  
  // points
  for (int i = 0; i < n; i++) {
    long x_i, y_i;
    cin >> x_i >> y_i; // read the point
    
    Point p_i(x_i, y_i);
    points.push_back(p_i);
  }
  
  // true is for randomization
  Min_circle mc(points.begin(), points.end(), true);
  Min_circle_traits::Circle c = mc.circle();
  
  // Print output
  cout << ceil_to_double(CGAL::sqrt(c.squared_radius())) << endl;
  
  return true;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cout << setiosflags(ios::fixed) << setprecision(0);
  while(solve()) {}
}