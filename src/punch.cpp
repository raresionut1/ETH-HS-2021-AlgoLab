///
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

#define INF INT_MAX

struct Entry {
  int cost;
  int drinks;
  bool used;
};

int inline operator< (const Entry &e1, const Entry &e2) {
  return e1.cost != e2.cost
    ? e1.cost < e2.cost
    : e1.drinks > e2.drinks;
}

int inline operator!= (const Entry &e1, const Entry &e2) {
    return e1.cost != e2.cost || e1.drinks != e2.drinks;
}

void solve () {
  int n; cin >> n; // nr of distinct beverages
  int k; cin >> k; // minimum number of litres

  vector<int> costs(n + 1);
  vector<int> litres(n + 1);

  for (int i = 1; i <= n; i++) {
    cin >> costs[i]; // cost of a beverage
    cin >> litres[i]; // nr of liters of the beverage
  }

  vector<vector<Entry>> dp(n + 1, vector<Entry>(k + 1));

  // ============== Bottom-Up DP ===============
  // dp[first i drinks][litres threshold] = {min_cost, max_nr_drinks}
  // dp[i][j] = best(dp[i-1][j], dp[i][j - litres[i]])
  for (int i = 0; i <= n; i++)
    dp[i][0] = {0, 0, 0};
  
  for (int j = 1; j <= k; j++)
    dp[0][j] = {INF, 0, 0};
  
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= k; j++) {
      Entry without = {dp[i - 1][j].cost,
                       dp[i - 1][j].drinks,
                       false};
      Entry with = {costs[i] + dp[i][max(0, j - litres[i])].cost,
                    dp[i][max(0, j - litres[i])].drinks + !dp[i][max(0, j - litres[i])].used,
                    true};
                    
      dp[i][j] = min(without, with);
    }
  }

  cout << dp[n][k].cost << " " << dp[n][k].drinks << endl;
}

int main () {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int t; cin >> t;
  while (t--)
    solve();
}