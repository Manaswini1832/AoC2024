#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

long long canBeFormed(unordered_map<string, bool> &present, string design)
{
    // dp[i] = no of ways of forming first i letters of design word using words from present
    vector<long long> dp(design.length() + 1, 0);
    dp[0] = 1;

    for (int i = 1; i <= design.length(); i++)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            if (dp[j])
            {
                if (present.find(design.substr(j, i - j)) != present.end())
                {
                    dp[i] += dp[j];
                }
            }
        }
    }

    return dp[design.length()];
}

int main()
{
    string line;
    getline(cin, line);
    string towelStr;
    unordered_map<string, bool> present;
    for (auto c : line)
    {
        if (c == ' ')
            continue;
        if (c != ',')
            towelStr += c;
        else
        {
            present[towelStr] = true;
            towelStr = "";
        }
    }

    if (!towelStr.empty())
        present[towelStr] = true;

    cin.ignore();

    vector<string> designs;
    while (getline(cin, line))
    {
        designs.push_back(line);
    }

    long long result = 0;
    for (auto design : designs)
        result += canBeFormed(present, design);
    cout << "Final result : " << result << endl;
    return 0;
}