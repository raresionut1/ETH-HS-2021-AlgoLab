///1
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
  
typedef boost::graph_traits<weighted_graph>::edge_descriptor Edge;

vector<Edge> kruskal(const weighted_graph &G) {
  vector<Edge> mst; // vector to store MST edges (not a property map!)
  boost::kruskal_minimum_spanning_tree(G, back_inserter(mst));
  return mst;
}

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

void solve() {
  int n; cin >> n; // nr of vertices
  int e; cin >> e; // nr of edges
  int s; cin >> s; // nr of species
  int a; cin >> a; // start tree
  int b; cin >> b; // finish tree
  
  // final graph
  weighted_graph G(n);
  
  // weighted graphs for each species
  vector<weighted_graph> species_graph(s, weighted_graph(n));
  
  // add weighted edges
  for (int i = 0; i < e; i++) {
    int u, v;
    cin >> u >> v;
    for (int j = 0; j < s; j++) {
      int w; cin >> w;
      boost::add_edge(u, v, w, species_graph[j]);
    }
  }
  
  vector<int> hives;
  for (int i = 0; i < s; ++i) {
    int h; cin >> h;
    hives.push_back(h);
  }
  
  // ======================= PART 1
  
  // Get minimum spanning tree for each species
  // and create a new graph composed of all species' mst
  for (int i = 0; i < s; i++) {
    vector<Edge> mst = kruskal(species_graph[i]);
    for (auto e : mst) {
      int source = boost::source(e, species_graph[i]);
      int target = boost::target(e, species_graph[i]);
      int weight = boost::get(boost::edge_weight_t(), species_graph[i], e);
      boost::add_edge(source, target, weight, G);
    }
  }
  
  // ======================= PART 2
  
  // Get shortest path between A and B using Dijkstra
  cout << dijkstra_dist(G, a, b) << endl;
}

int main() {
  int t; cin>>t;
  while (t-- > 0)
    solve();
}