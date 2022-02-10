///3
#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq IT; // input type
typedef CGAL::Gmpq ET; // solver type
typedef CGAL::Quadratic_program<IT> Program; // program type
typedef CGAL::Quadratic_program_solution<ET> Solution; // solution type

using namespace std;

void solve() {
  // ================== READ THE INPUT ================== 
  int n; cin >> n; // nr of new posters
  int m; cin >> m; // nr of old posters
  int h; cin >> h; // default height of the posters
  int w; cin >> w; // default width of the posters
  
  // positions of the center of the new posters 
  vector<int> x_new(n);
  vector<int> y_new(n);
  // positions of the center of the old posters
  vector<int> x_old(m);
  vector<int> y_old(m);
  
  // Read the positions of the center of the new posters
  for (int i = 0; i < n; i++)
    cin >> x_new[i] >> y_new[i];
  
  // Read the positions of the center of the old posters
  for (int i = 0; i < m; i++)
    cin >> x_old[i] >> y_old[i];
  
  // ================== SOLVE THE PROBLEM ================== 
  Program lp(CGAL::SMALLER, true, 1, false, 0); // only positive variables
  
  // constraint id counter
  int c_id = 0;
  
  // objective function
  // -- we try to minimize -2 * (w + h) * sum(a_i)
  for (int i = 0; i < n; i++)
    lp.set_c(i, -2 * (w + h));
  
  // constraints - between new posters and new posters
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      // -- the constraint is:
      // ((a1 + a2) * w / 2 <= |x2 - x1|) || ((a1 + a2) * h / 2 <= |y2 - y1|)
      // -- we can linearize it to:
      // a1 + a2 <= 2 * max (|x2 - x1| / w, |y2 - y1| / h)
      ET dx = abs(x_new[i] - x_new[j]);
      ET dy = abs(y_new[i] - y_new[j]);
      ET upper_bound = max(2 * (dx / w), 2 * (dy / h));
      
      lp.set_a(i, c_id, 1);
      lp.set_a(j, c_id, 1);
      lp.set_b(c_id, upper_bound);
      c_id++;
    }
  }
  
  // constraints - between new posters and old posters
  for (int i = 0; i < n; i++) {
    // -- the constraint is:
    // ((a1 + 1) * w / 2 <= |x2 - x1|) || ((a1 + 1) * h / 2 <= |y2 - y1|)
    // -- we can linearize it to:
    // a1 <= 2 * max (|x2 - x1| / w, |y2 - y1| / h) - 1
    ET best_upper_bound = -1;
    for (int j = 0; j < m; j++) {
      ET dx = abs(x_new[i] - x_old[j]);
      ET dy = abs(y_new[i] - y_old[j]);
      ET upper_bound = 2 * max(dx / w, dy / h) - 1;
      best_upper_bound = (best_upper_bound == -1) 
                          ? upper_bound 
                          : min(best_upper_bound, upper_bound);
    }
    
    if (best_upper_bound != -1) {
      lp.set_a(i, c_id, 1);
      lp.set_b(c_id, best_upper_bound);
      c_id++;
    }
  }
  
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  // output solution
  if (s.is_optimal()) {
    double res = ceil(CGAL::to_double(-s.objective_value()));
    cout << setprecision(0) << fixed << res << endl;
  } else {
    cout << "ERROR!" << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
}