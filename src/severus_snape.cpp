#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

struct potA {
  long p;
  long h;
  potA(long pp, long hh){p=pp;h=hh;}
  potA(){potA(0,0);}
  
  bool operator<(const potA& other) const{
    return p < other.p || (p == other.p && h < other.h);
  }
};

long maxpower(int i, int k, int h, vector<potA> &potions, vector<vector<vector<long>>> &memo){
  // cout<<i<<" "<<k<<" "<<h<<endl;
  if(i == 0 || k == 0){
    if(h == 0) 
      return 0;
    return -1;
  }
  
  if (memo[i - 1][k - 1][h] > -2)
    return memo[i - 1][k - 1][h];
  
  long best = -1;
  if(i > k && k > 0)
    best = max(best, maxpower(i - 1, k, h, potions, memo));
  
  long temp = maxpower(i - 1, k - 1, max(0l, h - potions[i - 1].h), potions, memo);
  if(temp > -1)
    best = max(best, temp + potions[i - 1].p);
  
  memo[i - 1][k - 1][h] = best;
  return best;
}

void snape(){
  int n, m; cin >> n >> m;
  int a,b; cin >> a >> b;
  long p, h, w; cin >> p >> h >> w;
  
  vector<potA> pota(n);
  vector<int> potb(m);
  
  long t1,t2;
  for(int i = 0; i < n; i++){
    cin >> t1 >> t2;
    pota[i] = potA(t1, t2);
  }
  
  for (int i = 0; i < m; i++)
    cin >> potb[i];
  
  // sort(pota.rbegin(), pota.rend());
  sort(potb.rbegin(), potb.rend());
  
  long s = 0;
  vector<int> extra = vector<int>(n + 1, -1);
  
  int totalb = 0;
  while(s < w && totalb < m){
    s += potb[totalb];
    totalb += 1;
  }
  
  int maxa = n;
  
  if(s < w) {
    cout<< -1 <<endl;
    return;
    
  } else {
    extra[0] = totalb;
    
    for(long i = 1; i <= n; i++){
      while(s < w + i * a && totalb < m){
        s += potb[totalb];
        totalb += 1;
      }
      
      if(s < w) {
        maxa = i - 1;
        break;
      } else{
        extra[i] = totalb;
      }
    }
  }
  
  int totala = 0;
  if(n > 0 && (p > 0 || h > 0)) {
    vector<vector<vector<long>>> memo(n, vector<vector<long>>(n, vector<long>(h + 1, -2)));
    for(int i = 1; i <= maxa; i++){
      if(maxpower(n, i, h, pota, memo) >= p + b * (long)extra[i]){
        totala = i;
        break;
      }
    }
    
    if(totala == 0){
      cout << -1 << endl;
      return;
    }
  }
  cout << totala + extra[totala] << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin>>t;
  for(int i = 0; i < t; i++)
    snape();
  return 0;
}