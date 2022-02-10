///4
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::FT FT;
// we want to store an index with each vertex
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>    Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  DT;
typedef boost::disjoint_sets_with_storage<> UF;

bool check_conflicts(vector<Point> &points, FT sq_r) {
  DT dt;
  dt.insert(points.begin(), points.end());
  
  for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); e++)
    if (dt.segment(e).squared_length() <= sq_r)
      return false;
      
  return true;
}

void solve() {
  int n; cin >> n; // nr of stations (points)
  int m; cin >> m; // nr of clues (start and end locations)
  int r; cin >> r; // radius of stations
  FT sq_r = FT(r) * FT(r);
  
  // read the stations
  vector<Point> stations(n);
  for (int i = 0; i < n; i++) {
    int x, y;
    cin >> x >> y;
    stations[i] = Point(x, y);
  }
  
  // read the clues
  vector<Point> start_loc(m);
  vector<Point> end_loc(m);
  for (int i = 0; i < m; i++) {
    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    start_loc[i] = Point(x1, y1);
    end_loc[i] = Point(x2, y2);
  }
  
  ////////////////////////////////////////////////////////
  UF uf(n); // union find for vertices -> to find component of the vertex
  
  // Compute Delaunay Triangulation on stations
  DT dt;
  dt.insert(stations.begin(), stations.end());
  
  // assign indices to vertices
  int id = 0;
  for(auto v = dt.finite_vertices_begin(); v != dt.finite_vertices_end(); v++)
    v->info() = id++;
    
  // Iterate through all edges and save only the ones <= r
  // --> create an adajcenty list
  vector<vector<int>> adj(n);
  for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); e++) {
    if (dt.segment(e).squared_length() > sq_r)
      continue;
      
    // add edge
    int u = e->first->vertex((e->second + 1) % 3)->info();
    int v = e->first->vertex((e->second + 2) % 3)->info();
    adj[u].push_back(v);
    adj[v].push_back(u);
    
    // the vertices are in the same component
    uf.union_set(u, v);
  }
  
  // Do a BFS for each component and check if it can be 2-Colored
  bool ok = true;
  
  vector<int> colors(n, -1); // -1 == not visited | 0 = red | 1 = black
  for (int i = 0; i < n; i++) {
    if (colors[i] != -1)
      continue;
      
    queue<int> q;
    colors[i] = 0;
    q.push(i);
    
    while (!q.empty()) {
      int u = q.front(); q.pop();
      int color = colors[u];
      int next_color = color == 0 ? 1 : 0;
      
      for (int v : adj[u]) {
        if (colors[v] == -1) {
          colors[v] = next_color;
          q.push(v);
          continue;
        }
        
        if (colors[v] == color) {
          ok = false;
          break;
        }
      }
      
      if (!ok)
        break;
    }
    
    if (!ok)
      break;
  }
  
  if (ok) {
    vector<Point> reds;
    vector<Point> blacks;
    for (auto v = dt.finite_vertices_begin(); v != dt.finite_vertices_end(); ++v) {
      if (colors[v->info()] == -1)
        cout << "IMPOSSIBLE\n";
      
      if (colors[v->info()] == 0)
        reds.push_back(v->point());
      else
        blacks.push_back(v->point());
    }
    
    if (!check_conflicts(reds, sq_r) ||
        !check_conflicts(blacks, sq_r))
      ok = false;
  }
  
  /////////////////////////////////////////////////////
  for (int i = 0; i < m; i++) {
    if (!ok) {
      cout << "n";
      continue;
    }
    
    Point start = start_loc[i];
    Point end = end_loc[i];
    
    if (CGAL::squared_distance(start, end) <= sq_r) {
      cout << "y";
      continue;
    }
    
    auto v1 = dt.nearest_vertex(start);
    Point p1 = v1->point();
    int id1 = v1->info();
    
    if (CGAL::squared_distance(start, p1) > sq_r) {
      cout << "n";
      continue;
    }
    
    auto v2 = dt.nearest_vertex(end);
    Point p2 = v2->point();
    int id2 = v2->info();
    
    if (CGAL::squared_distance(end, p2) > sq_r) {
      cout << "n";
      continue;
    }
    
    if (uf.find_set(id1) == uf.find_set(id2)) {
      cout << "y";
      continue;
    } else {
      cout << "n";
      continue;
    }
  }
  cout << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  int t; cin >> t;
  while (t--)
    solve();
}