// pt1 coded. should test for actual input
#include <iostream>
#include <string>
#include <vector>
#include <climits>
#include <unordered_map>
#include <queue>
#include <tuple>
#include <set>
#include <algorithm>
using namespace std;

vector<vector<char>> numPad = {
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {'X', '0', 'A'}};

vector<vector<char>> dirPad = {
    {'X', '^', 'A'},
    {'<', 'v', '>'}};

// some helper funcs
pair<int, int> getNumInds(char key)
{
    unordered_map<char, pair<int, int>> numInds;
    numInds['0'] = {3, 1};
    numInds['1'] = {2, 0};
    numInds['2'] = {2, 1};
    numInds['3'] = {2, 2};
    numInds['4'] = {1, 0};
    numInds['5'] = {1, 1};
    numInds['6'] = {1, 2};
    numInds['7'] = {0, 0};
    numInds['8'] = {0, 1};
    numInds['9'] = {0, 2};
    numInds['A'] = {3, 2};
    return numInds[key];
}

pair<int, int> getDirInds(char key)
{
    unordered_map<char, pair<int, int>> dirInds;
    dirInds['^'] = {0, 1};
    dirInds['A'] = {0, 2};
    dirInds['<'] = {1, 0};
    dirInds['v'] = {1, 1};
    dirInds['>'] = {1, 2};
    return dirInds[key];
}

string getHash(int i, int j)
{
    return to_string(i) + '|' + to_string(j);
}

bool isValid(int x, int y, int r, int c)
{
    return (x >= 0 && x < r && y >= 0 && y < c);
}

char getDirKeyFromInds(int xinit, int yinit, int nx, int ny)
{
    if (xinit == nx)
    {
        if (ny < yinit)
            return '<';
        return '>';
    }
    else
    {
        if (nx < xinit)
            return '^';
        return 'v';
    }
}

// function to give all shortest paths on dirpad, taking which we can get numCode on the numPad
vector<string> num2Dir(string numCode, string sol, int start, int end, unordered_map<string, vector<vector<vector<string>>>> &numSPs)
{
    if (end >= numCode.length())
        return {sol};
    pair<int, int> startInds;
    if (start == -1)
        startInds = {3, 2};
    else
        startInds = getNumInds(numCode[start]);
    pair<int, int> endInds = getNumInds(numCode[end]);
    vector<string> allPaths;
    string mapKey = getHash(startInds.first, startInds.second);
    for (auto path : numSPs[mapKey][endInds.first][endInds.second])
    {
        vector<string> ret = num2Dir(numCode, sol + path, end, end + 1, numSPs);
        for (auto retStr : ret)
            allPaths.push_back(retStr);
    }
    return allPaths;
}

// function to give all shortest paths on dirpad, taking which we can get dirCode on the prev dirPad
vector<string> dir2Dir(string dirCode, string sol, int start, int end, unordered_map<string, vector<vector<vector<string>>>> &dirSPs)
{
    if (end >= dirCode.length())
        return {sol};
    pair<int, int> startInds;
    if (start == -1)
        startInds = {0, 2};
    else
        startInds = getDirInds(dirCode[start]);
    pair<int, int> endInds = getDirInds(dirCode[end]);
    vector<string> allPaths;
    string mapKey = getHash(startInds.first, startInds.second);
    for (auto path : dirSPs[mapKey][endInds.first][endInds.second])
    {
        vector<string> ret = dir2Dir(dirCode, sol + path, end, end + 1, dirSPs);
        for (auto retStr : ret)
            allPaths.push_back(retStr);
    }
    return allPaths;
}

// function that gives out a "shortness" value for a given code
int value(string code)
{
    return code.length();
}

int getNumPart(string numCode)
{
    string numStr = "";
    for (auto c : numCode)
    {
        if ((c - '0') >= 0 && (c - '0') <= 9)
        {
            numStr += c;
        }
    }

    int result = stoi(numStr);

    return result;
}

vector<vector<vector<string>>> bfs(int i, int j, int padRows, int padCols)
{
    vector<vector<int>> cost(padRows, vector<int>(padCols, INT_MAX));
    vector<vector<vector<string>>> pathStrs(padRows, vector<vector<string>>(padCols));

    queue<tuple<int, int, int, string>> q;
    q.push({0, i, j, ""});

    vector<vector<char>> pad;
    if (padRows == 4)
        pad = numPad;
    else
        pad = dirPad;

    int dirx[] = {0, -1, 0, 1};
    int diry[] = {-1, 0, 1, 0};

    cost[i][j] = 0;
    pathStrs[i][j].push_back("");

    while (!q.empty())
    {
        auto [currCost, x, y, pathStr] = q.front();
        q.pop();

        for (int k = 0; k < 4; k++)
        {
            int nx = x + dirx[k], ny = y + diry[k];
            if (isValid(nx, ny, padRows, padCols) && pad[nx][ny] != 'X')
            {
                string newPathStr = pathStr + getDirKeyFromInds(x, y, nx, ny);
                int newCost = currCost + 1;

                if (newCost < cost[nx][ny])
                {
                    cost[nx][ny] = newCost;
                    pathStrs[nx][ny].clear();
                    pathStrs[nx][ny].push_back(newPathStr);
                    q.push({newCost, nx, ny, newPathStr});
                }
                else if (newCost == cost[nx][ny])
                {
                    if (find(pathStrs[nx][ny].begin(), pathStrs[nx][ny].end(), newPathStr) == pathStrs[nx][ny].end())
                    {
                        pathStrs[nx][ny].push_back(newPathStr);
                    }
                    q.push({newCost, nx, ny, newPathStr});
                }
            }
        }
    }

    for (int m = 0; m < padRows; m++)
    {
        for (int n = 0; n < padCols; n++)
        {
            for (int l = 0; l < pathStrs[m][n].size(); l++)
            {
                pathStrs[m][n][l] += 'A';
            }
        }
    }
    return pathStrs;
}

int main()
{
    // first enumerate all the shortest paths to go from a key on numPad to another key
    // translate each path to <,^,v,>,A combo
    unordered_map<string, vector<vector<vector<string>>>> numSPs; // key of map formed using indices of start, each element of vector is a combo of arrows used to get to that key from start
    for (int i = 0; i < numPad.size(); i++)
    {
        for (int j = 0; j < numPad[0].size(); j++)
        {
            string numSPsKey = getHash(i, j);
            if (numPad[i][j] == 'X')
                continue;
            vector<vector<vector<string>>> bfsRet = bfs(i, j, 4, 3);
            numSPs[numSPsKey] = bfsRet;
        }
    }

    // enumerate all ways to go from a key on dir pad to another key
    unordered_map<string, vector<vector<vector<string>>>> dirSPs; // key of map formed using indices of start, each element of vector is a combo of arrows used to get to that key from start
    for (int i = 0; i < dirPad.size(); i++)
    {
        for (int j = 0; j < dirPad[0].size(); j++)
        {
            string dirSPsKey = getHash(i, j);
            if (dirPad[i][j] == 'X')
                continue;
            vector<vector<vector<string>>> bfsRet = bfs(i, j, 2, 3);
            dirSPs[dirSPsKey] = bfsRet;
        }
    }

    long long finalResult = 0;

    string line;
    while (getline(cin, line))
    {
        string numCode = line;
        cout << "Numcode : " << numCode << endl;
        cout << "Numpad robot to 1st dir robot" << endl;

        // between numpad robot & 1st dir robot
        string sol = "";
        vector<string> numToFirstDir = num2Dir(numCode, sol, -1, 0, numSPs);

        // between 1st dir robot & 2nd dir robot
        cout << "1st dir robot to 2nd dir robot" << endl;
        vector<string> dirToSecondDir;
        int overallMinValue = INT_MAX;
        for (auto sp : numToFirstDir)
        {
            vector<string> sps = dir2Dir(sp, "", -1, 0, dirSPs);
            int currPathValue = value(sps[0]);
            if (currPathValue < overallMinValue)
            {
                overallMinValue = currPathValue;
                dirToSecondDir.clear();
                for (auto ele : sps)
                    dirToSecondDir.push_back(ele);
            }
            else if (currPathValue == overallMinValue)
            {
                for (auto ele : sps)
                    dirToSecondDir.push_back(ele);
            }
            else
            {
                continue;
            }
        }
        // between 2nd dir robot and me
        cout << "2nd dir robot to me" << endl;
        vector<string> dirToMe;
        overallMinValue = INT_MAX;
        for (auto sp : dirToSecondDir)
        {
            vector<string> sps = dir2Dir(sp, "", -1, 0, dirSPs);
            int currPathValue = value(sps[0]);
            if (currPathValue < overallMinValue)
            {
                overallMinValue = currPathValue;
                dirToMe.clear();
                for (auto ele : sps)
                    dirToMe.push_back(ele);
            }
            else if (currPathValue == overallMinValue)
            {
                for (auto ele : sps)
                    dirToMe.push_back(ele);
            }
            else
            {
                continue;
            }
        }

        int numPartOfCode = getNumPart(numCode);
        int lengthOfMySequence = overallMinValue;
        finalResult += (numPartOfCode * lengthOfMySequence);
    }

    cout << "Final result : " << finalResult << endl;

    return 0;
}