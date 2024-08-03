#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

typedef vector<vector<int>> sudoku;

class constraint_graph{
    private:
        vector<set<int>> row;
        vector<set<int>> column;
        vector<set<int>> sub_grid;
    public:
        sudoku state;
        constraint_graph(sudoku sudoku_state) : state(sudoku_state){
            row=vector<set<int>>(9,set<int>({1,2,3,4,5,6,7,8,9}));
            column=vector<set<int>>(9,set<int>({1,2,3,4,5,6,7,8,9}));
            sub_grid=vector<set<int>>(9,set<int>({1,2,3,4,5,6,7,8,9}));
        }
        void erase(int val,int x,int y){
            row[x].erase(val);
            column[y].erase(val);
            sub_grid[3*(x/3)+y/3].erase(val);
        }
        void insert(int val,int x,int y){
            row[x].insert(val);
            column[y].insert(val);
            sub_grid[3*(x/3)+y/3].insert(val);
        }
        bool MRV(int &x,int &y){
            //Minimum Reamining Value (MRV) Heuristic
            int max=0;
            x=-1,y=-1;
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(state[i][j]) continue;
                    int cnt=0;
                    for(int k=0;k<9;k++){
                        if(state[i][k]==0) cnt++;
                        if(k!=i&&state[k][j]==0) cnt++;
                        int sub_grid_x = i - i%3 + k/3, sub_grid_y=j - j%3 + k%3;
                        if(sub_grid_x!=i&&sub_grid_y!=j&&state[sub_grid_x][sub_grid_y]==0) cnt++;
                    }
                    if(cnt>max){
                        max=cnt;
                        x=i,y=j;
                    }
                }
            }
            if(x!=-1) return true;
            return false;
        }
        bool is_available(int val,int x,int y){
            if(row[x].find(val)!=row[x].end()&&column[y].find(val)!=column[y].end()&&sub_grid[3*(x/3)+y/3].find(val)!=sub_grid[3*(x/3)+y/3].end())
                return true;
            return false;
        }
        bool ARC_consistency(){
            for(int i=0;i<9;i++)
                for(int j=0;j<9;j++){
                    set<int> rc,intersection;
                    set_intersection(row[i].begin(),row[i].end(),column[j].begin(),column[j].end(),inserter(rc,rc.begin()));
                    set_intersection(rc.begin(),rc.end(),sub_grid[3*(i/3)+j/3].begin(),sub_grid[3*(i/3)+j/3].end(),inserter(intersection,intersection.begin()));
                    if(intersection.empty()&&!state[i][j]) return false;
                    }
            return true;
        }
};

class csp_solver{
    private:
        constraint_graph D;
    public:
        csp_solver(sudoku &initial_state): D(initial_state){
            for(int i=0;i<9;i++)
                for(int j=0;j<9;j++)
                    D.erase(initial_state[i][j],i,j);
            //env.print();
        }
        bool solve(){
            if(!D.ARC_consistency()) return false;
            int x,y;
            if(!D.MRV(x,y)) return true;
            for(int val=1;val<=9;val++){
                if(D.is_available(val,x,y)){
                    D.state[x][y]=val;
                    D.erase(val,x,y);
                    if(solve()) return true;
                    D.state[x][y]=0;
                    D.insert(val,x,y);
                }
            }
            return false;
        }
        void print(){
            for (int x = 0; x < 9; ++x) {
                    for (int y = 0; y < 9; ++y) {
                        cout << D.state[x][y] << " ";
                    }
                cout << endl;
                }
        }
};


int main(){
    sudoku grid = {
        { 3, 0, 6, 5, 0, 8, 4, 0, 0 },
        { 5, 2, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 8, 7, 0, 0, 0, 0, 3, 1 },
        { 0, 0, 3, 0, 1, 0, 0, 8, 0 },
        { 9, 0, 0, 8, 6, 3, 0, 0, 5 },
        { 0, 5, 0, 0, 9, 0, 6, 0, 0 },
        { 1, 3, 0, 0, 0, 0, 2, 5, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 7, 4 },
        { 0, 0, 5, 2, 0, 6, 3, 0, 0 }};
    csp_solver msm(grid);
    if(msm.solve()){
        msm.print();
    }else cout<<"no solution exist\n";
    return 0;
}