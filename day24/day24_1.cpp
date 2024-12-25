#include <iostream>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
using namespace std;

void topologicalSort(vector<string> &topSortSeq, unordered_map<string, vector<string>> &indegs)
{
    if (indegs.empty())
        return;
    vector<unordered_map<string, vector<string>>::iterator> toErase;
    for (auto it = indegs.begin(); it != indegs.end(); it++)
    {
        string key = it->first;
        vector<string> connecNodes = it->second;
        if (connecNodes.size() == 0)
        {
            topSortSeq.push_back(key);
            for (auto &[childKey, childChildren] : indegs)
            {
                auto iter = find(childChildren.begin(), childChildren.end(), key);
                if (iter != childChildren.end())
                {
                    indegs[childKey].erase(iter);
                }
            }
            toErase.push_back(it);
        }
    }

    for (auto tE : toErase)
        indegs.erase(tE);

    topologicalSort(topSortSeq, indegs);
}

void process_input(unordered_map<string, int> &values, unordered_map<string, vector<string>> &indegs)
{
    string line;

    while (getline(cin, line) && !line.empty())
    {
        istringstream iss(line);
        string wire;
        int value;
        iss >> wire >> value;
        wire.pop_back();
        values[wire] = value;
        indegs[wire] = {};
    }
}

void process_gates(unordered_map<string, vector<string>> &indegs, unordered_map<string, tuple<int, string, string>> &op)
{
    string line;

    while (getline(cin, line))
    {
        istringstream iss(line);
        string input1, gate, input2, arrow, output;

        iss >> input1 >> gate >> input2 >> arrow >> output;
        indegs[output] = {input1, input2};

        if (gate == "AND")
        {
            op[output] = {0, input1, input2};
        }
        else if (gate == "OR")
        {
            op[output] = {1, input1, input2};
        }
        else if (gate == "XOR")
        {
            op[output] = {2, input1, input2};
        }
    }
}

int evaluate(tuple<int, string, string> operation, unordered_map<string, int> &values)
{
    int opCode = get<0>(operation);
    int input1 = values[get<1>(operation)];
    int input2 = values[get<2>(operation)];
    int result = -1;
    switch (opCode)
    {
    case 0: // and
        result = input1 & input2;
        break;
    case 1: // or
        result = input1 | input2;
        break;
    case 2: // xor
        result = input1 ^ input2;
        break;
    default:
        break;
    }
    return result;
}

int main()
{
    unordered_map<string, int> values;                    // wire name maps to its value
    unordered_map<string, vector<string>> indegs;         // wire name maps to  nodes it is dependent on
    unordered_map<string, tuple<int, string, string>> op; // wire name maps to {opcode, operand1, operand2} where opcode=0->and, 1->or, 2->xor

    process_input(values, indegs);

    process_gates(indegs, op);

    vector<string> topSortSeq;
    topologicalSort(topSortSeq, indegs);

    for (auto wire : topSortSeq)
    {
        if (values.find(wire) == values.end())
        {
            values[wire] = evaluate(op[wire], values);
        }
    }

    vector<pair<int, int>> zValues; //{sortValue based on z00 suffix which is 00 here, value of that wire}
    for (auto &[key, value] : values)
    {
        int zInd = key.find('z');
        if (zInd == string::npos)
            continue;
        if (key[zInd + 1] - '0' >= 0 && key[zInd + 1] - '0' <= 9 && key[zInd + 2] - '0' >= 0 && key[zInd + 2] - '0' <= 9)
        {
            int zval = stoi(key.substr(zInd + 1));
            zValues.push_back({zval, value});
        }
    }

    long long result = 0;
    sort(zValues.begin(), zValues.end());
    long long factor = 1;
    for (int i = 0; i < zValues.size(); i++)
    {
        result += (factor * zValues[i].second);
        factor *= 2;
    }
    cout << "Final result : " << result << endl;

    return 0;
}
