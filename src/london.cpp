///5
#include <iostream>
#include <vector>
#include <unordered_map>

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
  int h; cin >> h; // nr of lines in the newspaper
  int w; cin >> w; // nr of letters on every line
  string letter; cin >> letter; // the letter formed from pieces
  int n = letter.length(); // the length of the letter
  vector<string> front(h); // front page of the newspaper
  vector<string> back(h); // back page of the newspaper
  
  // read the front lines of the newspaper
  for (int i = 0; i < h; i++)
    cin >> front[i];
    
  // read the back lines of the newspaper
  for (int i = 0; i < h; i++)
    cin >> back[i];
  
  // get letters pairs frequencies from newspaper
  unordered_map<string, int> freq_newspaper;

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      char ch1 = front[i].at(j); // front letter
      char ch2 = back[i].at(w - 1 - j); // back letter
      
      // sort the pair lexicographically
      string key = ch1 > ch2
                  ? string() + ch1 + ch2
                  : string() + ch2 + ch1;
                   
      // check if the key doesn't exist in the frequency map
      if (freq_newspaper.find(key) == freq_newspaper.end())
        freq_newspaper[key] = 0;
      // increase frequency count for the key
      freq_newspaper[key] += 1;
    }
  }
  
  // get letters frequencies from letter
  unordered_map<char, int> freq_letter;
  
  for (char c : letter) {
    // check if the key doesn't exist in the frequency map
    if (freq_letter.find(c) == freq_letter.end())
      freq_letter[c] = 0;
    // increase frequency count for the key
    freq_letter[c] += 1;
  }
  
  // Create graph for Max Flow
  graph G(freq_newspaper.size() + freq_letter.size() + 2); // 2 extra nodes bc of multiple sources, multiple sinks
  edge_adder adder(G);
  int source = 0; // master source index
  int sink = 1; // master sink index
  int idx = 2; // current index
  
  // we need to keep count of indexes of letters
  unordered_map<char, int> idx_letter;
  
  // add edges from letters from the message to the sink
  for (const pair<char, int> &entry : freq_letter) {
    int curr_idx = idx++;
    idx_letter.insert({entry.first, curr_idx});
    adder.add_edge(curr_idx, sink, entry.second);
  }
  
  // add edges from pairs of letters to letters in the message
  for(const pair<string, int> &entry : freq_newspaper) {
    int curr_idx = idx++;
    
    char ch1 = entry.first.at(0); // first letter in the pair
    char ch2 = entry.first.at(1); // second letter in the pair
    
    // add the edge only if the letter 1 can be found in the message
    adder.add_edge(curr_idx, idx_letter[ch1], entry.second);
      
    // add the edge only if the letter 2 can be found in the message
    adder.add_edge(curr_idx, idx_letter[ch2], entry.second);
      
    // connect the source to the current pair of letters
    adder.add_edge(source, curr_idx, entry.second);
  }
  
  // Calculate flow from source to sink
  long flow = boost::push_relabel_max_flow(G, 0, sink);
  
  // check if the flow is equal to the message length
  cout << (flow == n ? "Yes" : "No") << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while(t-- > 0)
    solve();
  return 0;
}