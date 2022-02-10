#include <iostream>
#include <vector>
#include <algorithm>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;
// Interior Property Maps
typedef  boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void solve() {
  int n; cin >> n; // nr of vertices
  int m; cin >> m; // nr of edges
  int s; cin >> s; // nr of starting locations
  int d; cin >> d; // nr of given destinations
  
  // build graph
  graph G(2 * n + 2);
  edge_adder adder(G);
  int source = 2 * n;
  int sink = 2 * n + 1;
  
  // add edges
  for (int i = 0; i < n; i++) {
    adder.add_edge(i, i + n, 1);
  }
  
  for (int i = 0; i < m; i++) {
    int u; cin >> u;
    int v; cin >> v;
    adder.add_edge(u + n, v, 1);
  }
  
  for (int i = 0; i < s; i++) {
    int si; cin >> si;
    adder.add_edge(source, si, 1);
  }
  
  for (int i = 0; i < d; i++) {
    int di; cin >> di;
    adder.add_edge(di + n, sink, 1);
  }
  
  // Find a min cut via maxflow
  int flow = boost::push_relabel_max_flow(G, source, sink);
  cout << flow << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t--) {
    solve();
  }
}