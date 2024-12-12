/******************************************************************************
- dfs from each unique plot
    - gotchas : we can have unconnected regions so need to do mutiple dfs's for a single "type" of plot
        - I think I'll treat each region as separate and take its price(even though they have same plots but are disconnected) and then add prices int he end just like in XOXO example
- no of elements in the dfs path = area
- Perimeter calculations
    - each box in a plot has an initial perimeter of 4
    - the no of sides it is surrounded by same plot, it loses those many "perimeter sides" from the init perimeter
        - so if a B is surrounded on 2 sides by other Bs, it loses 2 from init peri => its perimeter contribution is 2 
*******************************************************************************/
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

bool isValid(int x, int y, int r, int c){
    return (x >= 0 && x < r && y >= 0 && y < c);
}

void dfs(int i, int j, vector<vector<char>>& garden, vector<vector<bool>>& visited, int& area, int& perimeter, char focusChar){
    if(visited[i][j]) return;
    
    int r = garden.size();
    int c = garden[0].size();
    
    visited[i][j] = true;
    
    int initPerimeter = 4;
    area++;
    
    int dirx[] = {0, -1, 0, 1};
    int diry[] = {-1, 0, 1, 0};
    
    for(int k = 0; k < 4; k++){
        int nx = i + dirx[k];
        int ny = j + diry[k];
        if(isValid(nx, ny, r, c) && !visited[nx][ny] && garden[nx][ny] == focusChar){
            dfs(nx, ny, garden, visited, area, perimeter, focusChar);
        }
        if(isValid(nx, ny, r, c) && garden[nx][ny] == focusChar) initPerimeter--;
    }
    
    perimeter += initPerimeter;
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
            int perimeter = 0;
            if(!visited[i][j]) {
                dfs(i, j, garden, visited, area, perimeter, garden[i][j]);
                price += (area*perimeter);
            }
             
        }
    }
    
    cout << "Final price : " << price << endl;
}
