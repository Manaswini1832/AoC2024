/******************************************************************************
- dfs from each unique plot
    - gotchas : we can have unconnected regions so need to do mutiple dfs's for a single "type" of plot
        - I think I'll treat each region as separate and take its price(even though they have same plots but are disconnected) and then add prices int he end just like in XOXO example
- no of elements in the dfs path = area
- Side calculations
    - clue from reddit : an n-sided polygon has n-corners
    
    
this code tells the total length of the border segments and not the sides(this works correctly)
TODO : should merge edges of same side into a single side
*******************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <unordered_map>
#include <algorithm>
using namespace std;

bool isValid(int x, int y, int r, int c){
    return (x >= 0 && x < r && y >= 0 && y < c);
}

void dfs(int i, int j, vector<vector<char>>& garden, vector<vector<bool>>& visited, int& area, char focusChar, vector<vector<int>>& top, vector<vector<int>>& left, vector<vector<int>>& right, vector<vector<int>>& down){
    if(visited[i][j]) return;
    
    int r = garden.size();
    int c = garden[0].size();
    
    visited[i][j] = true;
    
    area++;
    
    int dirx[] = {0, -1, 0, 1};
    int diry[] = {-1, 0, 1, 0};
    
    for(int k = 0; k < 4; k++){
        int nx = i + dirx[k];
        int ny = j + diry[k];
        if (!isValid(nx, ny, r, c) || garden[nx][ny] != focusChar) {
            // borders.push_back({i, j, nx, ny});
            if(k == 0){
                left.push_back({i, j, nx, ny});
            }else if(k == 1){
                top.push_back({i, j, nx, ny});
            }else if(k == 2){
                right.push_back({i, j, nx, ny});
            }else{
                down.push_back({i, j, nx, ny});
            }
        } else if (!visited[nx][ny] && garden[nx][ny] == focusChar) {
            dfs(nx, ny, garden, visited, area, focusChar, top, left, right, down);
        }
    }
}

bool sameSide(vector<int> b1, vector<int> b2, int dir, vector<vector<int>>& borders){
    if((dir == 0) || (dir == 3)){//top, down
        if(b1[2] == b2[2]){
            //if starting at b1[2],b1[3] till b2[2],b2[3] all contiguous cols are in borders, only then merge
            if(abs(b1[2] - b2[2]) == 1) return true;
            for(int i = min(b1[3], b2[3]); i <= max(b1[3], b2[3]); i++){
                vector<int> toFind = {b1[2], i};
                if(find(borders.begin(), borders.end(), toFind) == borders.end()) return false;
            }
            return true;
        }
    }else{
        if(b1[3] == b2[3]){
            //if starting at b1[2],b1[3] till b2[2],b2[3] all contiguous rows are in borders, only then merge
            if(abs(b1[3] - b2[3]) == 1) return true;
            for(int i = min(b1[2], b2[2]); i <= max(b1[2], b2[2]); i++){
                vector<int> toFind = {i, b1[3]};
                if(find(borders.begin(), borders.end(), toFind) == borders.end()) return false;
            }
            return true;
        }
    }
}

int getSidesForEachDir(vector<vector<int>>& borders, int dir){
    unordered_map<int, vector<int>> parent; //maps parent border to a number
    parent[1] = borders[0];
    int parentIdx = 1;
    
    for(int i = 1; i < borders.size(); i++){
        vector<int> currentBorder = borders[i];
        bool placed = false;
        for(auto [key, value]: parent){
            if(sameSide(value, currentBorder, dir, borders)){
                placed = true;
                break;
            }
        }
        
        if(!placed){
            //setup a new parent in the hashmap
            parentIdx++;
            parent[parentIdx] = currentBorder;
        }
    }
    
    return parentIdx;
}

int mergeBorders(vector<vector<int>>& top, vector<vector<int>>& left, vector<vector<int>>& right, vector<vector<int>>& down){
    int topSides = getSidesForEachDir(top, 0);
    int leftSides = getSidesForEachDir(left, 1);
    int rightSides = getSidesForEachDir(right, 2);
    int downSides = getSidesForEachDir(down, 3);
    int totalSides = topSides+leftSides+rightSides+downSides;
    cout << "Total border segments = " << totalSides << "\n";
    return totalSides;
}

int main()
{
    vector<vector<char>> garden;
    string line;
    while(getline(cin, line)){
        vector<char> gardRow;
        for(auto c: line){
            gardRow.push_back(c);
        }
        garden.push_back(gardRow);
    }
    
    
    vector<vector<bool>> visited(garden.size(), vector<bool>(garden[0].size(), false));
    int price = 0;
    
    for(int i = 0; i < garden.size(); i++){
        for(int j = 0; j < garden[0].size(); j++){
            int area = 0;
            if(!visited[i][j]) {
                vector<vector<int>> top;
                vector<vector<int>> left;
                vector<vector<int>> right;
                vector<vector<int>> down;
                dfs(i, j, garden, visited, area, garden[i][j], top, left, right, down);
                cout << garden[i][j] << " at " << i << ", " << j << " has these" << "\n";
                int sides = mergeBorders(top, left, right, down);
                price += (area * sides);
            }
        }
    }
    
    //cout << "Final price : " << price << endl;
}
