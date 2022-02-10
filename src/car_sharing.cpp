///5
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#define INF INT_MAX
#define MAX_P 100

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
  void add_edge(long from, long to, long capacity, long cost) {
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

struct Request {
  long u; // start location
  long v; // end location
  long d; // departure time
  long a; // arrival time
  long p; // profit
};


void solve() {
  long n; cin >> n; // nr of requests (edges)
  long s; cin >> s; // nr of stations (nodes)
  
  vector<set<long>> timestamps(s);
  
  // vector of supplies
  vector<long> supply(s);
  long supply_sum = 0;
  for (long i = 0; i < s; i++) {
    cin >> supply[i];
    supply_sum += supply[i];
    timestamps[i].insert(0);
  }
  
  long max_timestamp = 0;
  // vector of requests
  vector<Request> requests(n);
  for (long i = 0; i < n; i++) {
    Request req;
    cin >> req.u >> req.v >> req.d >> req.a >> req.p;
    requests[i] = req;
    
    // add the timestamps in the set
    timestamps[req.u - 1].insert(req.d);
    timestamps[req.v - 1].insert(req.a);
    
    max_timestamp = max(max_timestamp, req.d);
    max_timestamp = max(max_timestamp, req.a);
  }
  
  // add max timestamp for each station
  for (long i = 0; i < s; i++)
    timestamps[i].insert(max_timestamp);
  
  // make a map for each timestamp index
  vector<unordered_map<long, long>> timestamp_idx(s);
  long idx = 0;
  for (long i = 0; i < s; i++) {
    for (long timestamp : timestamps[i]) {
      timestamp_idx[i][timestamp] = idx;
      idx++;
    }
  }
    
  // Create graph, edge adder class and propery maps
  graph G(idx + 2);
  edge_adder adder(G);
  
  const long source = idx;
  const long sink = idx + 1;
  
  // Add the edges between timestamps
  for (long i = 0; i < s; i++) {
    // add edge between source and first timestamp
    adder.add_edge(source, timestamp_idx[i][0], supply[i], 0);
    // add edge between last timestamp and sink
    adder.add_edge(timestamp_idx[i][max_timestamp], sink, INF, 0);
    
    // add edges between timestamps
    long last_t = -1;
    for (long t : timestamps[i]) {
      if (last_t == -1) {
        last_t = t;
        continue;
      }
      
      long index1 = timestamp_idx[i][last_t];
      long index2 = timestamp_idx[i][t];
      
      adder.add_edge(index1, index2, INF, MAX_P * (t - last_t));
      
      last_t = t;
    }
  }
  
  // Add the edges for requests
  for (Request req : requests) {
    long index1 = timestamp_idx[req.u - 1][req.d];
    long index2 = timestamp_idx[req.v - 1][req.a];
    adder.add_edge(index1, index2, 1, MAX_P * (req.a - req.d) - req.p);
  }

  // Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  long cost = MAX_P * supply_sum * max_timestamp - boost::find_flow_cost(G);
  
  cout << cost << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  long t; cin >> t;
  while(t--)
    solve();
}
