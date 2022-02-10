///3
// Includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
// Delaunay includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
// LP includes
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;

// LP typedefs
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// rounding function
int floor_to_double(const CGAL::Quotient<ET>& x) {
  int a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}

void solve () {
  int n; cin >> n; // nr of warehouses
  int m; cin >> m; // nr of stadiums
  int c; cin >> c; // nr of contour lines
  
  // ============= READ INPUT =============
  vector<Point> w_pos(n);
  vector<int> supply(n);
  vector<int> alcohol_perc(n);
  // read warehouses
  for (int i = 0; i < n; i++) {
    cin >> w_pos[i]; // coordinates
    cin >> supply[i]; // supply
    cin >> alcohol_perc[i]; // alcohol percentage (in %)
  }
  
  vector<Point> s_pos(m);
  vector<int> demand(m);
  vector<int> alcohol_limit(m);
  // read stadiums
  for (int i = 0; i < m; i++) {
    cin >> s_pos[i]; // coordinates
    cin >> demand[i]; // demand
    cin >> alcohol_limit[i]; // upper limit on pure alcohol consumed
  }
  
  vector<vector<int>> revenues(n, vector<int>(m));
  // read revenues
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      cin >> revenues[i][j]; // revenue
  
  vector<Point> c_pos(c);
  vector<int> c_radius(c);
  // read contour lines
  for (int i = 0; i < c; i++) {
    cin >> c_pos[i]; // coordinates of center
    cin >> c_radius[i]; // radius
  }
  
  // ============= SOLVE PROBLEM =============
  // For each warehouse / stadium
  // --> find the contours that enclose it
  vector<set<int>> w_contours(n);
  vector<set<int>> s_contours(m);
  
  for (int i = 0; i < c; i++) {
    // iterate over all warehouses --> check if it is enclosed by current contour
    for (int j = 0; j < n; j++)
      if (pow(c_radius[i], 2) >= CGAL::squared_distance(c_pos[i], w_pos[j]))
        w_contours[j].insert(i);
    
    // iterate over all stadiums --> check if it is enclosed by current contour
    for (int j = 0; j < m; j++)
      if (pow(c_radius[i], 2) >= CGAL::squared_distance(c_pos[i], s_pos[j]))
        s_contours[j].insert(i);
  }
  
  // --- PART 1 --- LINEAR PROGRAMMING ---
  // create an LP with Ax <= b and no upper bounds
  // -> lower bound == 0 because we can't deliver negative nr of liters of beer
  // Obs: variable_index(w, s) = w * m + s
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  // --- OBJECTIVE FUNCTION ---
  // maximize sum(l_ws * (r_ws - t_ws / 100))
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      int r_ws = revenues[i][j];
      
      // compute t_ws
      vector<int> intersection;
      set_intersection(w_contours[i].begin(), w_contours[i].end(), 
                      s_contours[j].begin(), s_contours[j].end(), 
                      back_inserter(intersection));
      int t_ws = w_contours[i].size() + 
                s_contours[j].size() - 
                2 * intersection.size();
      
      // set variable coefficient in the objective function
      // multiply by 100 to avoid NOT integer coefficients
      lp.set_c(i * m + j, -(r_ws * 100 - t_ws));
    }
  }
  
  // --- CONSTRAINTS ---
  // constraint row index
  int row = 0;
  
  // set supply constraints
  // sum(l_w,si) <= supply(w)
  for (int i = 0; i < n; i++) {
    lp.set_b(row, supply[i]);
    for (int j = 0; j < m; j++) {
      lp.set_a(i * m + j, row, 1);
    }
    row++;
  }
  
  // set demand constraints
  // sum(l_wi,s) == demand(s)
  for (int j = 0; j < m; j++) {
    // upper limit: sum(l_wi,s) <= demand(s)
    lp.set_b(row, demand[j]);
    for (int i = 0; i < n; i++) {
      lp.set_a(i * m + j, row, 1);
    }
    row++;
    
    // lower limit: sum(l_wi,s) >= demand(s)
    lp.set_b(row, -demand[j]);
    for (int i = 0; i < n; i++) {
      lp.set_a(i * m + j, row, -1);
    }
    row++;
  }
  
  // set alcohol limit constraints
  // sum(l_wi,s * a_wi / 100) <= alcohol_limit(s)
  for (int j = 0; j < m; j++) {
    lp.set_b(row, alcohol_limit[j] * 100);
    for (int i = 0; i < n; i++) {
      lp.set_a(i * m + j, row, alcohol_perc[i]);
    }
    row++;
  }
  
  // --- GET SOLUTION ---
  Solution sol = CGAL::solve_nonnegative_linear_program(lp, ET());
  
  // ============= PRINT OUTPUT =============
  if (sol.is_infeasible()) {
    cout << "RIOT!" << endl;
  } else { // Can't be unbounded (bc of the limited supply)
    cout << fixed << setprecision(0) << floor_to_double(-sol.objective_value() / 100) << endl;
  }
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t--)
    solve();
}