#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <queue>
#include <tuple>
#include <set>
#include <unordered_map>

using namespace std;

vector<pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

int directionIndex(char dir) {
    switch (dir) {
        case '^': return 0;
        case '>': return 1;
        case 'v': return 2;
        case '<': return 3;
        default: return -1;
    }
}

char directionChar(int dir) {
    switch (dir) {
        case 0: return '^';
        case 1: return '>';
        case 2: return 'v';
        case 3: return '<';
        default: return '|';
    }
}

vector<char> rotate90deg(char head) {
    vector<char> result;
    switch (head) {
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
        case 'S':
            result.push_back('^');
            result.push_back('v');
            break;
        default:
            break;
    }
    return result;
}

int dijkstra(vector<vector<char>>& grid, pair<int, int> startPos, pair<int, int> endPos) {
    int r = grid.size(), c = grid[0].size();
    
    //cost, x, y, directionCharacter
    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<>> pq;
    
    vector<vector<vector<int>>> cost(r, vector<vector<int>>(c, vector<int>(4, INT_MAX)));
    
    int startDir = directionIndex('>');
    pq.push({0, startPos.first, startPos.second, startDir});
    cost[startPos.first][startPos.second][startDir] = 0;

    while (!pq.empty()) {
        int curCost, x, y, dir;
        tie(curCost, x, y, dir) = pq.top();
        pq.pop();

        if (make_pair(x, y) == endPos) {
            return curCost;
        }

        // same direction
        int dx = x + directions[dir].first;
        int dy = y + directions[dir].second;
        if (dx >= 0 && dx < r && dy >= 0 && dy < c && grid[dx][dy] != '#') {
            int nextCost = curCost + 1;
            if (nextCost < cost[dx][dy][dir]) {
                cost[dx][dy][dir] = nextCost;
                pq.push({nextCost, dx, dy, dir});
            }
        }

        // rotate 90degs
        vector<char> heads = rotate90deg(directionChar(dir));
        for (auto head : heads) {
            int newDir = directionIndex(head);
            dx = x + directions[newDir].first;
            dy = y + directions[newDir].second;
            if (dx >= 0 && dx < r && dy >= 0 && dy < c && grid[dx][dy] != '#') {
                int nextCost = curCost + 1 + 1000;
                if (nextCost < cost[dx][dy][newDir]) {
                    cost[dx][dy][newDir] = nextCost;
                    pq.push({nextCost, dx, dy, newDir});
                }
            }
        }
    }

    return -1;
}

int main() {
    string line;
    vector<vector<char>> grid;
    pair<int, int> startPos, endPos;
    int rowInd = 0;

    while (getline(cin, line)) {
        vector<char> row;
        for (char& c : line) {
            row.push_back(c);
            if (c == 'S') startPos = {rowInd, row.size() - 1};
            if (c == 'E') endPos = {rowInd, row.size() - 1};
        }
        grid.push_back(row);
        rowInd++;
    }

    int result = dijkstra(grid, startPos, endPos);
    cout << "Final result: " << (result == INT_MAX ? -1 : result) << endl;

    return 0;
}

