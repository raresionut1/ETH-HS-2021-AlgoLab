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
  int n; cin >> n; // nr of players
  int m; cin >> m; // nr of rounds
  
  // -- model problem as a graph --
  // (source) --1p--> (rounds) --1p--> (players) --final score--> sink
  graph G(m + n + 2); // #rounds + #players + source + sink
  edge_adder adder(G);
  // rounds  - {0 ... m - 1}
  // players - {m ... m + n - 1}
  int source = m + n;
  int sink = m + n + 1;
  
  // iterate through rounds
  for (int i = 0; i < m; i++) {
    // connect the source to the current game (with capacity 1)
    adder.add_edge(source, i, 1);
    
    // read rounds
    int a; cin >> a; // player A
    int b; cin >> b; // player B
    int c; cin >> c; // 1 => player A won | 2 => player B won | 0 => unknown
    
    // indexes of the players in the graph
    int idx_a = m + a;
    int idx_b = m + b;
    
    // check the result
    switch (c) {
      // player A won => add edge to A in the graph
      case 1:
        adder.add_edge(i, idx_a, 1);
        break;
        
      // player B won => add edge to B in the graph
      case 2:
        adder.add_edge(i, idx_b, 1);
        break;
        
      // we don't know who won => add edges in the graph to both A and B
      case 0:
        adder.add_edge(i, idx_a, 1);
        adder.add_edge(i, idx_b, 1);
        break;
        
      default:
        cout << "ERROR" << endl;
    }
  }
  
  int total_points = 0;
  
  // iterate through final scores
  for (int i = 0; i < n; i++) {
    int s_i; cin >> s_i; // score of player i
    int idx_i = m + i; // index of the player i in the graph
    
    // connect the cuurent player to the sink (with capacity == final score)
    adder.add_edge(idx_i, sink, s_i);
    total_points += s_i;
  }
  
  // Calculate flow from source to sink
  long flow = boost::push_relabel_max_flow(G, source, sink);
  
  cout << (flow == m && total_points == m ? "yes" : "no") << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
  return 0;
}