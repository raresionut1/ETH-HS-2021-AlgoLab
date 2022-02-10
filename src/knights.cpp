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
  int m; cin >> m; // nr of columns
  int n; cin >> n; // nr of rows
  int k; cin >> k; // nr of knights
  int c; cin >> c; // vertex (intersection) capacity
  vector<vector<int>> grid(n, vector<int>(m)); // the cave grid denoting intersections ids
  vector<pair<int, int>> knights(k); // knights positions
  
  // build grid
  int vertex_idx = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
      grid[i][j] = vertex_idx++;
      
  // read knights positions
  for (int i = 0; i < k; i++) {
    int x; cin >> x;
    int y; cin >> y;
    knights[i] = make_pair(x, y);
  }
  
  // build graph
  // (2 *) bc of vertex capacities property | v(c) => v_in --c--> v_out
  // (+ 2) bc of multiple sources, multiple sinks
  graph G(2 * m * n + 2);
  edge_adder adder(G);
  int source = 2 * m * n; // master source
  int sink = 2 * m * n + 1; // master sink
  
  // if it is a knight at an intersection => connect the master source to it
  for (int i = 0; i < k; i++) {
    int idx_in = grid[knights[i].second][knights[i].first] * 2; // intersection in index
    adder.add_edge(source, idx_in, 1);
  }
    
  // add edges
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      // intersection in and out indexes
      int idx_s_in = grid[i][j] * 2;
      int idx_s_out = grid[i][j] * 2 + 1;
      
      // connect the intersection in and out vertexes with capacity c
      adder.add_edge(idx_s_in, idx_s_out, c);
      
      // connect the intersection to master sink if necessary
      // --- north / south ---
      if (i == 0 || i == n - 1)
        adder.add_edge(idx_s_out, sink, 1);
      
      // connect the intersection to master sink if necessary
      // --- west / east ---
      if (j == 0 || j == m - 1)
        adder.add_edge(idx_s_out, sink, 1);
      
      // add only out edges from current intersection
      
      // add edge to north
      if (i > 0) {
        int idx_t_in = grid[i - 1][j] * 2;
        adder.add_edge(idx_s_out, idx_t_in, 1); // from, to, capacity
      }
      
      // add edge to south
      if (i < n - 1) {
        int idx_t_in = grid[i + 1][j] * 2;
        adder.add_edge(idx_s_out, idx_t_in, 1); // from, to, capacity
      }
      
      // add edge to west
      if (j > 0) {
        int idx_t_in = grid[i][j - 1] * 2;
        adder.add_edge(idx_s_out, idx_t_in, 1); // from, to, capacity
      }
      
      // add edge to east
      if (j < m - 1) {
        int idx_t_in = grid[i][j + 1] * 2;
        adder.add_edge(idx_s_out, idx_t_in, 1); // from, to, capacity
      }
    }
  }
  
  // Calculate flow from source to sink
  long flow = boost::push_relabel_max_flow(G, source, sink);
  
  cout << flow << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
  return 0;
}