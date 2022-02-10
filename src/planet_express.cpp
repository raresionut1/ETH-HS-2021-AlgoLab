///
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

typedef boost::adjacency_list
  <
  boost::vecS,
  boost::vecS,
  boost::directedS,
  boost::no_property, // no vertex property
  boost::property<boost::edge_weight_t, int> // edge property (interior)
  > weighted_graph;
  
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

vector<int> dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n); //exterior property
  boost::dijkstra_shortest_paths(
    G, s,
    boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
      boost::get(boost::vertex_index, G))));
  return dist_map;
}

void solve() {
  int n; cin >> n; // nr of vertices (planets)
  int m; cin >> m; // nr of edges
  int k; cin >> k; // nr of warehouses
  int T; cin >> T; // nr of planets in the teleportation network
  
  // init graph
  weighted_graph G(n);
  
  // keep track of planets that are in the teleportation network
  vector<int> telnet(T);
  for (int i = 0; i < T; i++) {
    int t; cin >> t;
    telnet[i] = t;
  }
  
  // add weighted edges
  for (int i = 0; i < m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    boost::add_edge(v, u, w, G); // edge in REVERSE
  }
  
  // ======================= PART 1
  // Get strongly connected components
  vector<int> scc(n);
  int nr_scc = boost::strong_components(G, make_iterator_property_map(scc.begin(), get(boost::vertex_index, G)));

  vector<int> nr_vertices_in_scc(nr_scc, 0);
  // count number of nodes in each scc
  for (int i = 0; i < T; i++) {
    int vertex = telnet[i];
    int component = scc[vertex];
    nr_vertices_in_scc[component]++;
  }
  
  // ======================= PART 2
  // Add edges between LINKED planets in the teleportation network
  // => use "middleman" technique
  for (int i = 0; i < T; i++) {
    int vertex = telnet[i];
    int component = scc[vertex];
    int middleman = n + component;
    int cost = nr_vertices_in_scc[component] - 1;
    
    boost::add_edge(vertex, middleman, cost, G);
    boost::add_edge(middleman, vertex, 0, G);
  }
  
  int shortest_cost = -1;
  
  // Get shortest paths using Dijkstra
  vector<int> dist_map = dijkstra_dist(G, n - 1);
  
  for (int i = 0; i < k; i++) {
    int cost = dist_map[i];
    
    if (shortest_cost == -1 || cost < shortest_cost)
      shortest_cost = cost;
  }
  
  // print output
  if (shortest_cost > 1000000)
    cout << "no" << endl;
  else
    cout << shortest_cost << endl;
}

int main() {
  int t; cin>>t;
  while (t-- > 0)
    solve();
}