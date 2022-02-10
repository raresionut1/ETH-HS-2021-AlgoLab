///
// STL includes
#include <iostream>
#include <vector>
#include <limits>

// BGL includes
#include <boost/pending/disjoint_sets.hpp>

using namespace std;

#define INF INT_MAX

typedef boost::disjoint_sets_with_storage<> UF;

struct Edge {
  int u;
  int v;
  int w;
};

inline bool operator<(const Edge &e1, const Edge &e2) {
  return e1.w < e2.w;
}

// we need to implement Kruskal since we give the edges already sorted
int mst_cost(int n, vector<Edge> &edges, int exclude_edge, vector<int> &mst_edges_ids) {
  UF uf(n);
  int total_cost = 0;
  
  for (int i = 0; i < ((int) edges.size()); i++) {
    if (i == exclude_edge)
      continue;
      
    int u = edges[i].u; 
    int v = edges[i].v;
    int w = edges[i].w;
    
    if (uf.find_set(u) != uf.find_set(v)) {
      uf.union_set(u, v);
      mst_edges_ids.push_back(i);
      total_cost += w;
      
      // check if MST was formed => STOP
      if (--n == 1)
        return total_cost;
    }
  }
  
  return total_cost;
}

void solve() {
  int n; cin >> n;
  int tatooine; cin >> tatooine; // actually irrelevant to solve the problem
  
  vector<Edge> edges;
  
  // read edges
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      int w; cin >> w;
      
      Edge edge;
      edge.u = i;
      edge.v = j;
      edge.w = w;
      
      edges.push_back(edge);
    }
  }
  
  // sort the edges (for fast MST computation)
  sort(edges.begin(), edges.end());
  
  // compute best MST
  vector<int> mst_edges_ids;
  mst_cost(n, edges, -1, mst_edges_ids);
  
  // get the min cost among other MST than the best one
  int min_cost = INF;

  for (int id : mst_edges_ids) {
    vector<int> mst2_edges;
    int cost = mst_cost(n, edges, id, mst2_edges);
    min_cost = min(min_cost, cost);
  }
  
  cout << min_cost << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t--)
    solve();
}