///1
#include <iostream>
#include <vector>

// BGL include
#include <boost/graph/adjacency_list.hpp>
// BGL flow include
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;

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
  int l; cin >> l; // nr of locations (vertices)
  int p; cin >> p; // nr of paths (edges)
  
  graph G(l + 2); // 1 extra nodse bc of multiple sources, sinks
  edge_adder adder(G);
  const int source = l; // master source index
  const int sink = l + 1; // master sink index
  
  vector<int> supply(l); // supply of soldiers vector for each location
  vector<int> demand(l); // demand of soldiers vector for each location
  
  // read locations
  for (int i = 0; i < l; i++) {
    cin >> supply[i]; // nr of soldiers stationed at the location
    cin >> demand[i]; // nr of soldiers needed to defend the location
  }
  
  // read paths
  for (int j = 0; j < p; j++) {
    // f and t can be equal
    int f; cin >> f; // edge start
    int t; cin >> t; // edge end
    int c; cin >> c; // nr of minimum traversals
    int C; cin >> C; // nr of maximum traversals
    
    // adjust demand, supply and capacity
    demand[f] += c; // d_f = d_f + c
    supply[t] += c; // s_t = s_t + c
    int capacity = C - c; // c(e) = C - c
    
    adder.add_edge(f, t, capacity);
  }
    
  // add edges for the supply and demand
  for (int i = 0; i < l; i++) {
    adder.add_edge(source, i, supply[i]); // source --supply--> vertex
    adder.add_edge(i, sink, demand[i]); // vertex --demand--> sink
  }
  
  // compute sum of demands
  long demands_sum = 0;
  for (int i = 0; i < l; i++)
    demands_sum += demand[i];
  
  // Calculate flow from source to sink
  long flow = boost::push_relabel_max_flow(G, source, sink);
  
  // Max flow should be equal to sum of demands
  cout << (flow == demands_sum ? "yes" : "no") << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
  return 0;
}