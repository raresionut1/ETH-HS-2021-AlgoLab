///1
#include <iostream>
#include <vector>
#include <climits>
#include <tuple>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

#define INF INT_MAX

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};


void solve() {
    int C; cin >> C; // nr of cities
    int G; cin >> G; // nr of guides
    int B; cin >> B; // total budget
    int K; cin >> K; // start location
    int A; cin >> A; // final location
    
    vector<tuple<int, int, int, int>> edges(G);
    
    int start_cap = 0;
    int end_cap = 0;
    
    for (int i = 0; i < G; i++) {
      int u; cin >> u;
      int v; cin >> v;
      int cost; cin >> cost;
      int cap; cin >> cap;
      
      if (u == K)
        start_cap += cap;
      
      if (v == A)
        end_cap += cap;
      
      edges[i] = make_tuple(u, v, cost, cap);
    }
    
    int res = 0;
    int l = 0;
    int r = min(start_cap, end_cap);
    while (l <= r) {
      int m = l + (r - l) / 2;
      
      // Create graph, edge adder class and propery maps
      graph Graph(C + 2);
      edge_adder adder(Graph);
      auto c_map = boost::get(boost::edge_capacity, Graph);
      // auto r_map = boost::get(boost::edge_reverse, Graph);
      auto rc_map = boost::get(boost::edge_residual_capacity, Graph);
      
      const int source = C;
      const int sink = C + 1;
    
      adder.add_edge(source, K, m, 0);
      
      for (int i = 0; i < G; i++) {
        int u = get<0>(edges[i]);
        int v = get<1>(edges[i]);;
        int cost = get<2>(edges[i]);;
        int cap = get<3>(edges[i]);;
        
        adder.add_edge(u, v, cap, cost);
      }
      
      adder.add_edge(A, sink, m, 0);
      
      // Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
      boost::successive_shortest_path_nonnegative_weights(Graph, source, sink);
      int cost = boost::find_flow_cost(Graph);
      // Iterate over all edges leaving the source to sum up the flow values.
      int flow = 0;
      out_edge_it e, eend;
      for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source, Graph), Graph); e != eend; ++e)
        flow += c_map[*e] - rc_map[*e];
        
      if (cost <= B) {
        res = flow;
        l = m + 1;
      } else {
        r = min(m - 1, flow);
      }
    }
    
    cout << res << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  long t; cin >> t;
  while (t--)
    solve();
}
