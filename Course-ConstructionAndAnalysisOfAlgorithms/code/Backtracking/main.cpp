//
#include <iostream>
#include <vector>

using namespace std;


class Square{
public:
    int x;
    int y;
    int size;
    Square(int x, int y, int size): x(x), y(y), size(size){};
    
};

int bestRes = 0;
int scale = 1;
vector<Square> bestSquareList;
vector<vector<int>> bestMap;

void drawMap(vector<vector<int>>& map){
    for (int i = 0; i < map.size(); i++){
        for(int j = 0; j < map.size(); j++){
            cout<< (char) (map[i][j]+'0');
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<endl;
}
void addSquareToMap(int x, int y, int size, int reclvl, vector<vector<int>>& map){
    size*=scale;
    for(int j = x; j < x + size; j++){
       for(int k = y; k < y + size; k++){
           map[j][k] = reclvl;
       }
   }
}

void backtracking(int n, vector<Square>& squareList, vector<vector<int>>& map, int startSize, int emptyCells, int reclvl = 1){
    
    if(bestRes != 0 && reclvl >= bestRes){//optimization
        return;
    }
    
}


void start(int n){
    vector<Square> squareList;
    vector<vector<int>> map;
    vector<int> tmp (n, 0);
    
    for(int i = 0; i < n; i++){
        map.push_back(tmp);
    }
    for(int i = 2; i <=n; i++){
           if(n % i == 0){
               scale = n/i;
               n = i;
               break;
           }
    }
    backtracking(n, squareList, map, (n+1)/2, n*n);
}

int main() {
    cout<<"Enter size:"<<endl;
    int n;
    cin>>n;
    start(n);
    
    cout<<bestRes<<endl;
    for(int i = 0; i < bestRes; i++){
        cout<<bestSquareList[i].x+1<<" "<<bestSquareList[i].y+1<<" "<<bestSquareList[i].size<<endl;
    }
    
    drawMap(bestMap);
    return 0;
}
