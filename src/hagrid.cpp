///1
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

vector<int> sizes;

bool custom_comparator(pair<long, long> p1, pair<long, long> p2) {
  return p1.first * sizes[p2.second] < p2.first * sizes[p1.second];
}

long preprocess (long u,
                 vector<unordered_map<long, long>> &adj,
                 vector<vector<pair<long, long>>> &adjsum) {
  long curr = 0;
  int size = 1;
  
  for (auto &entry : adj[u]) {
    long v = entry.first;
    long l = entry.second;
    long value = l + preprocess(v, adj, adjsum);
    adjsum[u].push_back(make_pair(value, v));
    curr += value;
    size += sizes[v];
  }

  sizes[u] = size;
  
  return curr;
}

long compute_reward (long u, long t, vector<long> &rewards,
                     vector<unordered_map<long, long>> &adj,
                     vector<vector<pair<long, long>>> &adjsum) {
  long reward = rewards[u] - t;
  long t_penalty = 0;
  
  sort(adjsum[u].begin(), adjsum[u].end(), custom_comparator);
  
  for (auto entry : adjsum[u]) {
    long v = entry.second;
    long w = entry.first;
    
    reward += compute_reward(v, t + t_penalty + adj[u][v], rewards, adj, adjsum);
    t_penalty += 2 * w;
  }
  
  return reward;
}

void solve () {
  /////////////////////////// READ INPUT ////////////////////////////
  long n; cin >> n; // nr of chambers (nodes)
  
  vector<long> rewards(n + 1, 0);
  for (long i = 1; i <= n; i++)
    cin >> rewards[i];
    
  vector<unordered_map<long, long>> adj(n + 1);
  for (long i = 0; i < n; i++) {
    long u; cin >> u;
    long v; cin >> v;
    long l; cin >> l;
    
    adj[u][v] = l;
  }
  
  /////////////////////////// SOLVE PROBLEM ////////////////////////////
  vector<vector<pair<long, long>>> adjsum(n + 1);
  sizes = vector<int>(n + 1, 0);
  preprocess(0, adj, adjsum);
  long reward = compute_reward(0, 0, rewards, adj, adjsum);
  
  cout << reward << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  long t; cin >> t;
  while (t--)
    solve();
}