#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

int canBeFormed(unordered_map<string, bool> &present, string design)
{
    // dp[i] = true if first i letters of design word can be formed by strings in present
    vector<bool> dp(design.length() + 1, false);
    dp[0] = true;

    for (int i = 1; i <= design.length(); i++)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            if (dp[j])
            {
                if (present.find(design.substr(j, i - j)) != present.end())
                {
                    dp[i] = true;
                    break;
                }
            }
        }
    }

    return dp[design.length()] == false ? 0 : 1;
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

    int result = 0;
    for (auto design : designs)
        result += canBeFormed(present, design);
    cout << "Final result : " << result << endl;
    return 0;
}