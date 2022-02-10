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

bool solve() {
  // ================== READ THE INPUT ================== 
  int n; cin >> n; // nr of constraints
  if (n == 0)
    return false;
  
  int d; cin >> d; // nr of dimensions
  
  // constraints --> a0*x0 + a1*x1 + ... + a_(d-1)*x_(d-1) <= b
  vector<vector<int>> A(n, vector<int>(d));
  vector<int> b(n);
  
  // Read the constraints
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < d; j++)
      cin >> A[i][j];
      
    cin >> b[i];
  }
  
  // ================== SOLVE THE PROBLEM ================== 
  Program lp(CGAL::SMALLER, false, 0, false, 0); // no bounds
  
  // variables --> radius
  const int r = d;
  
  // objective function
  // -- we try to maximize the radius -> minimize -radius
  lp.set_c(r, -1);
  
  // lower bound -> radius needs to be positive
  lp.set_l(r, true, 0);
  
  // constraints
  // a_t * c + ||a|| * r <= b
  // a -> parameters of the d dimensional hyperplane
  // c -> variables of the center of the d dimensional ball
  // r -> the radius of the d dimensional ball
  for (int i = 0; i < n; i++) {
    int norm = 0;
    for (int j = 0; j < d; j++) {
      lp.set_a(j, i, A[i][j]);
      norm += pow(A[i][j], 2);
    }
    norm = sqrt(norm);
    lp.set_a(r, i, norm);
    lp.set_b(i, b[i]);
  }
  
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  // output solution
  if (s.is_optimal()) {
    double res = floor(CGAL::to_double(-s.objective_value()));
    cout << setprecision(0) << fixed << res << endl;
  } else if (s.is_infeasible()) { 
    cout << "none" << endl;
  } else {
    cout << "inf" << endl;
  }
  
  return true;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  while(solve()){}
}