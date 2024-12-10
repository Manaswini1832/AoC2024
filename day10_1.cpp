#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

bool isValid(int x, int y, int r, int c) {
    return x >= 0 && x < r && y >= 0 && y < c;
}

int dfs(int i, int j, int dirx_prev, int diry_prev, const vector<vector<int>>& topoMap, unordered_map<int, vector<int>>& done9) {
    int r = topoMap.size();
    int c = topoMap[0].size();
    
    if (topoMap[i][j] == 9){
        if(isValid(i-dirx_prev, j-diry_prev, r, c) && topoMap[i-dirx_prev][j-diry_prev] < topoMap[i][j]){
            //mark this 9 as done and return 1
            if(done9.find(i) == done9.end()){
                done9[i].push_back(j);
                return 1;
            }else{
                if(find(done9[i].begin(), done9[i].end(), j) == done9[i].end()){
                    done9[i].push_back(j);
                    return 1;
                }
                return 0;
            }
            
        }
        return 0;
    }

    int paths = 0;

    int dirx[] = {0, -1, 0, 1};
    int diry[] = {-1, 0, 1, 0};
    
    bool foundBranch = false;
    int branches = 0;
    if(topoMap[i][j] == 0 && dirx_prev == 0 && diry_prev == 0) foundBranch = true;

    for (int k = 0; k < 4; k++) {
        if (dirx[k] == -dirx_prev && diry[k] == -diry_prev) continue; //prevent going back on same path

        int nx = i + dirx[k];
        int ny = j + diry[k];

        if (isValid(nx, ny, r, c) && topoMap[nx][ny] == topoMap[i][j] + 1) {
            if(branches > 0) foundBranch = true;
            
            branches++;
            int dfsRes = dfs(nx, ny, dirx[k], diry[k], topoMap, done9);
            paths += dfsRes;
        }
    }
    
    return paths;
}

int main() {
    string line;
    vector<vector<int>> topoMap;

    while (getline(cin, line)) {
        vector<int> topoRow;
        for (char ch : line) {
            topoRow.push_back(ch - '0');
        }
        topoMap.push_back(topoRow);
    }

    int totalPaths = 0;

    for (int i = 0; i < topoMap.size(); i++) {
        for (int j = 0; j < topoMap[0].size(); j++) {
            if (topoMap[i][j] == 0) {
                unordered_map<int, vector<int>> done9;
                int paths = dfs(i, j, 0, 0, topoMap, done9);
                totalPaths += paths;
            }
        }
    }

    cout << "Final result: " << totalPaths << endl;

    return 0;
}
