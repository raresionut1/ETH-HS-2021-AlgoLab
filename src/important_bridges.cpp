///2
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace std;

namespace boost {
  struct edge_component_t {
      enum { num = 555 };
      typedef edge_property_tag kind;
  } edge_component;
}

typedef boost::adjacency_list
  <
  boost::vecS,
  boost::vecS,
  boost::undirectedS,
  boost::no_property,
  boost::property<boost::edge_component_t, std::size_t>
  > graph;
  
typedef boost::graph_traits<graph>::edge_descriptor edge;
typedef boost::graph_traits<graph>::vertex_descriptor vertex;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

void solve() {
  int n; cin >> n; // nr of vertices (islands)
  int m; cin >> m; // nr of edges (bridges)
  
  // init graph
  graph G(n);
  // add edges
  for (int i = 0; i < m; i++) {
    int u, v;
    std::cin >> u >> v;
    boost::add_edge(u, v, G);
  }
  
  // get biconnected components
  boost::property_map<graph, boost::edge_component_t >::type component
      = get(boost::edge_component, G);

  int num_comps = biconnected_components(G, component);
  
  // count number of edges of each biconnected component
  vector<vector<edge>> component_edges(num_comps, vector<edge>());
  
  edge_it e_beg, e_end;
  for (boost::tie(e_beg, e_end) = boost::edges(G); e_beg != e_end; ++e_beg)
    component_edges[component[*e_beg]].push_back(*e_beg);
    
  // if a component has only one edge -> it's a critical edge (bridge)
  vector<vector<int>> critical_edges;
  
  for (int i = 0; i < num_comps; i++) {
    if (component_edges[i].size() == 1) {
      int u = boost::source(component_edges[i][0], G);
      int v = boost::target(component_edges[i][0], G);
      
      // swap if necessary
      if (u > v) {
        auto aux = u;
        u = v;
        v = aux;
      }
      
      vector<int> critical_edge({u, v});
      critical_edges.push_back(critical_edge);
    }
  }
  
  // sort the edges by their values
  sort(critical_edges.begin(), critical_edges.end());
  
  // print output
  // k (nr of critical edges)
  // u, v (critical edges) --- k times
  cout << critical_edges.size() << endl;
  for (unsigned int i = 0; i < critical_edges.size(); i++)
    cout << critical_edges[i][0] << " " << critical_edges[i][1] << endl;
}

int main() {
  int t; cin>>t;
  while (t-- > 0)
    solve();
}