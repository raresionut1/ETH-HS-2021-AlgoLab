///1
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

// CGAL general typedefs
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;

// Delaunay typedefs
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>     Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  DT;

// Union Find typedef
typedef boost::disjoint_sets_with_storage<> UF;

struct Edge {
  int u; // first node index (always a tree)
  int v; // second node index (if it is -1, then it is a bone; otherwise tree)
  double sql; // squared length
};

inline bool operator<(const Edge &e1, const Edge &e2) {
  return e1.sql < e2.sql;
}

void solve() {
  // ============================= READ INPUT =============================
  int n; cin >> n; // nr of trees (nodes)
  int m; cin >> m; // nr of bones
  double s; cin >> s; // s == 4 * r^2
  int k; cin >> k; // mininum nr of bones
  
  // read trees
  vector<pair<Point, int>> trees(n);
  for (int i = 0; i < n; i++) {
    Point tree; cin >> tree;
    trees[i] = make_pair(tree, i);
  }
  
  // read bones
  vector<Point> bones(m);
  for (int i = 0; i < m; i++)
    cin >> bones[i];
  
  // ============================= SOLVE PROBLEM =============================
  // compute Delaunay Triangulation on trees
  // giving them indices
  DT dt;
  dt.insert(trees.begin(), trees.end());

  // extract edges from DT
  vector<Edge> edges;
  for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e) {
    // create edge and add it
    Edge edge;
    edge.u = e->first->vertex((e->second+1)%3)->info(); // first tree
    edge.v = e->first->vertex((e->second+2)%3)->info(); // second tree
    edge.sql = dt.segment(e).squared_length();
    
    edges.push_back(edge);
  }
  
  // add also an edge from each bone to its nearest tree
  for (int i = 0; i < m; i++) {
    auto nearest_tree = dt.nearest_vertex(bones[i]);
    
    // create edge and add it
    Edge edge;
    edge.u = nearest_tree->info(); // tree index
    edge.v = -1; // bone index
    edge.sql = 4 * CGAL::squared_distance(bones[i], nearest_tree->point());
    
    edges.push_back(edge);
  }
  
  // sort edges by their length
  sort(edges.begin(), edges.end());
  
  // ------------------ ANSWER QUESTION 1 & 2 ------------------
  int a = -1; // maximum number of bones using default radius
  long q = -1; // minimum radius to cover at least k bones
  
  // union find for trees
  UF uf(n);
  
  // keep track of nr of bones in each component
  vector<int> component_bones(n, 0);
  // also keep track of the maximum value of bones in any component
  int max_bones = 0;
  
  // process edges in order of increasing length
  for (Edge e : edges) {
    // check if we need set 'a' value
    if (a == -1 && s < e.sql)
      a = max_bones;
    
    // check type of edge
    if (e.v != -1) {
      // (Tree -- Tree)
      // get trees
      int t1 = e.u;
      int t2 = e.v;
      // determine the component of each tree
      int c1 = uf.find_set(t1);
      int c2 = uf.find_set(t2);
      // get the nr of bones in each component
      int nr1 = component_bones[c1];
      int nr2 = component_bones[c2];
      // merge components (and their number of bones)
      if (c1 != c2) {
        uf.link(c1, c2);
        int c_new = uf.find_set(t1);
        component_bones[c_new] = nr1 + nr2;
        // update max_bones counter if necessary
        max_bones = max(max_bones, nr1 + nr2);
      }
    } else {
      // (Tree -- Bone)
      // get tree and bone
      int t = e.u;
      // find its component and increase bone counter
      int c = uf.find_set(t);
      component_bones[c]++;
      // update max_bones counter if necessary
      max_bones = max(max_bones, component_bones[c]);
    }
    
    // update 'q' value, depending on max_bones and k
    if (q == -1 && max_bones >= k)
      q = e.sql; 
    
    // early exit
    // check if we can stop expanding
    if (a != -1 && q != -1)
      break;
  }
  
  if (a == -1)
    a = max_bones;
  
  // ============================= PRINT OUTPUT =============================
  cout << a << " " << q << endl;
}

int main() 
{
  std::ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  std::size_t t;
  for (std::cin >> t; t > 0; --t) solve();
  return 0;
}
