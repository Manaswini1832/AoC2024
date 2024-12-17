/*
Guessing that we need to find pairs of same frequency antennas
    - for each pair, make a line equation
    - count the number of points of the grid that are on this line
*/

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

struct Line {
    int A, B, C;

    Line(vector<int> pt1, vector<int> pt2) {
        A = pt2[1] - pt1[1];
        B = pt1[0] - pt2[0];
        C = -(A * pt1[0] + B * pt1[1]);
    }

    bool isPointOnLine(vector<int> pt) {
        return (A * pt[0] + B * pt[1] + C) == 0;
    }
};

int countPtsOnLine(Line* line, vector<vector<char>>& grid, char key){
    int result = 0;
    for(int i = 0; i < grid.size(); i++){
        for(int j = 0; j < grid[0].size(); j++){
            if(grid[i][j] == '#') continue;
            if(grid[i][j] == '.' && line->isPointOnLine({i, j})) {
                grid[i][j] = '#';
                result++;
            }
        }
    }
    return result;
}

int countAntiNodes(vector<vector<char>>& grid){
    int r = grid.size();
    int c = grid[0].size();
    
    unordered_map<char, vector<vector<int>>> antennas;
    
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            if(grid[i][j] == '.') continue;
            antennas[grid[i][j]].push_back({i, j});
        }
    }
    
    int result = 0;
    
    for(auto& [key, indices]: antennas){
        for(int i = 0; i < indices.size(); i++){
            for(int j = i+1; j < indices.size(); j++){
                Line line(indices[i], indices[j]);
                result += countPtsOnLine(&line, grid, key);
            }
        }
    }
    
    for(auto& [key, indices]: antennas){
        result += indices.size();
    }
    
    return result;
}

int main()
{
    vector<vector<char>> grid;
    string input;
    while(getline(cin, input)){
        vector<char> charVector(input.begin(), input.end());
        grid.push_back(charVector);
    }
    
    int result = countAntiNodes(grid);
    cout << "Final result : " << result << endl;

    return 0;
}
