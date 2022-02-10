///1
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <queue>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

using namespace std;

#define INF FT(LONG_MAX) * FT(LONG_MAX)

typedef CGAL::Exact_predicates_exact_constructions_kernel      K;
typedef K::Point_2                                             Point;
typedef K::FT                                                  FT;
// we want to store an index with each vertex
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int, K>      Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  DT;

void solve() {
  int n; cin >> n; // nr of trees
  int m; cin >> m; // nr of balloons
  long r; cin >> r; // balloons radius
  
  // read trees
  vector<Point> trees(n);
  for (int i = 0; i < n; i++) {
    long x, y;
    cin >> x >> y;
    trees[i] = Point(x, y);
  }
  
  // Compute Delaunay Triangulation
  DT dt;
  dt.insert(trees.begin(), trees.end());
  
  // Assign an index to each face (infinite faces have id 0)
  int id = 0;
  for (auto f = dt.finite_faces_begin(); f != dt.finite_faces_end(); f++)
    f->info() = ++id;
    
  // ================== STEP 1 ==================
  // vector of neighbors -- each element: (edge_length, neighbor_id)
  vector<vector<pair<FT, int>>> neighbors(id + 1); // 1 extra id for outside face
  for (auto f1 = dt.finite_faces_begin(); f1 != dt.finite_faces_end(); f1++) {
    int id1 = f1->info();
    
    // add takeoff edge
    FT takeoff_d = CGAL::squared_distance(dt.dual(f1), f1->vertex(0)->point());
    neighbors[0].push_back(make_pair(takeoff_d, id1));
    
    // iterate through neighbor faces
    for (int i = 0; i < 3; i++) {
      auto f2 = f1->neighbor(i);
      int id2 = dt.is_infinite(f2) ? 0 : f2->info();
      Point p1 = f1->vertex((i + 1) % 3)->point();
      Point p2 = f1->vertex((i + 2) % 3)->point();
      FT edge_len = CGAL::squared_distance(p1, p2);
      
      if (id2 == 0)
        neighbors[id2].push_back(make_pair(edge_len, id1));
      else
        neighbors[id1].push_back(make_pair(edge_len, id2));
    }
  }
  
  // ================== STEP 2 ==================
  // find the maximum radius that can escape from every face
  vector<FT> max_width(id + 1, -1);
  priority_queue<pair<FT, int>> pq;
  pq.push(make_pair(INF, 0)); // push the infinity face to the pq
  
  while (!pq.empty()) {
    // get the element on the top and retrieve its information
    auto entry = pq.top(); pq.pop();
    FT width1 = entry.first;
    int id1 = entry.second;
    
    // Check if the max_width for this face was already found
    if (max_width[id1] != -1)
      continue;
      
    max_width[id1] = width1;
    
    for (auto &neighbor : neighbors[id1]) {
      int id2 = neighbor.second;
      
      if (max_width[id2] == -1) {
        FT edge_len2 = neighbor.first;
        FT width2 = edge_len2;
        width2 = CGAL::min(width1, width2); // get the min between the radiuses
        pq.push(make_pair(width2, id2));
      }
    }
  }
  
  // ================== STEP 3 ==================
  // for each balloon, find if it can take off or not
  for (int i = 0; i < m; i++) {
    long x, y; // balloon coordinates
    cin >> x >> y;
    Point balloon(x, y);
    long s; cin >> s; // balloon radius
    FT d = (FT(s) + FT(r)) * (FT(s) + FT(r)); // squared constraint distance
    
    // check if the initial position of the balloon is already touching trees
    Point nearest_tree = dt.nearest_vertex(balloon)->point();
    FT dist = CGAL::squared_distance(balloon, nearest_tree);
    if (d > dist) {
      cout << "n";
      continue;
    }
    
    // get the face where the balloon is initially located
    auto balloon_f = dt.locate(balloon);
    int f_id = dt.is_infinite(balloon_f) ? 0 : balloon_f->info();
    
    // print output
    cout << (4 * d <= max_width[f_id] ? "y" : "n");
  }
  
  cout << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t--)
    solve();
}