///1
#include <iostream>
#include <vector>
#include <set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;

typedef boost::adjacency_list
  <
  boost::vecS,
  boost::vecS,
  boost::undirectedS
  > graph;
  
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
  
int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n); // exterior property map
  
  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(),
    boost::get(boost::vertex_index, G)));
    
  int matching_size = boost::matching_size(G,
   boost::make_iterator_property_map(mate_map.begin(),
    boost::get(boost::vertex_index, G)));
    
  return matching_size;
}


void solve() {
  int n; cin >> n; // nr of students
  int c; cin >> c; // nr of characteristics
  unsigned int f; cin >> f; // nr of common characteristics
  
  vector<set<string>> characteristics(n, set<string>());
  
  // read characteristics
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < c; j++) {
      string ch; cin >> ch;
      characteristics[i].insert(ch);
    }
  }
  
  // compute common characteristics for each pair of students
  graph G(n);
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      vector<string> intersection;
      set_intersection (characteristics[i].begin(), 
                        characteristics[i].end(), 
                        characteristics[j].begin(), 
                        characteristics[j].end(), 
                        back_inserter(intersection));
      // if they have more than f characteristics in common
      // add an edge to the graph
      if (intersection.size() > f)
        boost::add_edge(i, j, G);
    }
  }
  
  // find maximum matching size (nr of edges returned by the Edmonds algorithm)
  int matching_size = maximum_matching(G);
  
  // print output
  cout << (n == 2 * matching_size ? "not optimal" : "optimal") << endl;
}

int main() {
  int t; cin>>t;
  while (t-- > 0)
    solve();
}