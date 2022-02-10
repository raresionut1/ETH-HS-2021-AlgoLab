#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

typedef boost::adjacency_list
  <
  boost::vecS,
  boost::vecS,
  boost::undirectedS,
  boost::no_property, // no vertex property
  boost::property<boost::edge_weight_t, int> // edge property (interior)
  > weighted_graph;
  
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

vector<edge_desc> kruskal(const weighted_graph &G) {
  vector<edge_desc> mst; // vector to store MST edges (not a property map!)
  boost::kruskal_minimum_spanning_tree(G, back_inserter(mst));
  return mst;
}

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
  int n; cin >> n; // nr of vertices
  int m; cin >> m; // nr of edges
  
  // init graph
  weighted_graph G(n);
  // add weighted edges
  for (int i = 0; i < m; i++) {
    int u, v, w;
    std::cin >> u >> v >> w;
    boost::add_edge(u, v, w, G);
  }
  
  // ======================= PART 1
  
  // Get minimum spanning tree
  vector<edge_desc> mst = kruskal(G);
  
  // get the sum w of edges in the mst
  int total_w = 0;
  for (auto e : mst) {
    total_w += boost::get(boost::edge_weight_t(), G, e);
  }
  
  // ======================= PART 2
  
  // Get shortest paths using Dijkstra
  vector<int> dist_map = dijkstra_dist(G, 0);
  int furthest_vertex_distance = *max_element(begin(dist_map), end(dist_map));
  
  // print output
  cout << total_w << " " << furthest_vertex_distance << endl;
}

int main() {
  int t; cin>>t;
  while (t-- > 0)
    solve();
}