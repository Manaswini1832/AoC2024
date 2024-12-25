#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool fitCheck(vector<int> lock, vector<int> key)
{
    for (int i = 0; i < lock.size(); i++)
    {
        if (lock[i] + key[i] + 2 > 7)
            return false;
    }
    return true;
}

int main()
{
    string line;
    vector<int> block(5, -1);
    vector<vector<int>> locks;
    vector<vector<int>> keys;

    bool lock = false, key = false;

    while (getline(cin, line))
    {
        if (line.empty())
        {
            if (!block.empty())
            {
                if (lock)
                    locks.push_back(block);
                else
                    keys.push_back(block);
                for (int i = 0; i < block.size(); i++)
                {
                    block[i] = -1;
                }
                lock = false;
                key = false;
            }
        }
        else
        {
            if (!key && !lock)
            {
                if (line[0] == '#')
                    lock = true;
                if (line[0] == '.')
                    key = true;
            }

            for (int i = 0; i < line.length(); i++)
            {
                if (line[i] == '#')
                    block[i] += 1;
            }
        }
    }

    if (!block.empty())
    {
        if (lock)
            locks.push_back(block);
        else
            keys.push_back(block);
    }

    int result = 0;
    for (auto l : locks)
    {
        for (auto k : keys)
        {
            if (fitCheck(l, k))
                result++;
        }
    }

    cout << "Final result : " << result << endl;

    return 0;
}
