#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

void printGrid(vector<vector<char>>& grid){
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) cout << grid[i][j] << " ";
        cout << "\n";
    }
}

pair<int, int> translateMove(char move){
    unordered_map<char, pair<int, int>> translation;
    translation['>'] = { 0,  1 };
    translation['<'] = { 0, -1 };
    translation['^'] = {-1,  0 };
    translation['v'] = { 1,  0 };
    return translation[move];
}

pair<int, int> moveRobot(vector<vector<char>>& grid, char move, pair<int, int> robotPos){
    pair<int, int> dir = translateMove(move);
    int nx = robotPos.first + dir.first;
    int ny = robotPos.second + dir.second;
    if(grid[nx][ny] == '.'){//move
        grid[robotPos.first][robotPos.second] = '.';
        grid[nx][ny] = '@';
        return {nx, ny};
    }else if(grid[nx][ny] == '#'){//wall. dont move
        return robotPos;
    }else{
        //find the first . that's before a wall and move robots and boxes a step in that direction
        //if you encounter a wall before a dot no need to move anything
        while(grid[nx][ny] != '.'){
            if(grid[nx][ny] == '#') return robotPos;
            nx += dir.first;
            ny += dir.second;
        }
        //we're at a dot that's right after a box. so we need to move all the boxes a step towards this dot until we reach the robot and then move the robot a step in this dir too
        while(nx != robotPos.first || ny != robotPos.second){
            int prevx = nx - dir.first;
            int prevy = ny - dir.second;
            
            grid[nx][ny] = 'O';
            grid[prevx][prevy] = '.';
            
            nx = prevx;
            ny = prevy;
        }
        
        grid[nx+dir.first][ny+dir.second] = '@';
        grid[nx][ny] = '.';
        return {nx+dir.first, ny+dir.second};
    }
    
}

int main() {
    string line;
    vector<vector<char>> grid;
    
    pair<int, int> robotPos;

    int rowInd = 0;
    while (getline(cin, line)) {
        if (line.empty()) break;
        vector<char> row;
        int colInd = 0;
        for (auto c : line) {
            row.push_back(c);
            if(c == '@') robotPos = {rowInd, colInd};
            colInd++;
        }
        grid.push_back(row);
        rowInd++;
    }

    string moves;
    while (getline(cin, line)) moves += line;
    
    cout << "Initial state : " << "\n";
    printGrid(grid);

    for(auto move: moves){
        robotPos = moveRobot(grid, move, robotPos);
    }
    
    cout << "After all the moves : " << "\n";
    printGrid(grid);
    
    long long result = 0;
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid[0].size(); j++){
            if(grid[i][j] == 'O') result += ((100*i) + j);
        }
    }
    
    cout << "Final result : " << result << endl;

    return 0;
}
