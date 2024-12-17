/*
aoc6.2 :
- In each X position(exclude start) calculated from pt1, put a O and check for a cycle
- hint from subreddit : if an obstacle is reached while going in the same direction more than once, there's a cycle

Lesson : learnt that to put a pair as a key in an unordered_map, we need to provide a custom hash function

ALSO TELL ME WHY I WAS WALKING OVER AND STANDING ON AN OBSTACLE INSTEAD OF AROUND IT?????t_t_t_t_t
DEBUGGED AND THERE IT WAST_T_T_T
*/

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

bool isValid(int x, int y, int r, int c)
{
    return (x >= 0 && x < r && y >= 0 && y < c);
}

bool corner(int x, int y, int r, int c)
{
    return (x == 0 || x == r - 1 || y == 0 || y == c - 1);
}

void dfs(vector<vector<char>> &grid, pair<int, int> guardPos, char guard,
         unordered_map<char, pair<int, int>> &direction, unordered_map<char, char> &arrow)
{
    int r = grid.size();
    int c = grid[0].size();

    int x = guardPos.first;
    int y = guardPos.second;

    if (grid[x][y] != ':')
        grid[x][y] = 'X';

    int dirx = direction[guard].first;
    int diry = direction[guard].second;

    int nx = x + dirx;
    int ny = y + diry;

    if (isValid(nx, ny, r, c))
    {
        if (!corner(nx, ny, r, c) && grid[nx][ny] != '#') // can move
            dfs(grid, {nx, ny}, guard, direction, arrow);
        if (!corner(nx, ny, r, c) && grid[nx][ny] == '#') // turn 90deg right
            dfs(grid, {x + direction[arrow[guard]].first, y + direction[arrow[guard]].second}, arrow[guard], direction, arrow);
        if (corner(nx, ny, r, c) && grid[nx][ny] != '#')
        { // reached corner stop dfs
            grid[nx][ny] = 'X';
            return;
        }
        if (corner(nx, ny, r, c) && grid[nx][ny] == '#') // turn 90deg right
            dfs(grid, {x + direction[arrow[guard]].first, y + direction[arrow[guard]].second}, arrow[guard], direction, arrow);
    }
}

struct pair_hash
{
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const
    {
        auto h1 = hash<T1>{}(p.first);
        auto h2 = hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

bool cyclePresent(int ox, int oy, vector<vector<char>> grid, pair<int, int> guardPos, char guard,
                  unordered_map<char, pair<int, int>> &direction,
                  unordered_map<char, char> &arrow,
                  unordered_map<pair<int, int>, vector<char>, pair_hash> &cache)
{
    if (ox != -1 && oy != -1)
    {
        grid[ox][oy] = 'O';
    }

    int r = grid.size();
    int c = grid[0].size();

    int x = guardPos.first;
    int y = guardPos.second;

    int dirx = direction[guard].first;
    int diry = direction[guard].second;

    int nx = x + dirx;
    int ny = y + diry;

    pair<int, int> key = {x, y};

    if (isValid(nx, ny, r, c))
    {
        if (!corner(nx, ny, r, c) && grid[nx][ny] != '#' && grid[nx][ny] != 'O')
        {
            return cyclePresent(-1, -1, grid, {nx, ny}, guard, direction, arrow, cache);
        }

        if (!corner(nx, ny, r, c) && (grid[nx][ny] == '#' || grid[nx][ny] == 'O'))
        {
            if (cache.find(key) != cache.end() &&
                find(cache[key].begin(), cache[key].end(), guard) != cache[key].end())
            {
                // cycle
                return true;
            }

            cache[key].push_back(guard);

            // Where to go next. right. but if there's an obstacle there go down. if there's an obstacle there too go left
            nx = x + direction[arrow[guard]].first;
            ny = y + direction[arrow[guard]].second;
            if (grid[nx][ny] != '#' && grid[nx][ny] != 'O')
            {
                guard = arrow[guard];
                return cyclePresent(-1, -1, grid, {nx, ny},
                                    guard, direction, arrow, cache);
            }

            guard = arrow[arrow[guard]];
            nx = x + direction[guard].first;
            ny = y + direction[guard].second;
            return cyclePresent(-1, -1, grid, {nx, ny},
                                guard, direction, arrow, cache);
        }

        if (corner(nx, ny, r, c) && (grid[nx][ny] != '#' && grid[nx][ny] != 'O'))
        {
            return false;
        }

        if (corner(nx, ny, r, c) && (grid[nx][ny] == '#' || grid[nx][ny] == 'O'))
        {
            if (cache.find(key) != cache.end() &&
                find(cache[key].begin(), cache[key].end(), guard) != cache[key].end())
            {
                // cycle
                return true;
            }

            cache[key].push_back(guard);

            // Where to go next. right. but if there's an obstacle there go down. if there's an obstacle there too go left
            nx = x + direction[arrow[guard]].first;
            ny = y + direction[arrow[guard]].second;
            if (grid[nx][ny] != '#' && grid[nx][ny] != 'O')
            {
                guard = arrow[guard];
                return cyclePresent(-1, -1, grid, {nx, ny},
                                    guard, direction, arrow, cache);
            }

            guard = arrow[arrow[guard]];
            nx = x + direction[guard].first;
            ny = y + direction[guard].second;
            return cyclePresent(-1, -1, grid, {nx, ny},
                                guard, direction, arrow, cache);
        }
    }

    return false;
}

int main()
{
    // direction lookup based on guard's current position
    unordered_map<char, pair<int, int>> direction;
    direction['^'] = {-1, 0};
    direction['>'] = {0, 1};
    direction['v'] = {1, 0};
    direction['<'] = {0, -1};

    unordered_map<char, char> arrow;
    arrow['^'] = '>';
    arrow['>'] = 'v';
    arrow['v'] = '<';
    arrow['<'] = '^';

    vector<vector<char>> grid;
    string line;
    pair<int, int> guardPos;
    char guard;

    int rowNum = 0;
    int colNum = 0;

    while (getline(cin, line))
    {
        vector<char> row;
        rowNum++;

        for (auto c : line)
        {
            colNum++;
            if (c == '^' || c == '>' || c == '<' || c == 'v')
            {
                guardPos.first = rowNum - 1;
                guardPos.second = colNum - 1;
                guard = c;
            }
            row.push_back(c);
        }
        colNum = 0;

        grid.push_back(row);
    }

    grid[guardPos.first][guardPos.second] = ':'; // guard's starting pos

    dfs(grid, guardPos, guard, direction, arrow);

    int result = 0;
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[0].size(); j++)
        {
            if (grid[i][j] == 'X')
            {
                unordered_map<pair<int, int>, vector<char>, pair_hash> cache;
                if (cyclePresent(i, j, grid, guardPos, guard, direction, arrow, cache))
                {
                    result++;
                }
            }
        }
    }

    cout << "Final result : " << result << endl;
    return 0;
}
