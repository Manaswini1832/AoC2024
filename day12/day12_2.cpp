/******************************************************************************
- dfs from each unique plot
    - gotchas : we can have unconnected regions so need to do mutiple dfs's for a single "type" of plot
        - I think I'll treat each region as separate and take its price(even though they have same plots but are disconnected) and then add prices int he end just like in XOXO example
- no of elements in the dfs path = area
- Side calculations
    - clue from reddit : an n-sided polygon has n-corners
    - interpreting the clue : a corner is formed by a square of a region if it is surrounded on three sides by different squares("different" can also be an out-of-garden square)
*******************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <unordered_map>
using namespace std;

bool isValid(int x, int y, int r, int c){
    return (x >= 0 && x < r && y >= 0 && y < c);
}

void dfs(int i, int j, vector<vector<char>>& garden, vector<vector<bool>>& visited, int& area, char focusChar, vector<vector<int>>& labeledGarden, int label){
    if(visited[i][j]) return;
    
    int r = garden.size();
    int c = garden[0].size();
    
    visited[i][j] = true;
    labeledGarden[i][j] = label;
    
    area++;
    
    int dirx[] = {0, -1, 0, 1};
    int diry[] = {-1, 0, 1, 0};
    
    for(int k = 0; k < 4; k++){
        int nx = i + dirx[k];
        int ny = j + diry[k];
        if(isValid(nx, ny, r, c) && !visited[nx][ny] && garden[nx][ny] == focusChar){
            dfs(nx, ny, garden, visited, area, focusChar, labeledGarden, label);
        }
    }
}

void cornerDfs(int i, int j, vector<vector<int>>& garden, vector<vector<bool>>& visited, set<vector<double>>& cornersCreated, int focusChar){
    if(!visited[i][j]) return;
    
    int r = garden.size();
    int c = garden[0].size();
    
    visited[i][j] = false;
    
    int dirx[] = {0, -1, 0, 1};
    int diry[] = {-1, 0, 1, 0};
    
    for(int k = 0; k < 4; k++){
        int nx = i + dirx[k];
        int ny = j + diry[k];
        if(isValid(nx, ny, r, c) && visited[nx][ny] && garden[nx][ny] == focusChar){
            cornerDfs(nx, ny, garden, visited, cornersCreated, focusChar);
        }
    }
    
    for(int k = 0; k < 4; k++){
        //check if four corners are being formed
        int x1 = i+dirx[k], y1 = j+diry[k];
        int x2 = i+dirx[(k+1)%4], y2 = j+diry[(k+1)%4];
        int x3 = x1+x2-i, y3 = y1+y2-j;
        
        //each corner surrounded by these 3squares + this one garden square
        int x[] = {x1, x2, x3};
        int y[] = {y1, y2, y3};
    
        vector<int> typesOfSurrounding(2, 0); //first num = no of garden squares, second num = types of non-garden squares(includes out of garden squares too)
        
        for(int m = 0; m < 3; m++){
            //here 2 cases can occur
                //case1 : corner surrounded on 3sides by non garden[i][j] and on one side by garden[i][j]
                //case2 : corner surrounded on 3sides by garden[i][j] and on one side by non garden[i][j]
            if((isValid(x[m], y[m], r, c) && garden[x[m]][y[m]] != garden[i][j]) || (!isValid(x[m], y[m], r, c))){
                typesOfSurrounding[1]++;
            }
             
            else if((isValid(x[m], y[m], r, c) && garden[x[m]][y[m]] == garden[i][j])) typesOfSurrounding[0]++;
        }
        
        if((typesOfSurrounding[0] == 0 && typesOfSurrounding[1] == 3) || (typesOfSurrounding[0] == 2 && typesOfSurrounding[1] == 1)){
            //get corner coords in midpoint terms
            double cornerx, cornery;
            if ((k == 0) || (k == 2)) {
                cornerx = (static_cast<double>(x3 + x1)) / 2.0;
                cornery = (static_cast<double>(y3 + y2)) / 2.0;
            } else {
                cornerx = (static_cast<double>(x3 + x2)) / 2.0;
                cornery = (static_cast<double>(y3 + y1)) / 2.0;
            }
            cornersCreated.insert({cornerx, cornery});
        }
        else if(typesOfSurrounding[0] == 1 && typesOfSurrounding[1] == 2){
            if((!isValid(i+x3, j+y3, r, c)) || (garden[i][j] != garden[i+x3][j+y3])) continue;
            //diagonally opposite points are different regions?
            set<int> uniqueRegions;
            
            for(int m = 0; m < 3; m++){
                if((isValid(x[m], y[m], r, c) && garden[x[m]][y[m]] != garden[i][j]) || (!isValid(x[m], y[m], r, c))){
                    if(isValid(x[m], y[m], r, c)){
                        uniqueRegions.insert(garden[x[m]][y[m]]);
                    }
                }
            }
            
            // we have 2 diff surrounding regions (excluding the current region)
            if(uniqueRegions.size() == 2){
                // get corner coords in midpoint terms
                double cornerx, cornery;
                if ((k == 0) || (k == 2)) {
                    cornerx = (static_cast<double>(x3 + x1)) / 2.0;
                    cornery = (static_cast<double>(y3 + y2)) / 2.0;
                } else {
                    cornerx = (static_cast<double>(x3 + x2)) / 2.0;
                    cornery = (static_cast<double>(y3 + y1)) / 2.0;
                }
    
                vector<double> cornerCoord = {cornerx, cornery, static_cast<double>(i), static_cast<double>(j)};
                cornersCreated.insert(cornerCoord);
            }
        }
    }
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
    
    vector<vector<int>> labeledGarden(garden.size(), vector<int>(garden[0].size(), -1));
    int label = 0;
    
    vector<vector<bool>> visited(garden.size(), vector<bool>(garden[0].size(), false));
    
    //int price = 0;
    unordered_map<int, int> price;
    
    for(int i = 0; i < garden.size(); i++){
        for(int j = 0; j < garden[0].size(); j++){
            int area = 0;
            if(!visited[i][j]) {
                label++;
                dfs(i, j, garden, visited, area, garden[i][j], labeledGarden, label);
                price[label] = area;
            }
        }
    }
    
    cout << "labelled garden" << "\n";
    for(int i = 0; i < labeledGarden.size(); i++){
        for(int j = 0; j < labeledGarden[0].size(); j++){
            if(visited[i][j]) {
                set<vector<double>> cornersCreated;
                int label = labeledGarden[i][j];
                cornerDfs(i, j, labeledGarden, visited, cornersCreated, labeledGarden[i][j]);
                int cornersNum = cornersCreated.size();
                price[label] *=  cornersNum;
                //cout << "Region starting at " << labeledGarden[i][j] << " at " << i << ", " << j << " has " << cornersNum << " corners" << "\n";
            }
        }
    }
    
    int result = 0;
    for(auto entry: price){
        result += entry.second;
    }
    
    cout << "Final price : " << result << endl;
}
