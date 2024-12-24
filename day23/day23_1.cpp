/*
need to find no of connected components of 3nodes where atleast one of the nodes' value starts with t
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <set>
using namespace std;

int main()
{
    string line;
    unordered_map<string, vector<string>> comps; // graph of comp connecs
    while (getline(cin, line))
    {
        int dashInd = line.find('-');
        string comp1 = line.substr(0, dashInd);
        string comp2 = line.substr(dashInd + 1, line.length() - dashInd);
        comps[comp1].push_back(comp2);
        comps[comp2].push_back(comp1);
    }

    set<vector<string>> connecComps;

    for (auto &[comp, compChildren] : comps)
    {
        bool tFound = false;
        for (auto compChild : compChildren)
        {
            for (auto lastChild : comps[compChild])
            {
                // check if there's a path from lastChild to comp and tFound = true for all related nodes
                if (find(comps[lastChild].begin(), comps[lastChild].end(), comp) != comps[lastChild].end())
                {
                    if (comp[0] == 't')
                        tFound = true;
                    if (compChild[0] == 't')
                        tFound = true;
                    if (lastChild[0] == 't')
                        tFound = true;

                    if (tFound)
                    {
                        vector<string> toPush = {comp, compChild, lastChild};
                        sort(toPush.begin(), toPush.end());
                        connecComps.insert(toPush);
                    }

                    tFound = false;
                }
            }
        }
    }

    cout << "Connected components : " << connecComps.size() << endl;
    for (auto cC : connecComps)
    {
        for (auto ele : cC)
        {
            cout << ele << " ";
        }
        cout << endl;
    }

    return 0;
}