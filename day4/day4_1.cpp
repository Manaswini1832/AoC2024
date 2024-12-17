#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*
Day 4 - part1
Go to each X and do a dfs of depth 4 in all 8 directions
while doing the dfs, keep building the string till the last and if laststr = XMAS, return 1 else 0
*/

bool isValid(int newx, int newy, int r, int c){
    return (newx >= 0 && newx < r && newy >= 0 && newy < c);
}

int dfs(vector<vector<char>>& grid, int i, int j, int iprev, int jprev, string sub, int depth){
    if (depth == 4) {
        return sub == "XMAS" ? 1 : 0;
    }
    
    int r = grid.size();
    int c = grid[0].size();
    
    int count = 0;
    
    int dirx[] = {0, 1, 0, -1, 1, -1, 1, -1};
    int diry[] = {-1, 0, 1, 0, -1, -1, 1, 1};
    
    if(iprev == 0 && jprev == 0){
        // Check all 8 directions
        for (int d = 0; d < 8; d++) {
            int x = i + dirx[d];
            int y = j + diry[d];
            if (isValid(x, y, r, c)) {
                count += dfs(grid, x, y, dirx[d], diry[d], sub + grid[x][y], depth + 1);
            }
        }
    }else{
        //keep going in this direction
        int x = i + iprev;
        int y = j + jprev;
        if (isValid(x, y, r, c)) {
            count += dfs(grid, x, y, iprev, jprev, sub + grid[x][y], depth + 1);
        }
    }
    
    return count;
}

int getXmas(vector<vector<char>>& grid){
    int count = 0;
    int r = grid.size();
    int c = grid[0].size();
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            if(grid[i][j] == 'X'){
                count += dfs(grid, i, j, 0, 0, "X", 1);
            }
        }
    }
    return count;
}

int main()
{
    vector<vector<char>> grid;
    string input;
    while(getline(cin, input)){
        vector<char> charVector(input.begin(), input.end());
        grid.push_back(charVector);
    }
    
    cout << getXmas(grid) << endl;


    return 0;
}
