#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
using namespace std;

typedef long long ll;
typedef vector<vector<int>> vvi;
ll p10[]={1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000,10000000000};

class puzzle{
    public:
        ll state;
        vector<ll> get_next_states(){
            vector<ll> ans;
            int dx[]={0,0,1,-1};
            int dy[]={1,-1,0,0};
            for(int i=0;i<4;i++){
                int x=(state/p10[10])%10;
                int y=(state/p10[9])%10;
                int x1=x+dx[i],y1=y+dy[i];
                if(x1>=0&&x1<3&&y1>=0&&y1<3){
                    int a=(state/p10[3*x1+y1])%10;
                    ans.push_back(state-a*p10[3*x1+y1]+a*p10[3*x+y]-x*p10[10]-y*p10[9]+x1*p10[10]+y1*p10[9]);
                }
            }
            return ans;
        }
};

class agent{
    private:
        ll goal;
        puzzle env;
        unordered_set<ll> closed_list;// state
        unordered_map<ll,set<vector<ll>>::iterator> open_list; //{map, pointer to cost values}
        set<vector<ll>> cost; //{f,g,state}
        ll encode(vvi state){
            ll ans=0;
            int x,y;
            for(int i=2;i>=0;i--)
                for(int j=2;j>=0;j--){
                    ans=ans*10+state[i][j];
                    if(state[i][j]==0) x=i,y=j;}
            ans+=x*p10[10]+y*p10[9];
            return ans;
        }
        void decode(ll state){
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++) cout<<(state/p10[3*i+j])%10;
                cout<<'\n';
            }
            cout<<'\n';
        }
        int h(ll state){
            int ans=0;
            for(int i=0;i<9;i++) 
                if((state/p10[i])%10==(goal/p10[i])%10) ans++;
            return 9-ans;
        }
    public:
    agent(vvi start,vvi end){
        ll st=encode(start);
        open_list[st]=cost.insert({h(st),0,st}).first;
        goal=encode(end);
    }
    bool solve(){
        vector<ll> ans;
        unordered_map<ll,ll> ancestor;
        while(!open_list.empty()){
            env.state=(*cost.begin())[2];
            ll g=(*cost.begin())[1];
            if(env.state==goal){
                ll temp=goal;
                while(ancestor.find(temp)!=ancestor.end()){
                    ans.push_back(temp);
                    temp=ancestor[temp];
                }
                ans.push_back(temp);
                reverse(ans.begin(),ans.end());
                for(ll x:ans) decode(x);
                cout<<g<<'\n';
                return true;}
            cost.erase(open_list[env.state]);
            open_list.erase(env.state);
            closed_list.insert(env.state);
            for(ll ns:env.get_next_states()){
                if(closed_list.find(ns)!=closed_list.end()) continue;
                int fn=g+1+h(ns);
                if(open_list.find(ns)==open_list.end()){
                    open_list[ns]=cost.insert({fn,g+1,ns}).first;
                    ancestor[ns]=env.state;
                }else if(open_list.find(ns)!=open_list.end() && fn<(*open_list[ns])[0]){
                    cost.erase(open_list[ns]);
                    open_list[ns]=cost.insert({fn,g+1,ns}).first;
                    ancestor[ns]=env.state;
                }
            }

        }
        return false;
    }
};

int main(){
    vvi start={{1,6,2},{4,5,7},{8,3,0}};
    vvi end={{1,2,3},{4,5,6},{7,8,0}};//=12687054321/p10[10]=1%10=1;
    agent ankit(start,end);
    cout<<ankit.solve()<<'\n';
    return 0;
}
