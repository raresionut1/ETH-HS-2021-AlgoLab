#include <iostream>
#include <vector>
#include <climits>

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
    int E; cin >> E; // nr of East teams
    int W; cin >> W; // nr of West teams
    int M; cin >> M; // nr of non-difficult matches
    int D; cin >> D; // nr of difficult matches
    
    int P; cin >> P; // nr of total matches required
    int L; cin >> L; // nr of matches required per team
    
    if (P - E * L < 0 || P - W * L < 0) {
      cout << "No schedule!" << endl;
      int dummy;
      for (int i = 0; i < 3 * (M + D); i++)
        cin >> dummy;
      return;
    }
    
    // Create graph, edge adder class and propery maps
    graph G(2 * (E + W) + 4);
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    // auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    const int source = 2 * (E + W);
    const int sink = 2 * (E + W) + 1;
    const int aux_e = 2 * (E + W) + 2;
    const int aux_w = 2 * (E + W) + 3;
    
    const int E_offset = 0;
    const int W_offset = E;
    const int E_offset_diff = E + W;
    const int W_offset_diff = E + W + E;
    
    adder.add_edge(source, aux_e, P - E * L, 0);
    
    for (int i = 0; i < E; i++) {
      adder.add_edge(source, E_offset + i, L, 0);
      adder.add_edge(aux_e, E_offset + i, INF, 0);
      adder.add_edge(aux_e, E_offset_diff + i, INF, 0);
    }
    
    for (int i = 0; i < W; i++) {
      adder.add_edge(W_offset + i, sink, L, 0);
      adder.add_edge(W_offset + i, aux_w, INF, 0);
      adder.add_edge(W_offset_diff + i, aux_w, INF, 0);
    }
    
    adder.add_edge(aux_w, sink, P - W * L, 0);
    
    // non-difficult matches
    for (int i = 0; i < M; i++) {
      int u; cin >> u;
      int v; cin >> v;
      int r; cin >> r;
      
      adder.add_edge(E_offset + u, W_offset + v, 1, r);
    }
    
    // difficult matches
    for (int i = 0; i < D; i++) {
      int u; cin >> u;
      int v; cin >> v;
      int r; cin >> r;
      
      adder.add_edge(E_offset_diff + u, W_offset_diff + v, 1, r);
    }
    
    // Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    int cost = boost::find_flow_cost(G);
    // Iterate over all edges leaving the source to sum up the flow values.
    int flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source, G), G); e != eend; ++e)
      flow += c_map[*e] - rc_map[*e];     
    
    if (flow != P)
      cout << "No schedule!" << endl;
    else
      cout << cost << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  long t; cin >> t;
  while (t--)
    solve();
}
