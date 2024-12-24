#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
using namespace std;

void findCliques(set<string> &currentClique, vector<string> &candidates,
                 unordered_map<string, vector<string>> &graph, set<string> &maxClique)
{

    if (candidates.empty())
    {
        if (currentClique.size() > maxClique.size())
        {
            maxClique = currentClique;
        }
        return;
    }

    for (int i = 0; i < candidates.size(); ++i)
    {
        string node = candidates[i];
        set<string> newClique = currentClique;
        newClique.insert(node);

        vector<string> newCandidates;
        for (int j = i + 1; j < candidates.size(); ++j)
        {
            if (find(graph[node].begin(), graph[node].end(), candidates[j]) != graph[node].end())
            {
                newCandidates.push_back(candidates[j]);
            }
        }

        findCliques(newClique, newCandidates, graph, maxClique);
    }
}

int main()
{
    string line;
    unordered_map<string, vector<string>> comps;

    while (getline(cin, line))
    {
        int dashInd = line.find('-');
        string node1 = line.substr(0, dashInd);
        string node2 = line.substr(dashInd + 1);
        comps[node1].push_back(node2);
        comps[node2].push_back(node1);
    }

    vector<string> allNodesVec;
    for (auto [node, _] : comps)
        allNodesVec.push_back(node);

    set<string> sol = {};
    set<string> maxClique;
    findCliques(sol, allNodesVec, comps, maxClique); // recur dfs to find maxClique

    string password;
    for (auto ele : maxClique)
    {
        password += ele;
        password += ',';
    }
    password.pop_back();

    cout << "Password: " << password << endl;

    return 0;
}