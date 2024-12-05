#include <iostream>
#include <vector>
#include <string>
using namespace std;

/*
Day 4 - part2
Go to each A
Check on first diagonal : if a letter is M then is its diagonal opp S or vice-versa?
Check for the second diagonal too
If both diagonals satisfy the condition, count this A as a valid X-MAS
*/

bool isValid(int newx, int newy, int r, int c){
    return (newx >= 0 && newx < r && newy >= 0 && newy < c);
}

int masCheck(vector<vector<char>>& grid, int i, int j){
    int r = grid.size();
    int c = grid[0].size();
    
    int dirx[] = {-1, 1};
    int diry[] = {-1, -1};
    
    int tempCount = 0;
    for (int d = 0; d < 8; d++) {
        int x = i + dirx[d];
        int y = j + diry[d];
        int diagx = -1*dirx[d] + i;
        int diagy = -1*diry[d] + j;
        if (isValid(x, y, r, c) && isValid(diagx, diagy, r, c) && ((grid[x][y] == 'M' && grid[diagx][diagy] == 'S') || (grid[x][y] == 'S' && grid[diagx][diagy] == 'M'))) {
            tempCount++;
        }
    }
    
    if(tempCount == 2) return 1;
    return 0;
}

int getXmas(vector<vector<char>>& grid){
    int count = 0;
    int r = grid.size();
    int c = grid[0].size();
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            if(grid[i][j] == 'A'){
                count += masCheck(grid, i, j);
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
