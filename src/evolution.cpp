#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

struct query {
  int id;
  int b;
};

vector<int> age;
vector<vector<int>> children;
vector<vector<query>> queries;
vector<int> answer;
vector<int> path;

// Binary search over the current path to find largest element that is at most maxAge old
int search(int maxAge) {
  int a = 0;
  int b = path.size() - 1;
  while (a != b) {
    int m = (a + b) / 2;
    if (age[path[m]] <= maxAge) {
      b = m;
    } else {
      a = m + 1;
    }
  }
  return path[a];
  
  // for (unsigned int i = 0; i < path.size(); i++)
  //   if (age[path[i] <= maxAge]) 
  //     return path[i];
      
  // return -1;
}

// Use DFS to compute the path for the current vertex
// Answer all queries for the current vertex by calling the search function
void dfs(int u) {
  path.push_back(u);

  // Compute answers for queries at u
  for (query q : queries[u]) {
    answer[q.id] = search(q.b);
  }

  // Recursion
  for (int v : children[u]) {
    dfs(v);
  }
 
  path.pop_back();
}

// Strategy: Use binary search to answer queries
void solve() {
  int n, q;
  cin >> n >> q;
  
  // We will be working with integer indices
  unordered_map<string, int> id; // From name to id
  vector<string> name(n); // From id to name

  // Read species names and age
  age = vector<int>(n);
  for (int i = 0; i < n; ++i) {
    cin >> name[i];
    cin >> age[i];
    id[name[i]] = i;
  }
  
  vector<bool> have_parent(n, false);
  // read species offsprings (tree of life)
  
  // Read offsprings
  string parent, child;
  children = vector<vector<int>>(n);
  for (int i = 0; i < n - 1; ++i) {
    cin >> child >> parent;
    children[id[parent]].push_back(id[child]);
    have_parent[id[child]] = true;
  }
  
  // find the root of the tree of life
  int root = 0;
  for (int i = 0; i < n; i++) {
    if (!have_parent[i]) {
      root = i;
      break;
    }
  }
  
  // Read queries
  string queryName;
  int maxAge;
  queries = vector<vector<query>>(n);

  for (int queryId = 0; queryId < q; ++queryId) {
    cin >> queryName >> maxAge;
    queries[id[queryName]].push_back({queryId, maxAge});   
  }

  // Call dfs, the dfs actually solves the problem
  answer = vector<int>(n);
  path = vector<int>();
  dfs(root);

  // Output answers
  for (int j = 0; j < q; ++j) {
    cout << name[answer[j]] << " ";
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}