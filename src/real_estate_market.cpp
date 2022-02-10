#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

using namespace std;

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
  int N; cin >> N; // nr of interested buyers
  int M; cin >> M; // nr of sites
  int S; cin >> S; // nr of states
  
  // maximum numbers of sites per state
  vector<int> state_limits(S);
  for (int i = 0; i < S; i++) {
    cin >> state_limits[i];
  }
  
  // states of each site
  vector<int> states(M);
  for (int i = 0; i < M; i++) {
    int s; cin >> s;
    states[i] = s - 1; // states start from 1 --> decrement its index
  }
  
  // the bid of each buyer
  vector<vector<int>> bids(N, vector<int>(M));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      cin >> bids[i][j];
    }
  }
  
  // Create graph, edge adder class and propery maps
  graph G(N + M + S + 2);
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  // vertex indexes and offsets
  const int source = N + M + S;
  const int sink = N + M + S + 1;
  const int off_buyers = 0;
  const int off_sites = N;
  const int off_states = N + M;
  
  int bid_upper_bound = 100;
  
  // Add the edges
  for (int i = 0; i < N; i++) {
    // add edge from source to buyer: capacity == 1, cost == 0
    adder.add_edge(source, off_buyers + i, 1, 0);
    
    // add edge from buyer to sites: capacity == 1, cost == bid_upper_bound - bid
    for (int j = 0; j < M; j++)
      adder.add_edge(off_buyers + i, off_sites + j, 1, bid_upper_bound - bids[i][j]);
  }
  
  // add edge from sites to states: capacity == 1, cost == 0
  for (int i = 0; i < M; i++)
    adder.add_edge(off_sites + i, off_states + states[i], 1, 0);
    
  // add edge from states to sink: capacity == sites limit, cost == 0
  for (int i = 0; i < S; i++)
    adder.add_edge(off_states + i, sink, state_limits[i], 0);
    
  // Run the algorithm
  // Min Cost Max Flow with cycle_canceling
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  
  // Iterate over all edges leaving the source to sum up the flow values.
  int s_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e)
    s_flow += c_map[*e] - rc_map[*e];   
    
  // Compute total cost
  int cost = s_flow * bid_upper_bound - boost::find_flow_cost(G);
    
  // print output
  cout << s_flow << " " << cost << "\n";
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
}