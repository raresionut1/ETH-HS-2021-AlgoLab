///
#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

struct Legion {
  long a;
  long b;
  long c;
  long v;
};

void solve () {
  // ================= READ INPUT =================
  long xs, ys; // initial position
  cin >> xs >> ys;
  long n; cin >> n; // nr of legions
  
  // read legions
  vector<Legion> legions(n);
  for (long i = 0; i < n; i++) {
    cin >> legions[i].a;
    cin >> legions[i].b;
    cin >> legions[i].c;
    cin >> legions[i].v;
  }
  
  // ================= SOLVE PROBLEM =================
  
  // create an LP with Ax <= b
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  const long t = 0; // prep time
  const long xp = 1; // coordinate x of best location
  const long yp = 2; // coordinate y of best location
  
  long nrow = 0;

  // ========= CONSTRAINTS =========
  // set the 'side' constraints for the legions
  // -- for each legion, we compute the max prep time
  // dist (p, l) / v <= t
  
  // -- for left side:
  // (a * xp + b * yp + c) / sqrt(a^2 + b^2) <= t * v
  // (a * xp) + (b * yp) + c <= t * v * sqrt(a^2 + b^2)
  // (a * xp) + (b * yp) - (t * v * sqrt(a^2 + b^2)) <= -c
  
  for (long i = 0; i < n; i++) {
    long a = legions[i].a;
    long b = legions[i].b;
    long c = legions[i].c;
    long v = legions[i].v;
    
    // we need to be on the same side of the legion as the initial position
    if (a * xs + b * ys + c <= 0) {
      lp.set_a(xp, nrow, a);
      lp.set_a(yp, nrow, b);
      lp.set_b(nrow, -c);
    } else {
      lp.set_a(xp, nrow, -a);
      lp.set_a(yp, nrow, -b);
      lp.set_b(nrow, c);
    }
    lp.set_a(t, nrow, v * sqrt(a*a + b*b));
    
    nrow++;
  }
  
  // ========= OBJECTIVE FUNCTION =========
  // we want to maximize t
  lp.set_l(t, true, 0);
  lp.set_c(t, -1);
  
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  
  // output solution
  cout << floor(CGAL::to_double(-s.objective_value())) << endl; 
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  long t; cin >> t;
  while(t--)
    solve();
}
