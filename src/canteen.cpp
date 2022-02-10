///1
#include <iostream>
#include <vector>
#include <limits>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#define INF INT_MAX

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

struct Day {
  int prod_nr;
  int prod_cost;
  int cons_nr;
  int cons_cost;
  int storage_nr;
  int storage_cost;
};

void solve() {
  int n; cin >> n; // nr of days of planning ahead
  vector<Day> days(n);
  
  // ============ READING THE INPUT ============
  // production
  for (int i = 0; i < n; i++) {
    cin >> days[i].prod_nr; // nr of menus
    cin >> days[i].prod_cost; // production cost
  }
  
  // consumption
  for (int i = 0; i < n; i++) {
    cin >> days[i].cons_nr; // nr of students who want to eat
    cin >> days[i].cons_cost; // menu price
  }
  
  // storage
  for (int i = 0; i < n - 1; i++) {
    cin >> days[i].storage_nr; // nr of menus that can be stored overnight
    cin >> days[i].storage_cost; // cooling energy cost per menu
  }
  
  // ============ SOLVING THE PROBLEM ============
  
  
  // Create graph, edge adder class and propery maps
  graph G(n + 2);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  // number used to normalize the negative costs
  // (so we can use the faster algorithm)
  const int max_cons_cost = 20;
  
  // indexes and offsets
  const int source = 3 * n;
  const int sink = 3 * n + 1;
  
  // add the edges
  for (int i = 0; i < n; i++) {
    int idx = i;
    int next_idx = i + 1;
    
    // add edge between source and current day node
    // (capacity == prod_nr | cost == prod_cost)
    adder.add_edge(source, idx, days[i].prod_nr, days[i].prod_cost);
    
    // add edge between current day node and sink
    // (capacity == cons_nr | cost == -cons_cost)
    adder.add_edge(idx, sink, days[i].cons_nr, max_cons_cost - days[i].cons_cost);
    
    // if it's not the last day => add edge between day node and next day node
    // (capacity == storage_nr | cost == storage_cost)
    if (i < n - 1)
      adder.add_edge(idx, next_idx, days[i].storage_nr, days[i].storage_cost);
  }
  
  // Compute max flow, max cost
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e)
    flow += c_map[*e] - rc_map[*e];
    
  int cost = max_cons_cost * flow - boost::find_flow_cost(G);

  // Compute total number of students
  long total_cons_nr = 0;
  for (int i = 0; i < n; i++)
    total_cons_nr += days[i].cons_nr;
    
  // Check if all the students were fed
  string s = flow == total_cons_nr ? "possible" : "impossible";
  
  // print output
  cout << s << " " << flow << " " << cost << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
}