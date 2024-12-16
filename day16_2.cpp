#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <queue>
#include <tuple>
#include <set>
#include <unordered_set>
using namespace std;

int directionIndex(char dir)
{
    switch (dir)
    {
    case '^':
        return 0;
    case '>':
        return 1;
    case 'v':
        return 2;
    case '<':
        return 3;
    default:
        return -1;
    }
}

char directionChar(int dir)
{
    switch (dir)
    {
    case 0:
        return '^';
    case 1:
        return '>';
    case 2:
        return 'v';
    case 3:
        return '<';
    default:
        return '|';
    }
}

vector<char> rotate90deg(char head)
{
    vector<char> result;
    switch (head)
    {
    case '^':
        result.push_back('>');
        result.push_back('<');
        break;
    case '>':
        result.push_back('v');
        result.push_back('^');
        break;
    case 'v':
        result.push_back('<');
        result.push_back('>');
        break;
    case '<':
        result.push_back('^');
        result.push_back('v');
        break;
    }
    return result;
}

vector<pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

// same dijkstra from pt1 but has some addl logic added to track previous nodes of each position in every DIRECTION
int dijkstra(vector<vector<char>> &grid, pair<int, int> startPos, pair<int, int> endPos,
             vector<vector<vector<tuple<int, int, int>>>> &previous)
{
    int r = grid.size(), c = grid[0].size();

    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<>> pq;

    vector<vector<vector<int>>> cost(r, vector<vector<int>>(c, vector<int>(4, INT_MAX)));

    int startDir = directionIndex('>');
    pq.push({0, startPos.first, startPos.second, startDir});
    cost[startPos.first][startPos.second][startDir] = 0;

    previous[startPos.first][startPos.second][startDir] = {-1, -1, -1};

    while (!pq.empty())
    {
        int curCost, x, y, dir;
        tie(curCost, x, y, dir) = pq.top();
        pq.pop();

        if (make_pair(x, y) == endPos)
        {
            return curCost;
        }

        if (curCost > cost[x][y][dir])
            continue;

        // same direction
        int dx = x + directions[dir].first;
        int dy = y + directions[dir].second;
        if (dx >= 0 && dx < r && dy >= 0 && dy < c && grid[dx][dy] != '#' && grid[dx][dy] != 'O')
        {
            int nextCost = curCost + 1;
            if (nextCost < cost[dx][dy][dir])
            {
                cost[dx][dy][dir] = nextCost;
                previous[dx][dy][dir] = {x, y, dir};
                pq.push({nextCost, dx, dy, dir});
            }
        }

        // rotate 90 deg
        vector<char> heads = rotate90deg(directionChar(dir));
        for (auto head : heads)
        {
            int newDir = directionIndex(head);
            dx = x + directions[newDir].first;
            dy = y + directions[newDir].second;
            if (dx >= 0 && dx < r && dy >= 0 && dy < c && grid[dx][dy] != '#' && grid[dx][dy] != 'O')
            {
                int nextCost = curCost + 1001; // High rotation cost
                if (nextCost < cost[dx][dy][newDir])
                {
                    cost[dx][dy][newDir] = nextCost;
                    previous[dx][dy][newDir] = {x, y, dir};
                    pq.push({nextCost, dx, dy, newDir});
                }
            }
        }
    }

    return -1;
}

void printPrevious(vector<vector<vector<tuple<int, int, int>>>> &previous)
{
    cout << "Printing previous : " << "\n";
    for (int i = 0; i < previous.size(); i++)
    {
        for (int j = 0; j < previous[0].size(); j++)
        {
            for (int dir = 0; dir < 4; dir++)
            {
                cout << i << ", " << j << ", " << dir << " : ";
                cout << get<0>(previous[i][j][dir]) << ", " << get<1>(previous[i][j][dir]) << ", " << get<2>(previous[i][j][dir]) << "\n";
            }
        }
    }
}

// return a vector of possible directions you can backtrack in, based on the direction you're in rn
vector<int> reconstructionDirections(char initDir)
{
    // can't make 180deg turns
    vector<int> directions;
    switch (initDir)
    {
    case 0: // initDir = top
        directions.push_back(0);
        directions.push_back(1);
        directions.push_back(3);
        break;
    case 1: // right
        directions.push_back(0);
        directions.push_back(1);
        directions.push_back(2);
        break;
    case 2: // down
        directions.push_back(1);
        directions.push_back(2);
        directions.push_back(3);
        break;
    case 3: // left
        directions.push_back(0);
        directions.push_back(2);
        directions.push_back(3);
        break;
    default:
        break;
    }
    return directions;
}

// this basically is a backtracking function that does a dfs on all possible paths from E to S and checks if cost of that path = ref cost or not
void reconstructPath(vector<vector<vector<tuple<int, int, int>>>> &previous,
                     pair<int, int> startPos,
                     pair<int, int> endPos,
                     set<pair<int, int>> &pathNodes,
                     set<pair<int, int>> &temppathNodes,
                     vector<vector<vector<bool>>> &visited, int referenceCost, int currentPathCost, int prevDir)
{
    if (currentPathCost > referenceCost)
        return;
    if (startPos == endPos)
    {
        // add temppathnodes to pathnodes only if cost of path from E to here was same as referenceCost
        if (currentPathCost != referenceCost)
            return;
        pathNodes.insert(temppathNodes.begin(), temppathNodes.end());
        return;
    }

    int currX = endPos.first, currY = endPos.second;

    visited[currX][currY][prevDir] = true;

    vector<int> reconstructDirs = reconstructionDirections(prevDir);

    for (auto dir : reconstructDirs)
    {
        auto prevTuple = previous[currX][currY][dir];
        int prevX = get<0>(prevTuple), prevY = get<1>(prevTuple);

        if (prevX != -1 && prevY != -1)
        {
            temppathNodes.insert({currX, currY});
            // dfs from E to S
            if (!visited[prevX][prevY][dir])
            {
                if (prevDir == dir)
                    reconstructPath(previous, startPos, {prevX, prevY}, pathNodes, temppathNodes, visited, referenceCost, currentPathCost + 1, dir);
                else
                    reconstructPath(previous, startPos, {prevX, prevY}, pathNodes, temppathNodes, visited, referenceCost, currentPathCost + 1 + 1000, dir);
            }
            // remove node if it doesn't contribute to a valid path
            if (pathNodes.find({currX, currY}) == pathNodes.end())
            {
                temppathNodes.erase({currX, currY});
            }
        }
    }

    visited[currX][currY][prevDir] = false;
}

int main()
{
    string line;
    vector<vector<char>> grid;
    pair<int, int> startPos, endPos;

    while (getline(cin, line))
    {
        vector<char> row;
        for (char &c : line)
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

    cout << "rows : " << grid.size() << ", cols : " << grid[0].size() << endl;

    vector<vector<vector<tuple<int, int, int>>>> previous(
        grid.size(),
        vector<vector<tuple<int, int, int>>>(grid[0].size(),
                                             vector<tuple<int, int, int>>(4, {-1, -1, -1})));
    int referenceCost = dijkstra(grid, startPos, endPos, previous);
    cout << "Reference cost : " << referenceCost << endl;
    // printPrevious(previous);

    set<pair<int, int>> pathNodes;
    pathNodes.insert(startPos);
    set<pair<int, int>> temppathNodes;
    vector<vector<vector<bool>>> visited(r, vector<vector<bool>>(c, vector<bool>(4, false)));

    for (int dir = 0; dir < 4; dir++)
        reconstructPath(previous, startPos, endPos, pathNodes, temppathNodes, visited, referenceCost, 0, dir);

    for (const auto &node : pathNodes)
    {
        grid[node.first][node.second] = 'O';
    }

    int result = 0;
    for (const auto &row : grid)
    {
        for (char c : row)
        {
            cout << c;
            if (c == 'O')
                result++;
        }
        cout << endl;
    }

    cout << "Final result : " << result << endl;

    return 0;
}
