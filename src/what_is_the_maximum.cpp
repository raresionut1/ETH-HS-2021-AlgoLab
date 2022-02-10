#include <iostream>
#include <cmath>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

typedef int IT; // input type
typedef CGAL::Gmpz ET; // solver type
typedef CGAL::Quadratic_program<IT> Program; // program type
typedef CGAL::Quadratic_program_solution<ET> Solution; // solution type

void maximize(int a, int b) {
  // Transform problem into a minimization problem
  // maximize (by - ax) -> minimize (ax - by)
  Program lp(CGAL::SMALLER, true, 0, false, 0); // just lower bounds
  
  // variables
  const int x = 0;
  const int y = 1;
  
  // objective function
  lp.set_c(x, a); lp.set_c(y, -b); // ax - by
  
  // constraints
  lp.set_a(x, 0,  1); lp.set_a(y, 0, 1); lp.set_b(0,     4); //  x +  y <=  4
  lp.set_a(x, 1,  4); lp.set_a(y, 1, 2); lp.set_b(1, a * b); // 4x + 2y <= ab
  lp.set_a(x, 2, -1); lp.set_a(y, 2, 1); lp.set_b(2,     1); // -x +  y <=  1
  
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  // output solution
  if (s.is_optimal()) {
    double res = floor(CGAL::to_double(-s.objective_value()));
    cout << setprecision(0) << fixed << res << endl;
  } else if (s.is_unbounded()) {
    cout << "unbounded" << endl;
  } else if (s.is_infeasible()) { 
    cout << "no" << endl;
  }
}

void minimize(int a, int b) {
  Program lp(CGAL::SMALLER, false, 0, true, 0); // just upper bounds
  
  // variables
  const int x = 0;
  const int y = 1;
  const int z = 2;
  
  // objective function
  lp.set_c(x, a); lp.set_c(y, b); lp.set_c(z, 1); // ax + by + z
  
  // constraints
  lp.set_a(x, 0, -1); lp.set_a(y, 0, -1);                     lp.set_b(0,     4); //  x +  y     >=  -4
  lp.set_a(x, 1, -4); lp.set_a(y, 1, -2); lp.set_a(z, 1, -1); lp.set_b(1, a * b); // 4x + 2y + z >= -ab
  lp.set_a(x, 2,  1); lp.set_a(y, 2, -1);                     lp.set_b(2,     1); // -x +  y     >=  -1
  
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  // output solution
  if (s.is_optimal()) {
    double res = ceil(CGAL::to_double(s.objective_value()));
    cout << setprecision(0) << fixed << res << endl;
  } else if (s.is_unbounded()) {
    cout << "unbounded" << endl;
  } else if (s.is_infeasible()) { 
    cout << "no" << endl;
  }
}

bool solve() {
  int p; cin >> p; // problem type
  if (p == 0)
    return false;
    
  int a; cin >> a; // first coeficient
  int b; cin >> b; // second coeficient
  
  p == 1 ? maximize(a, b) : minimize(a, b); // solve the problem
  
  return true;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  while(solve()){}
}