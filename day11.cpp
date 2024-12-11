/*
Caching transformations won't help with time complexity since it's speeding up what's already sort of fast
The issue here is that the search space is super huge and we need to prune as much of it as possible
So instead of caching transformations we can cache this : if at a blink of say 25, I got the num 3 and I've
already seen all the nums till depth = 75 starting from this 3, I'll cache this so that if I again encounter
a 3 at the same depth/blink, I can look that up in the cache instead of going through the entire tree of transformns again
*/

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

void trimLeadingZeros(string &num)
{
    while (!num.empty() && num[0] == '0')
    {
        num.erase(0, 1);
    }
    if (num.empty())
        num = "0";
}

vector<string> getTwoStones(string stone)
{
    long long mid = stone.length() / 2;
    string num1 = stone.substr(0, mid);
    string num2 = stone.substr(mid);

    trimLeadingZeros(num1);
    trimLeadingZeros(num2);

    return {num1, num2};
}

string generateHash(string s1, string s2)
{
    return s1 + "|" + s2;
}

long long blink(vector<string> &stone, int depth, unordered_map<string, long long> &cache, int neededDepth)
{
    if (depth == neededDepth)
        return stone.size();

    long long result = 0;

    for (string &backStone : stone)
    {
        string key = generateHash(backStone, to_string(depth));
        if (cache.find(key) != cache.end())
        {
            result += cache[key];
            continue;
        }

        vector<string> backStoneTransformns;
        if (backStone == "0")
        {
            backStoneTransformns = {"1"};
        }
        else if (backStone.length() % 2 == 1)
        {
            long long num = stoll(backStone) * 2024;
            backStoneTransformns.push_back(to_string(num));
        }
        else
        {
            backStoneTransformns = getTwoStones(backStone);
        }

        long long thisStoneResult = blink(backStoneTransformns, depth + 1, cache, neededDepth);

        cache[key] = thisStoneResult;
        result += thisStoneResult;
    }

    if (stone.size() == 1)
    {
        string stoneKey = generateHash(stone[0], to_string(depth));
        cache[stoneKey] = result;
    }

    return result;
}

int main()
{
    string line;
    vector<string> stones;
    while (getline(cin, line))
    {
        stringstream ss(line);
        string numstr;
        while (ss >> numstr)
        {
            stones.push_back(numstr);
        }
    }

    unordered_map<string, long long> cache; // key should be a combo of num and depth at which num occured

    long long totalStones = 0;
    for (auto stone : stones)
    {
        vector<string> tempStone = {stone};
        long long numStones = blink(tempStone, 0, cache, 75);
        totalStones += numStones;
    }

    cout << "Total stones: " << totalStones << endl;
    return 0;
}
