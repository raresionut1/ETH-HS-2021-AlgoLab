///1
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property> UGraph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::property<boost::edge_weight_t, int>> DGraph; // An edge-weighted digraph.
typedef boost::property_map<DGraph, boost::edge_weight_t>::type WeightMap;
typedef boost::graph_traits<DGraph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<DGraph>::edge_descriptor edge_desc;

void solve() {
  // ===================== READ INPUT =====================
  int n; cin >> n; // number of locations around the peak (nodes)
  int m; cin >> m; // number of slopes and ski lifts (edges)
  int a; cin >> a; // number of secret agents
  int s; cin >> s; // number of shelters
  int c; cin >> c; // capacity of each shelter
  int d; cin >> d; // the time taken for entering a shelter
  
  // create directed graph
  DGraph G(n);
  
  // read edges
  for (int i = 0; i < m; i++) {
    char w; cin >> w; // S(lope) -> one way / L(ift) -> both directions
    int u; cin >> u; // edge start
    int v; cin >> v; // edge end
    int z; cin >> z; // nr of seconds taken
    
    // add edge from u --> v
    boost::add_edge(u, v, z, G);
    
    // if it is a lift, add a reverse edge too ( v --> u )
    if (w == 'L')
      boost::add_edge(v, u, z, G);
  }
  
  // read agent positions
  vector<int> agents(a);
  for (int i = 0; i < a; i++) {
    int p; cin >> p;
    agents[i] = p; 
  }
  
  // read shelters positions
  vector<int> shelters(s);
  for (int i = 0; i < s; i++) {
    int p; cin >> p;
    shelters[i] = p;
  }
  
  // ===================== SOLVE PROBLEM =====================
  // -- step 1: Compute shortest paths from agents to shelters using Dijkstra
  vector<vector<int>> all_dists(a, vector<int>(n));
  for (int i = 0; i < a; i++)
    dijkstra_shortest_paths(G, agents[i], 
      distance_map(make_iterator_property_map(all_dists[i].begin(), get(boost::vertex_index, G))));
    
  // -- keep only the distances between agents and shelters
  vector<vector<int>> dists(a, vector<int>(s));
  for (int i = 0; i < a; i++)
    for (int j = 0; j < s; j++)
      dists[i][j] = all_dists[i][shelters[j]];
      
  // -- step 2: find the smallest time required for all agents to enter the shelters
  //            using Binary Search
  int l = 0;
  int r = INT_MAX;
  int res = -1;
  
  while (l <= r) {
    int m = l + (r - l) / 2; // to avoid overflow
    
    // build another bipartite graph (to apply maximal matching)
    // -> we duplicate shelters with capacity > 1
    UGraph BG(a + s * c);
    for (int i = 0; i < a; i++)
      for (int j = 0; j < s; j++) {
        // check if shelter is reachable
        if (dists[i][j] == INT_MAX)
          continue;
          
        // for each extra spot in shelters, we add the penalty time to the path
        for (int k = 0; k < c; k++)
          // check if the agent can enter the shelter in time
          if (dists[i][j] + d * (k + 1) <= m)
            // --> add the edge in the bipartite graph
            add_edge(i, a + s * k + j, BG);
      }
      
    // Compute the maximum cardinality matching for the bipartite graph
    vector<vertex_desc> mate_map(a + s * c); // exterior property map
  
    boost::edmonds_maximum_cardinality_matching(BG,
      boost::make_iterator_property_map(mate_map.begin(),
      boost::get(boost::vertex_index, BG)));
      
    int matching_size = boost::matching_size(BG,
     boost::make_iterator_property_map(mate_map.begin(),
      boost::get(boost::vertex_index, BG)));
      
    // check if maximum matching == number of agents
    // (which means that all agents were able to hide in shelters)
    // --> adjust bounds accordingly
    if (matching_size < a) {
      l = m + 1;
    } else {
      res = m;
      r = m - 1;
    }
  }
  
  // ===================== PRINT OUTPUT =====================
  cout << res << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t--)
    solve();
}