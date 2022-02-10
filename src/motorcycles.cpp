///
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Ray_2 Ray;
typedef K::Segment_2 Segment;
typedef K::Line_2 Line;
typedef CGAL::Gmpq Gmpq;

struct Biker {
  int nr;
  long y0;
  Gmpq slope;
  
  // in order to be able to sort the bikers vector
  bool operator<(const Biker &b2) {
      return y0 < b2.y0;
  }
};

void solve () {
  int n; cin >> n; // nr of bikers
  
  vector<Biker> bikers(n);
  vector<bool> eliminated(n, false);
  
  // read bikers pos
  for (int i = 0; i < n; i++) {
    long y0, x1, y1;
    cin >> y0; // start
    cin >> x1 >> y1; // direction
    
    bikers[i] = {i, y0, Gmpq(y1 - y0, x1)};
  }
  
  // sort the bikers by their y0 value
  sort(bikers.begin(), bikers.end());
  
  Biker best_biker = bikers[0];
  // upward pass
  for (int i = 1; i < n; i++) {
    Gmpq best_slope = best_biker.slope;
    Gmpq slope = bikers[i].slope;

    if (CGAL::abs(slope) < CGAL::abs(best_slope) 
        || (CGAL::abs(slope) == CGAL::abs(best_slope) && slope > best_slope))
      best_biker = bikers[i];
    else if (slope < best_slope)
      eliminated[bikers[i].nr] = true;
  }
  
  best_biker = bikers[n - 1];
  // downward pass
  for (int i = n - 2; i >= 0; i--) {
    Gmpq best_slope = best_biker.slope;
    Gmpq slope = bikers[i].slope;

    if (CGAL::abs(slope) <= CGAL::abs(best_slope))
      best_biker = bikers[i];
    else if (slope > best_slope)
      eliminated[bikers[i].nr] = true;
  }

  // print output
  for (int i = 0; i < n; i++) {
    if (!eliminated[i]) {
      cout << i << " ";
    }
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0) {solve();}
  return 0;
}