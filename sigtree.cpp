#include <iostream>
#include <vector>
using namespace std;
void update(vector<int> &tree, int ndx,int val){
    int n=tree.size()/2+ndx;
    tree[n]=val;
    for(int i=n;i>1;i>>=1)
        tree[i>>1]=max(tree[i],tree[i^1]);
}
int query(vector<int> &tree,int ndx, int s,int e){
    int p=1;
    int ans=0;
    while(s<e){
        int mid=(s+e)/2;
        if(ndx<mid){e=mid; p<<=1;}
        else{
            ans=max(ans,tree[p<<1]);
            s=mid+1;
            p=(p<<1)|1;
        }
    }
    return ans;
}
int main(){
    int n,q;
    cin>>n;
    vector<int> tree(2*n,0);
    cin>>q;
    while(q--){
        int qt;
        cin>>qt;
        if(qt==1){
            int ndx,val;
            cin>>ndx>>val;
            update(tree,ndx,val);
            cout<<"value updated\n";
        }else{
            int ndx;
            cin>>ndx;
            cout<<"Maximum height is "<<query(tree,ndx,0,n-1);

        }
    }
    return 0;
}