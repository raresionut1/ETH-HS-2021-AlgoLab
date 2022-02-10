///3
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
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

bool check(int i, int j, int n, vector<vector<int>> &board) {
  // check if position is out of bounds
  // and if position is present (not hole)
  return i >= 0 && j >= 0
         && i < n && j < n
         && board[i][j] == 1;
}

void solve() {
  int n; cin >> n; // chessboard size
  vector<vector<int>> board(n, vector<int>(n));
  int nr_present = 0;
  
  // read the chessboard: 1 --> present | 0 --> hole
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      cin >> board[i][j];
      nr_present += board[i][j]; // increment nr_present counter
    }
  }
  
  // build graph
  graph G(pow(n, 2) + 2);
  edge_adder adder(G);
  // auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const int source = pow(n, 2);
  const int sink = pow(n, 2) + 1;
  
  // -- add edges --
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      // skip position if it is a hole
      if (board[i][j] == 0)
        continue;
        
      // compute current position's index
      int idx = i * n + j;
      
      // check parity
      if ((i + j) % 2 == 0) {
        // add an edge between source and every EVEN position
        adder.add_edge(source, idx, 1);
      } else {
        // add an edge between every ODD position and sink
        adder.add_edge(idx, sink, 1);
      }
      
      // if the parity is even, we also want to add edges between
      // the current position and all conflicting positions
      if ((i + j) % 2 == 0) {
        vector<pair<int, int>> conflicting_pos = 
          {
            {i - 1, j - 2},
            {i - 1, j + 2},
            {i + 1, j - 2},
            {i + 1, j + 2},
            {i - 2, j - 1},
            {i - 2, j + 1},
            {i + 2, j - 1},
            {i + 2, j + 1}
          };
          
        for(auto pos : conflicting_pos) {
          // check if the pair of positions is
          // not out of bounds and both of them
          // are present (not holes)
          if (!check(pos.first, pos.second, n, board))
            continue;
            
          int idx_conflicting = pos.first * n + pos.second;
            
          adder.add_edge(idx, idx_conflicting, 1);
        }
      }
    }
  }
  
  // Find maxflow
  int flow = boost::push_relabel_max_flow(G, source, sink);
  // # max matching == # max flow (in bipartite graph)
  // # min vertex cover == # max matching (in bipartite graph)
  // # min vertex cover == # vertices - # max independent set
  cout << nr_present - flow << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t--)
    solve();
}