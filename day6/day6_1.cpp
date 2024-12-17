#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

bool isValid(int x, int y, int r, int c){
    return (x >= 0 && x < r && y >= 0 && y < c);
}

bool corner(int x, int y, int r, int c){
    return (x == 0 || x == r-1 || y == 0 || y == c-1);
}

void dfs(vector<vector<char>>& grid, pair<int, int> guardPos, char guard, 
unordered_map<char, pair<int, int>>& direction, unordered_map<char, char>& arrow){
    int r = grid.size();
    int c = grid[0].size();
    
    int x = guardPos.first;
    int y = guardPos.second;
    
    grid[x][y] = 'X';
    
    int dirx = direction[guard].first;
    int diry = direction[guard].second;
    
    int nx = x + dirx;
    int ny = y + diry;
    
    if(isValid(nx, ny, r, c)){
        if(!corner(nx, ny, r, c) && grid[nx][ny] != '#') //can move
            dfs(grid, {nx, ny}, guard, direction, arrow);
        if(!corner(nx, ny, r, c) && grid[nx][ny] == '#') //turn 90deg right
            dfs(grid, {x+direction[arrow[guard]].first, y+direction[arrow[guard]].second}, arrow[guard], direction, arrow);
        if(corner(nx, ny, r, c) && grid[nx][ny] != '#') //reached corner stop dfs
            return;
        if(corner(nx, ny, r, c) && grid[nx][ny] == '#') //turn 90deg right
            dfs(grid, {x+direction[arrow[guard]].first, y+direction[arrow[guard]].second}, arrow[guard], direction, arrow);
    }
}

int main()
{
    //direction lookup based on guard's current pos
    unordered_map<char, pair<int, int>> direction;
    direction['^'] = {-1, 0};
    direction['>'] = {0,  1};
    direction['v'] = {1,  0};
    direction['<'] = {0, -1};
    
    //how arrow looks when 90deg turn happens
    unordered_map<char, char> arrow;
    arrow['^'] = '>';
    arrow['>'] = 'v';
    arrow['v'] = '<';
    arrow['<'] = '^';
    
    //inp
    vector<vector<char>> grid;
    string line;
    pair<int, int> guardPos;
    char guard;
    
    int rowNum = 0;
    int colNum = 0;
    
    while(getline(cin, line)){
        vector<char> row;
        rowNum++;
        
        for(auto c: line){
            colNum++;
            if(c == '^' || c == '>' || c == '<' || c == 'v') {
                guardPos.first = rowNum-1;
                guardPos.second = colNum-1;
                guard = c;
            }
            row.push_back(c);
        }
        colNum = 0;
        
        grid.push_back(row);
    }
    
    //dfs
    dfs(grid, guardPos, guard, direction, arrow);
    
    //out
    int result = 1;
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid[0].size(); j++){
            if(grid[i][j] == 'X') result++;
        }
    }
    
    cout << "Final result : " << result << endl;

    return 0;
}
