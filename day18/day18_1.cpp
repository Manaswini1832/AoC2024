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

// method1 : bfs
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

// method2 : dijkstra's shortest path algorithm
int dijkstra(vector<vector<char>> &grid, pair<int, int> startPos, pair<int, int> endPos)
{
    int r = grid.size(), c = grid[0].size();

    vector<vector<int>> cost(r, vector<int>(c, INT_MAX));

    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> pq;
    pq.push({0, startPos.first, startPos.second});

    int dirx[] = {0, -1, 0, 1};
    int diry[] = {-1, 0, 1, 0};

    while (!pq.empty())
    {
        int currCost = pq.top()[0], x = pq.top()[1], y = pq.top()[2];
        pq.pop();

        for (int i = 0; i < 4; i++)
        {
            int nx = x + dirx[i];
            int ny = y + diry[i];

            if (isValid(nx, ny, r, c) && grid[nx][ny] != '#')
            {
                int nextCost = currCost + 1;
                if (nextCost < cost[nx][ny])
                {
                    cost[nx][ny] = nextCost;
                    pq.push({nextCost, nx, ny});
                }
            }
        }
    }

    return cost[endPos.first][endPos.second];
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

    int bytesFallen = 1024;
    for (int i = 0; i < bytesFallen && i < obstacles.size(); i++)
    {
        grid[obstacles[i].first][obstacles[i].second] = '#';
    }

    // int cost = minCostReachEnd(grid, {0, 0}, {r - 1, c - 1});
    int cost = dijkstra(grid, {0, 0}, {r - 1, c - 1});
    cout << "Final result : " << cost << endl;

    return 0;
}
