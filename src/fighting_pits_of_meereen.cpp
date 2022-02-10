#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct State {
  // balance rating
  int balance;
  // Last 2 fighters from North
  int n1, n2;
  // Last 2 fighters from South
  int s1, s2;

  int score(int m, bool north, int fighter) {
    unordered_set<int> diff_fighters;

    if (m == 2) {
      if (north)
        diff_fighters = {fighter, n1};
      else
        diff_fighters = {fighter, s1};
    }

    if (m == 3) {
      if (north)
        diff_fighters = {fighter, n1, n2};
      else
        diff_fighters = {fighter, s1, s2};
    }

    int distinct_fighters =  diff_fighters.size() - diff_fighters.count(7);

    int next_balance = north == true ? balance + 1 : balance - 1;

    return 1000 * distinct_fighters - int(pow(2, abs(next_balance)));
  }
};

int encode(State state) {
  int encoding = 0;
  encoding += state.s2;

  encoding = encoding << 3;
  encoding += state.s1;

  encoding = encoding << 3;
  encoding += state.n2;

  encoding = encoding << 3;
  encoding += state.n1;

  encoding = encoding << 12;
  encoding += state.balance;

  return encoding;
}

State decode(int encoding) {
  // balance rating
  int balance = encoding & 0b111111111111;
  // Last 3 fighters from North
  int n1 = (encoding >> 12) & 0b111;
  int n2 = (encoding >> 15) & 0b111;
  // Last 3 fighters from South
  int s1 = (encoding >> 18) & 0b111;
  int s2 = (encoding >> 21) & 0b111;

  return State{balance, n1, n2, s1, s2};
}

void testcase() {
  int n; cin >> n; // nr of fighters in the queue (maximum 5000)
  int k; cin >> k; // different types of fighters (maximum 4)
  int m; cin >> m; // memory size (either 2 or 3)

  vector<int> v(n); // types of fighters
  for (int i = 0; i < n; i++)
    cin >> v[i];
  
  // Initial state (empty, balance 0)
  State initial_state = {0, 7, 7, 7, 7};

  // Map to memorize current states scores
  unordered_map<long, int> current_scores;
  current_scores[encode(initial_state)] = 0;

  for (int i = 0; i < n; i++) {
    int current_fighter = v[i];
    unordered_map<long, int> next_scores;

    for (auto entry : current_scores) {
      State current_state = decode(entry.first);
      int current_score = entry.second;

      int next_score_n = current_state.score(m, true, current_fighter);
      int next_score_s = current_state.score(m, false, current_fighter);

      // Verify if scores should be added to the map
      if (next_score_n >= 0) {
        // Compute next state after sending a fighter to the north
        State next_state_n = {current_state.balance + 1,
                  current_fighter,
                  current_state.n1,
                  current_state.s1,
                  current_state.s2};

        next_scores[encode(next_state_n)] = max(current_score + next_score_n, 
                              next_scores[encode(next_state_n)]);
      }
        
                      
      // Verify if scores should be added to the map
      if (next_score_s >= 0) {
        // Compute next state after sending a fighter to the south
        State next_state_s;
        if (current_state.balance > 0)
          next_state_s = {current_state.balance - 1,
                  current_state.n1,
                  current_state.n2,
                  current_fighter,
                  current_state.s1};
        else
          next_state_s = {current_state.balance + 1,
                  current_fighter,
                  current_state.s1,
                  current_state.n1,
                  current_state.n2};

        next_scores[encode(next_state_s)] = max(current_score + next_score_s, 
                              next_scores[encode(next_state_s)]);
      }
    }

    current_scores = next_scores;
  }

  // Print output
  int best_score = -1;
  for (auto entry : current_scores) {
    int score = entry.second;
    best_score = max(best_score, score);
  }
  
  cout << best_score << endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i)
    testcase();
}
