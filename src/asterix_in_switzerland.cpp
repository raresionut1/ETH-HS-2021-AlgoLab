///
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
  int n; cin >> n; // nr of provinces (vertices)
  int m; cin >> m; // nr of debt relations (edges)
  
  // -- model problem as a graph --
  // (source) --supply--> (players) <--debt--> (players) --demand--> (sink)
  graph G(n + 2); // #provinces + 2
  edge_adder adder(G);
  const int source = n;
  const int sink = n + 1;
  
  long supply_sum = 0;
  // read the supplies of vertices (bank balance of each province)
  // --> in this way we can separate the provinces --> bipartite graph!
  for (int i = 0; i < n; i++) {
    int supply; cin >> supply;
    
    if (supply >= 0) {
      // if supply is positive --> connect source to vertex with supply
      adder.add_edge(source, i, supply);
      supply_sum += supply;
    } else {
      // if supply is negative --> connect vertex to sink with -supply (which is the demand)
      adder.add_edge(i, sink, -supply);
    }
  }
  
  // read the edges between vertices (debt relations)
  for (int i = 0; i < m; i++) {
    int v1; cin >> v1;
    int v2; cin >> v2;
    int debt; cin >> debt;
    
    adder.add_edge(v1, v2, debt);
  }
  
  // Calculate flow from source to sink
  long flow = boost::push_relabel_max_flow(G, source, sink);
  
  cout << (flow != supply_sum ? "yes" : "no") << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
  return 0;
}