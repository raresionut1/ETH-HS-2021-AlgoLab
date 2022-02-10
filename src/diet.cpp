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
  int n; cin >> n; // nr of nutrients
  int m; cin >> m; // nr of food items
  if (n == 0 && m == 0)
    return false;
    
  vector<pair<int, int>> constraints(n); // nutrients constraints
  vector<int> p(m); // the cost of each food
  vector<vector<int>> C(m, vector<int>(n)); // the nutrients of each food
  
  // Read the nutrients bounds
  for (int i = 0; i < n; i++) {
    int lower_bound; cin >> lower_bound;
    int upper_bound; cin >> upper_bound;
    constraints[i] = make_pair(lower_bound, upper_bound);
  }
  
  // Read the food cost and nutrients
  for (int j = 0; j < m; j++) {
    cin >> p[j];
    
    for (int i = 0; i < n; i++)
      cin >> C[j][i];
  }
  
  // ================== SOLVE THE PROBLEM ================== 
  Program lp(CGAL::SMALLER, true, 0, false, 0); // only positive variables
  
  // objective function
  // -- we try to minimize sum(a_j * p_j)
  for (int j = 0; j < m; j++)
    lp.set_c(j, p[j]);
    
  int row = 0;
    
  // constraints
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      lp.set_a(j, 2 * i,     -C[j][i]); // lower bound
      lp.set_a(j, 2 * i + 1,  C[j][i]); // upper bound
    }
    
    lp.set_b(2 * i,     -constraints[i].first); // lower bound
    lp.set_b(2 * i + 1,  constraints[i].second); // upper bound
  }
  
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  // output solution
  if (s.is_optimal()) {
    double res = floor(CGAL::to_double(s.objective_value()));
    cout << setprecision(0) << fixed << res << endl;
  } else if (s.is_infeasible()) { 
    cout << "No such diet." << endl;
  } else {
    cout << "ERROR!" << endl;
  }
  
  return true;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  while(solve()){}
}