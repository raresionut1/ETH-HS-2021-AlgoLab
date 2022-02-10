///4
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

using namespace std;

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::FT FT;
// we want to store an index with each vertex
typedef CGAL::Triangulation_vertex_base_with_info_2<long,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  DT;
typedef boost::disjoint_sets_with_storage<>                    UF;

struct Edge {
  long u;
  long v;
  long l;
};

long inline operator< (const Edge &e1, const Edge &e2) {
  return e1.l < e2.l;
} 

int compute_nr_fam(vector<int> &clusters, int k) {
  switch (k) {
    case 1:
      return clusters[1];
      
    case 2:
      return clusters[2] + clusters[1] / 2;
    
    case 3:
      return clusters[3] +
             min(clusters[1], clusters[2]) +
             (clusters[2] - min(clusters[1], clusters[2])) / 2 +
             (clusters[1] - min(clusters[1], clusters[2])) / 3;
    
    case 4:
      return clusters[4] +
             min(clusters[1], clusters[3]) +
             (clusters[2] +  clusters[3] - min(clusters[1], clusters[3])) / 2 +
             (((clusters[2] + clusters[3] - min(clusters[1], clusters[3])) % 2) * 2  + clusters[1] - min(clusters[1], clusters[3])) / 4;
  }
  
  return -1;
}

void solve() {
  long n; cin >> n; // nr of tents (nodes)
  int k; cin >> k; // nr of tents per family (node regions)
  long f0; cin >> f0; // nr of families for q1
  long s0; cin >> s0; // fiven minimum squared distance for q2
  
  // read nodes
  vector<Point> points(n);
  for (long i = 0; i < n; i++) {
    long x, y;
    cin >> x >> y;
    points[i] = Point(x, y);
  }
  
  ////////////////////////////////////////////////////////////
  DT dt;
  dt.insert(points.begin(), points.end());
  
  // assign indexes to vertices
  long idx = 0;
  for (auto v = dt.finite_vertices_begin(); v != dt.finite_vertices_end(); v++)
    v->info() = idx++;
    
  // get DT edges and sort them by length
  vector<Edge> edges;
  for (auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); e++) {
    long i1 = e->first->vertex((e->second+1)%3)->info();
    long i2 = e->first->vertex((e->second+2)%3)->info();
    
    // ensure smaller index comes first
    if (i1 > i2)
      swap(i1, i2);
      
    edges.push_back({i1, i2, (long)dt.segment(e).squared_length()});
  }
  
  sort(edges.begin(), edges.end());
  
  //////////////////// -- Q1 & Q2 -- ///////////////////////
  vector<int> comp_size(n, 1);
  vector<int> clusters(k + 1, 0);
  clusters[1] = n;
  
  // Union find
  UF uf(n);
  long res_s = -1;
  int res_f = -1;
  
  for (Edge e : edges) {
    int nr_fam = compute_nr_fam(clusters, k);
    
    if (nr_fam >= f0) {
      res_s = e.l;
    }

    if(e.l >= s0) {
      res_f = max(res_f, nr_fam);
    }
    
    int id1_old = uf.find_set(e.u);
    int id2_old = uf.find_set(e.v);
    if (id1_old != id2_old) {
      clusters[min(comp_size[id1_old], k)]--;
      clusters[min(comp_size[id2_old], k)]--;
      
      uf.link(id1_old, id2_old);
      
      int id_new = uf.find_set(e.u);
      comp_size[id_new] = comp_size[id1_old] + comp_size[id2_old];
      clusters[min(comp_size[id_new], k)]++;
    }
  }
  
  cout << res_s << " " << res_f << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  long t; cin >> t;
  while (t--)
    solve();
}