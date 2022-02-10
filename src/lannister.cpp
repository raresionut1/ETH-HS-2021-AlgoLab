///4
#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;

typedef long IT; // input type
typedef CGAL::Gmpz ET; // solver type
typedef CGAL::Quadratic_program<IT> Program; // program type
typedef CGAL::Quadratic_program_solution<ET> Solution; // solution type

void solve() {
  // ================== READ THE INPUT ================== 
  int n; cin >> n; // nr of noble houses
  int m; cin >> m; // nr of common houses
  long s; cin >> s; // max allowable sum of lengths of all sewage pipes; -1 = inf

  vector<pair<int, int>> noble(n); // pos of noble houses
  vector<pair<int, int>> common(m); // pos of common houses
  
  // Read the positions of noble houses
  for (int i = 0; i < n; i++) {
    int x; cin >> x;
    int y; cin >> y;
    noble[i] = make_pair(x, y);
  }
  
  // Read the positions of common houses
  for (int i = 0; i < m; i++) {
    int x; cin >> x;
    int y; cin >> y;
    common[i] = make_pair(x, y);
  }
  
  // ================== SOLVE THE PROBLEM ================== 
  Program lp(CGAL::SMALLER, false, 0, false, 0); // no bounds
  
  // l1: a1*x + b1*y + c1 = 0 --> fresh water pipe
  // l2: a2*x + b2*y + c2 = 0 --> sewage pipe
  
  // l1 not vertical => b1 != 0 --> force b1 to 1
  // l2 not horizontal => a2 != 0 --> force a2 to 1
  // l1 _|_ l2 => (a1/b1) * (a2/b2) = -1 => a1/b2 = -1 => a1 = -b2
  
  // l1: -b2*x + y + c1 = 0 --> fresh water pipe
  // l2: x + b2*y + c2 = 0 --> sewage pipe
  
  // variables
  const int b2 = 0;
  const int c1 = 1;
  const int c2 = 2;
  const int l = 3;
  
  // constraint index counter
  int c_nr = 0;
  
  // ========= Cersei's constraint =========
  // The sewage canal (l2) should separate noble and common houses
  
  // -> noble houses should be on the left of l2: x + b2*y + c2
  for (int i = 0; i < n; i++) {
    // h.x + b2*h.y + c2 <= 0
    // --> b2*h.y + c2 <= -h.x
    lp.set_a(b2, c_nr, noble[i].second);
    lp.set_a(c2, c_nr, 1);
    lp.set_b(c_nr, -noble[i].first);
    c_nr++;
  }
  
  // -> common houses should be on the right of l2: x + b2*y + c2
  for (int i = 0; i < m; i++) {
    // h.x + b2*h.y + c2 >= 0 
    // --> -b2*h.y -c2 <= h.x
    lp.set_a(b2, c_nr, -common[i].second);
    lp.set_a(c2, c_nr, -1);
    lp.set_b(c_nr, common[i].first);
    c_nr++;
  }
  
  // Check if Cersei's constraint can't be fulfilled
  Solution sol = CGAL::solve_linear_program(lp, ET());

  if (sol.is_infeasible()) {
    cout << "Yuck!" << endl;
    return;
  }
  
  // ========= Tywin's constraint =========
  // The sum of the lengths of all sewage pipes must not exceed s
  
  // Check if s is not infinite
  if (s != -1) {
    // we assumed that a2 == 1 and that
    // noble houses are to the left of l2 and common houses to the right of l2
    // => we can model sum(dist(sewage_pipe, l2)) <= s as:
    // (sum(xc) - sum(xn)) + b2*(sum(yc) - sum(yn)) + c2*(m - n) <= s
    // => b2*(sum(yc) - sum(yn)) + c2*(m - n) <= s - (sum(xc) - sum(xn))
    pair<long, long> sum_n = make_pair(0, 0);
    pair<long, long> sum_c = make_pair(0, 0);
    
    // compute the sum of noble houses' coordinates
    for (int i = 0; i < n; i++) {
      sum_n.first += noble[i].first;
      sum_n.second += noble[i].second;
    }
    
    // compute the sum of common houses' coordinates
    for (int i = 0; i < m; i++) {
      sum_c.first += common[i].first;
      sum_c.second += common[i].second;
    }
    
    // constraints
    lp.set_a(b2, c_nr, sum_c.second - sum_n.second);
    lp.set_a(c2, c_nr, m - n);
    lp.set_b(c_nr, s - (sum_c.first - sum_n.first));
    
    c_nr++;
    
    // Check if Tywin's constraint can't be fulfilled
    sol = CGAL::solve_linear_program(lp, ET());
  
    if (sol.is_infeasible()) {
      cout << "Bankrupt!" << endl;
      return;
    }
  }
  
  // ========= Jaime's constraint =========
  // Minimize the longest water pipe
  lp.set_c(l, 1);
  
  // Make sure the length of the longest pipe (l) is positive
  lp.set_l(l, true, 0);
  
  // add constraints for the noble houses
  for (int i = 0; i < n; i++) {
    // length of water pipe <= l can be modeled as:
    // |h.y - b2*h.x + c1| <= l
    // --> -b2*h.x + c1 - l <= -h.y
    // -->  b2*h.x - c1 - l <= h.y
    
    lp.set_a(b2, c_nr, -noble[i].first);
    lp.set_a(c1, c_nr, 1);
    lp.set_a(l, c_nr, -1);
    lp.set_b(c_nr, -noble[i].second);

    c_nr++;
    
    lp.set_a(b2, c_nr, noble[i].first);
    lp.set_a(c1, c_nr, -1);
    lp.set_a(l, c_nr, -1);
    lp.set_b(c_nr, noble[i].second);

    c_nr++;
  }
  
  // add constraints for the common houses
  for (int i = 0; i < m; i++) {
    // we do the same thing as for the noble houses
    
    lp.set_a(b2, c_nr, -common[i].first);
    lp.set_a(c1, c_nr, 1);
    lp.set_a(l, c_nr, -1);
    lp.set_b(c_nr, -common[i].second);

    c_nr++;
    
    lp.set_a(b2, c_nr, common[i].first);
    lp.set_a(c1, c_nr, -1);
    lp.set_a(l, c_nr, -1);
    lp.set_b(c_nr, common[i].second);

    c_nr++;
  }
  
  // solve the program, using ET as the exact type
  sol = CGAL::solve_linear_program(lp, ET());
  
  // output solution
  if (sol.is_optimal()) {
    double res = ceil(CGAL::to_double(sol.objective_value()));
    cout << setprecision(0) << fixed << res << endl;
  } else { 
    cout << "ERROR!" << endl;
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- >0){solve();}
}