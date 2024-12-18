#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

bool isValid(int x, int y, int r, int c)
{
    return (x >= 0 && x < r && y >= 0 && y < c);
}

int minCostReachEnd(vector<vector<char>> &grid, pair<int, int> startPos, pair<int, int> endPos)
{
    int r = grid.size(), c = grid[0].size();
    vector<vector<bool>> visited(r, vector<bool>(c, false));
    queue<vector<int>> q;
    q.push({startPos.first, startPos.second, 0});
    visited[startPos.first][startPos.second] = true;

    int dirx[] = {0, -1, 0, 1};
    int diry[] = {-1, 0, 1, 0};

    while (!q.empty())
    {
        auto front = q.front();
        q.pop();

        int x = front[0], y = front[1];

        if (x == endPos.first && y == endPos.second)
        {
            return front[2];
        }

        for (int i = 0; i < 4; i++)
        {
            int nx = x + dirx[i];
            int ny = y + diry[i];

            if (isValid(nx, ny, r, c) && !visited[nx][ny] && grid[nx][ny] != '#')
            {
                visited[nx][ny] = true;
                q.push({nx, ny, front[2] + 1});
            }
        }
    }

    return -1;
}

int main()
{
    int r = 71, c = 71;
    vector<vector<char>> grid(r, vector<char>(c, '.'));

    vector<pair<int, int>> obstacles;
    string line;

    while (getline(cin, line))
    {
        if (line.empty())
            break;
        string numStr;
        int rowInd, colInd;
        int commInd = line.find(',');
        for (int i = 0; i < commInd; i++)
            numStr += line[i];
        colInd = stoi(numStr);
        numStr = "";
        for (int i = commInd + 1; i < line.length(); i++)
            numStr += line[i];
        rowInd = stoi(numStr);

        if (isValid(rowInd, colInd, r, c))
        {
            obstacles.push_back({rowInd, colInd});
        }
    }

    for (int i = 0; i < obstacles.size(); i++)
    {
        grid[obstacles[i].first][obstacles[i].second] = '#';
        int cost = minCostReachEnd(grid, {0, 0}, {r - 1, c - 1});
        if (cost == -1)
        {
            cout << "No path when " << obstacles[i].first << ", " << obstacles[i].second << " falls" << endl;
            break;
        }
        // else{
        //     cout << "Final result : " << cost << endl;
        // }
    }

    return 0;
}