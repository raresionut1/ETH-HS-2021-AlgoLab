///1
#include <iostream>
#include <vector>
// BGL include
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

#define INF numeric_limits<int>::max()

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
    boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void solve() {
  int n; cin >> n; // nr of vertices (intersections)
  int m; cin >> m; // nr of edges (streets)
  int s; cin >> s; // nr of stores
  graph G(n + 1); // 1 extra node bc of multiple sinks
  edge_adder adder(G);
  int sink = n; // master sink index
  
  // read stores
  for (int i = 0; i < s; i++) {
    int store; cin >> store;
    // connect it to master sink
    adder.add_edge(store, sink, 1); // from, to, capacity
  }
    
  // read streets
  for (int i = 0; i < m; i++) {
    int u; cin >> u;
    int v; cin >> v;
    adder.add_edge(u, v, 1); // from, to, capacity
    adder.add_edge(v, u, 1); // (because is undirected graph)
  }
  
  // Calculate flow from source to sink
  // The flow algorithm uses the interior properties (managed in the edge adder)
  // - edge_capacity, edge_reverse (read access),
  // - edge_residual_capacity (read and write access).
  long flow = boost::push_relabel_max_flow(G, 0, sink);
  
  cout << (flow == s ? "yes" : "no") << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
  return 0;
}