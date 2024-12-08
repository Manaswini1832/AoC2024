#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
using namespace std;

bool inGrid(vector<int> pt, int r, int c){
    return (pt[0] >= 0 && pt[0] < r && pt[1] >= 0 && pt[1] < c);
}

vector<int> getPoint(vector<int> mid, vector<int> pt1){
    //find the pt that makes mid the midpoint of this point and pt1
    int x = 2*mid[0] - pt1[0];
    int y = 2*mid[1] - pt1[1];
    return {x, y};
}

set<vector<int>> getAntiNodes(vector<vector<char>>& grid){
    int r = grid.size();
    int c = grid[0].size();
    
    unordered_map<char, vector<vector<int>>> antennas;
    
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            if(grid[i][j] == '.') continue;
            antennas[grid[i][j]].push_back({i, j});
        }
    }
    
    set<vector<int>> antiPoints;
    
    for(auto& [key, indices]: antennas){
        for(int i = 0; i < indices.size(); i++){
            for(int j = i+1; j < indices.size(); j++){
                vector<int> otherPt = getPoint(indices[i], indices[j]);
                if(inGrid(otherPt, r, c)) antiPoints.insert(otherPt);
                otherPt = getPoint(indices[j], indices[i]);
                if(inGrid(otherPt, r, c)) antiPoints.insert(otherPt);
            }
        }
    }
    
    return antiPoints;
}

int main()
{
    vector<vector<char>> grid;
    string input;
    while(getline(cin, input)){
        vector<char> charVector(input.begin(), input.end());
        grid.push_back(charVector);
    }
    
    set<vector<int>> antiPoints = getAntiNodes(grid);
    int result = antiPoints.size();
    cout << "Final result : " << result << endl;

    return 0;
}
