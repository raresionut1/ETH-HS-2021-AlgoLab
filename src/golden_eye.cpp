///
#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
// we want to store an index with each vertex
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>    Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  DT;
typedef boost::disjoint_sets_with_storage<> disjoint_sets;

struct Edge {
  int u, v; // indices of the vertices
  double l; // squared length of the edge
};

inline bool operator<(const Edge &e1, const Edge &e2) {
  return e1.l < e2.l;
}

void solve() {
  int n; cin >> n; // nr of jammers
  int m; cin >> m; // nr of missions
  double p; cin >> p; // initial power consumption
  
  vector<Point> jammers(n); // jammers coordinates
  
  // read jammers coordinates
  for (int i = 0; i < n; i++) {
    int x; cin >> x;
    int y; cin >> y;
    jammers[i] = Point(x, y);
  }
  
  vector<Point> missions_s(m); // missions starting points
  vector<Point> missions_t(m); // missions target points
  // read mission points
  for (int i = 0; i < m; i++) {
    int x0, y0, x1, y1; 
    cin >> x0 >> y0 >> x1 >> y1;
    
    missions_s[i] = Point(x0, y0);
    missions_t[i] = Point(x1, y1);
  }
  
  // Compute Delaunay Triangulation on jammers centers
  DT dt;
  dt.insert(jammers.begin(), jammers.end());
  
  // assign indices to vertexes
  int id = 0;
  for(auto v = dt.finite_vertices_begin(); v != dt.finite_vertices_end(); v++)
    v->info() = id++;

  
  // Iterate through all edges and save them and their squared length
  vector<Edge> edges;
  for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); e++) {
    Edge edge;
    edge.u = e->first->vertex((e->second + 1) % 3)->info();
    edge.v = e->first->vertex((e->second + 2) % 3)->info();
    edge.l = dt.segment(e).squared_length();
    edges.push_back(edge);
  }
  
  // sort the edges by length
  sort(edges.begin(), edges.end());
  
  // compute disjoint sets with power == p
  disjoint_sets ds_p(n);
  for (auto edge : edges) {
    if (edge.l > p)
      break;
    ds_p.union_set(edge.u, edge.v);
  }
  
  double a = 0; // value of minimum power needed for all missions
  disjoint_sets ds_a(n); // disjoint sets with power == a
  int idx_a = 0; // index of current edge
  
  double b = 0; // value of minimum power needed for same missions as P power
  disjoint_sets ds_b(n); // disjoint sets with power == b
  int idx_b = 0; // index of current edge
  
  string output_p = ""; // string for the output when power == p
  
  // iterate over missions
  for (int i = 0; i < m; i++) {
    // get mission points
    Point s = missions_s[i];
    Point t = missions_t[i];
    
    // get the nearest jammers
    auto nearest_to_s = dt.nearest_vertex(s);
    auto nearest_to_t = dt.nearest_vertex(t);
    
    // get the nearest jammers indexes
    int jammer_s_idx = dt.nearest_vertex(s)->info();
    int jammer_t_idx = dt.nearest_vertex(t)->info();
    
    // get distance to nearest jammers
    double dist_nearest_s = CGAL::squared_distance(s, nearest_to_s->point());
    double dist_nearest_t = CGAL::squared_distance(t, nearest_to_t->point());
    
    // compute the power needed
    double w = 4 * max(dist_nearest_s, dist_nearest_t);
    
    // check if for power == p, the mission can be completed successfully or not
    if (w <= p && ds_p.find_set(jammer_s_idx) == ds_p.find_set(jammer_t_idx)) {
      output_p = output_p + "y";
      
      // if it can, compute what is the minimum power required
      // (just considering the distance between the start and target points and their jammers)
      b = max(b, w);
      while(ds_b.find_set(jammer_s_idx) != ds_b.find_set(jammer_t_idx)) {
        ds_b.union_set(edges[idx_b].u, edges[idx_b].v);
        idx_b++;
      }
    } else {
      output_p = output_p + "n";
    }
    
    // compute what is the minimum power required to complete the current mission + all the previous missions
    // (just considering the distance between the start and target points and their jammers)
    a = max(a, w);
    while(ds_a.find_set(jammer_s_idx) != ds_a.find_set(jammer_t_idx)) {
      ds_a.union_set(edges[idx_a].u, edges[idx_a].v);
      idx_a++;
    }
  }
  
  // get the minimum power required for both cases
  b = max(b, edges[idx_b - 1].l);
  a = max(a, edges[idx_a - 1].l);
  
  // print output
  cout << fixed << setprecision(0) << output_p << endl << a << endl << b << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  
  int t; cin >> t;
  while (t--)
    solve();
}