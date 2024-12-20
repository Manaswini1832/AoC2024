#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

bool isValid(int x, int y, int r, int c)
{
    return x >= 0 && y >= 0 && x < r && y < c;
}

vector<vector<int>> bfs(vector<vector<char>> &grid, pair<int, int> pos)
{
    int r = grid.size(), c = grid[0].size();
    vector<vector<int>> dist(r, vector<int>(c, INT_MAX));
    vector<vector<bool>> visited(r, vector<bool>(c, false));

    queue<pair<int, int>> q;
    q.push(pos);
    dist[pos.first][pos.second] = 0;
    visited[pos.first][pos.second] = true;

    int dirx[] = {0, -1, 0, 1};
    int diry[] = {-1, 0, 1, 0};

    while (!q.empty())
    {
        pair<int, int> top = q.front();
        q.pop();
        int x = top.first, y = top.second;

        for (int i = 0; i < 4; i++)
        {
            int nx = x + dirx[i], ny = y + diry[i];
            if (isValid(nx, ny, r, c) && grid[nx][ny] != '#' && !visited[nx][ny])
            {
                q.push({nx, ny});
                visited[nx][ny] = true;
                dist[nx][ny] = dist[x][y] + 1;
            }
        }
    }
    return dist;
}

int manhattanDistance(pair<int, int> pt1, pair<int, int> pt2)
{
    return abs(pt1.first - pt2.first) + abs(pt1.second - pt2.second);
}

// returns all the points at a manhattan distance <= depth from the point (x, y)
vector<pair<int, int>> getManhattanRadiusPoints(int x, int y, int r, int c, int depth)
{
    vector<pair<int, int>> result;

    for (int dx = -depth; dx <= depth; dx++)
    {
        for (int dy = -depth; dy <= depth; dy++)
        {
            int x_new = x + dx, y_new = y + dy;
            // if the new pt is valid(inside grid) & at a manhattanDistance of <= 20(pt2) or 2(pt1) from current pt, put it in the vector
            if (isValid(x_new, y_new, r, c) && manhattanDistance({x, y}, {x_new, y_new}) <= depth)
                result.push_back({x_new, y_new});
        }
    }

    return result;
}

int main()
{
    string line;
    vector<vector<char>> grid;
    pair<int, int> startPos, endPos;

    while (getline(cin, line))
    {
        vector<char> row;
        for (char c : line)
        {
            row.push_back(c);
        }
        grid.push_back(row);
    }

    int r = grid.size(), c = grid[0].size();

    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < c; j++)
        {
            if (grid[i][j] == 'S')
                startPos = {i, j};
            if (grid[i][j] == 'E')
                endPos = {i, j};
        }
    }

    vector<vector<int>> startDists = bfs(grid, startPos);
    vector<vector<int>> endDists = bfs(grid, endPos);

    int result = 0;

    cout << "Dist from start to end = " << startDists[endPos.first][endPos.second] << endl;

    // for each point(this_spot) on grid, go 2 or 20 steps in all dirs each time to a new_spot and
    // get dist(start, this_spot) + dist(this_spot, next_spot) + dist(next_spot, end)
    // if above dist >= 100 increment result

    int cheatDepth = 20;
    int maxPathGain = 100;

    for (int x = 0; x < r; x++)
    {
        for (int y = 0; y < c; y++)
        {
            if (grid[x][y] == '#')
                continue;

            vector<pair<int, int>> pts = getManhattanRadiusPoints(x, y, r, c, cheatDepth);

            for (auto pt : pts)
            {
                int nx = pt.first, ny = pt.second;
                if (grid[nx][ny] == '#')
                    continue;
                int newDist = startDists[x][y] + manhattanDistance({x, y}, {nx, ny}) + endDists[nx][ny];
                if (startDists[endPos.first][endPos.second] - newDist >= maxPathGain)
                {
                    result++;
                }
            }
        }
    }

    cout << "Final result = " << result << endl;

    return 0;
}
