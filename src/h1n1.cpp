///i
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <queue>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

using namespace std;

#define INF numeric_limits<long>::max()

typedef CGAL::Exact_predicates_inexact_constructions_kernel    K;
typedef K::Point_2                                             Point;
// we want to store an index with each vertex
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K>      Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  DT;

void solve(int n) {
  // n == nr of infected people
  vector<Point> infected(n);
  // infected people coordinates
  for (int i = 0; i < n; i++) {
    long x; cin >> x;
    long y; cin >> y;
    infected[i] = Point(x, y);
  }
  
  // Compute Delaunay Triangulation
  DT dt;
  dt.insert(infected.begin(), infected.end());
  
  // Assign an index to each face (infinite faces have id 0)
  int id = 0;
  for (auto f = dt.finite_faces_begin(); f != dt.finite_faces_end(); f++)
    f->info() = ++id;
    
  // ================== STEP 1 ==================
  // vector of neighbors -- each element: (edge_length, neighbor_id)
  vector<vector<pair<long, int>>> neighbors(id + 1);
  for (auto f1 = dt.finite_faces_begin(); f1 != dt.finite_faces_end(); f1++) {
    int id1 = f1->info();
    
    // iterate through neighbor faces
    for (int i = 0; i < 3; i++) {
      auto f2 = f1->neighbor(i);
      int id2 = dt.is_infinite(f2) ? 0 : f2->info();
      Point p1 = f1->vertex((i + 1) % 3)->point();
      Point p2 = f1->vertex((i + 2) % 3)->point();
      long edge_len = CGAL::squared_distance(p1, p2);
      
      neighbors[id2].push_back(make_pair(edge_len, id1));
      neighbors[id1].push_back(make_pair(edge_len, id2));
    }
  }
  
  // ================== STEP 2 ==================
  // find the maximum radius that can escape from every face
  vector<double> max_radius(id + 1, -1);
  priority_queue<pair<double, int>> pq;
  pq.push(make_pair(INF, 0)); // push the infinity face to the pq
  
  while (!pq.empty()) {
    // get the element on the top and retrieve its information
    auto entry = pq.top(); pq.pop();
    double radius1 = entry.first;
    int id1 = entry.second;
    
    // Check if the max_radius for this face was already found
    if (max_radius[id1] != -1)
      continue;
      
    max_radius[id1] = radius1;
    
    for (auto neighbor : neighbors[id1]) {
      int id2 = neighbor.second;
      
      if (max_radius[id2] == -1) {
        double edge_len2 = neighbor.first;
        double radius2 = edge_len2;
        radius2 = CGAL::min(radius1, radius2); // get the min between the radiuses
        pq.push(make_pair(radius2, id2));
      }
    }
  }
  
  // ================== STEP 3 ==================
  // for each user, find if he can escape or not 
  
  // nr of users that try to escape
  int m; cin >> m;
  // starting coordinates of the users
  for (int i = 0; i < m; i++) {
    long x; cin >> x;
    long y; cin >> y;
    long d; cin >> d; // squared radius
    Point user = Point(x, y);
    
    // check if the initial position of the user is already touching infected
    Point nearest_infected = dt.nearest_vertex(user)->point();
    double dist = CGAL::squared_distance(user, nearest_infected);
    if (d > dist) {
      cout << "n";
      continue;
    }
    
    // get the face where the user is initially located
    auto user_f = dt.locate(user);
    int user_id = dt.is_infinite(user_f) ? 0 : user_f->info();
    
    // print output
    cout << (4 * d <= max_radius[user_id] ? "y" : "n");
  }
  
  cout << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  int n;
  while (cin >> n && n != 0)
    solve(n);
}