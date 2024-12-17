#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <climits>
using namespace std;

void printGrid(vector<vector<char>>& grid){
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            cout << grid[i][j];
        }
        cout << "\n";
    }
    cout.flush();
}


pair<int, int> translateMove(char move){
    unordered_map<char, pair<int, int>> translation;
    translation['>'] = { 0,  1 };
    translation['<'] = { 0, -1 };
    translation['^'] = {-1,  0 };
    translation['v'] = { 1,  0 };
    return translation[move];
}

bool isValid(int x, int y, int r, int c){
    return (x >= 0 && x < r && y >= 0 && y < c);
}

pair<int, int> moveRobot(vector<vector<char>>& grid, char move, pair<int, int> robotPos) {
    int r = grid.size(), c = grid[0].size();
    
    pair<int, int> dir = translateMove(move);
    int nx = robotPos.first + dir.first;
    int ny = robotPos.second + dir.second;

    if (!isValid(nx, ny, r, c)) return robotPos;

    if (grid[nx][ny] == '.') {
        grid[nx][ny] = '@';
        grid[robotPos.first][robotPos.second] = '.';
        return {nx, ny};
    }

    if (grid[nx][ny] == '#') return robotPos;

    if (grid[nx][ny] == '[' || grid[nx][ny] == ']') {
        // horizontal movement
        if (move == '<' || move == '>') {
            while (grid[nx][ny] != '.') {
                if (grid[nx][ny] == '#') return robotPos;
                ny += dir.second;
                if (!isValid(nx, ny, r, c)) return robotPos;
            }
            while (grid[nx][ny - dir.second] == '[' || grid[nx][ny - dir.second] == ']') {
                grid[nx][ny] = grid[nx][ny - dir.second];
                grid[nx][ny - dir.second] = '.';
                ny -= dir.second;
            }
            
            grid[nx][ny] = '@';
            grid[robotPos.first][robotPos.second] = '.';
            return {nx, ny};
        } 
        // vertical movement
        else {
            int left = ny, right = ny;
            if (grid[nx][ny] == '[') right = ny + 1;
            else left = ny - 1;
            
            unordered_map<int, pair<int, int>> bracketLimits;
            int leftMin = INT_MAX, rightMax = INT_MIN;

            // track connected boxes vertically
            int currentRow = nx; //current row being checked
            int targetRow = nx; //row where all clear so we can move boxes here
            bool canMove = false;
            bracketLimits[targetRow] = {left, right};
            
            while (true) {
                targetRow += dir.first;

                if (!isValid(targetRow, 0, r, c)) break;
                
                if(isValid(targetRow, left, r, c) && grid[targetRow][left] == ']'){
                    if(isValid(targetRow, left-1, r, c)) left--;
                }
                if(isValid(targetRow, right, r, c) && grid[targetRow][right] == '['){
                    if(isValid(targetRow, right+1, r, c)) right++;
                }
                
                bracketLimits[targetRow] = {left, right};

                // check if entire expanded region(start at left, end at right) is clear
                bool rowClear = true;
                for (int col = left; col <= right; col++) {
                    if (grid[targetRow][col] == '#') {
                        rowClear = false; break;
                    }
                }

                if (!rowClear) break;

                bool onlyDots = true;
                for (int col = left; col <= right; col++) {
                    if (grid[targetRow][col] != '.') {
                        onlyDots = false;
                        break;
                    }
                }

                if (onlyDots) {
                    canMove = true;
                    break;
                }

                currentRow = targetRow;
            }

            if (canMove) {
                // move the connected boxes
                if(move == '^'){
                    for (int row = currentRow; row <= nx; row += (-dir.first)) {
                        pair<int, int> bL = bracketLimits[row];
                        for (int col = bL.first; col <= bL.second; col++) {
                            if (grid[row][col] == '[' || grid[row][col] == ']') {
                                grid[targetRow][col] = grid[row][col];
                                grid[row][col] = '.';
                            }
                        }
                        targetRow++;
                    }
                }else{
                    for (int row = currentRow; row >= nx; row -= (dir.first)) {
                        pair<int, int> bL = bracketLimits[row];
                        for (int col = bL.first; col <= bL.second; col++) {
                            if (grid[row][col] == '[' || grid[row][col] == ']') {
                                grid[targetRow][col] = grid[row][col];
                                grid[row][col] = '.';
                            }
                        }
                        targetRow--;
                    }   
                }
                
                // Move robot
                grid[robotPos.first][robotPos.second] = '.';
                grid[targetRow][ny] = '@';
                return {targetRow, ny};
            }

            return robotPos;
        }
    }

    return robotPos;
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
            if(c == '#') {row.push_back(c); colInd++; row.push_back(c);}
            else if(c == 'O') {row.push_back('['); colInd++; row.push_back(']');}
            else if(c == '.') {row.push_back('.'); colInd++; row.push_back('.');}
            else {row.push_back('@'); row.push_back('.'); robotPos = {rowInd, colInd}; colInd++;}

            colInd ++;
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
        cout << "\nMove " << move << " : \n";
        //printGrid(grid);
    }
    
    cout << "After all the moves : " << "\n";
    printGrid(grid);
    
    long long result = 0;
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid[0].size(); j++){
            if(grid[i][j] == '[') {result += (j + 100*i);
            cout << "result : " << result << endl;}
        }
    }
    
    cout << "Final result : " << result << endl;

    return 0;
}
