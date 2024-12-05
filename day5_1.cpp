/******************************************************************************
AoC day 5.1

Maintain two hashmaps : 
    one to store where(indices) the pagenums occur in the current update
    two to store the rules
There are only two digit numbers(max=99) in the puzzle input so I'll store the hashmap2 in such a way that
    if hashmap2[i][j] = -1, i should occur before j
    if hashmap2[i][j] = 1, i should occur after j
    if hashmap2[i][j] = 0, j isn't in any rule associated with i
    
hashmap1{
75:0
47:1
61:2
53:3
29:4
}

hashmap2{ //<int, vector<int>>
47 : {before 53, .....}
53 : after 47
97 : before 13
13 : after 97
97 : before 61
61 : after 97
75 : before 53
53 : after 75
...
}
*******************************************************************************/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <regex>
#include <string>
using namespace std;

bool isValidUpdate(unordered_map<int, int>& h1, unordered_map<int, vector<int>>& h2) {
    // cout << "update : " << "\n";
    // for(auto [num, index]: h1){
    //     cout << num << " : " << index << "\n";
    // }
    
    for (auto& [num, index] : h1) {
        // num appears at index. So check if all the rules of num from h2 are satisfied
        
        vector<int> rules(100, 0);
        rules = h2[num];

        for (int i = 11; i <= 99; i++) {
            if (rules[i] == -1) {
                // num appears before i
                if (h1.find(i) != h1.end() && h1[num] > h1[i]) return false;
            } else if (rules[i] == 1) {
                // num appears after i
                if (h1.find(i) != h1.end() && h1[num] <= h1[i]) return false;
            }
        }
    }
    return true;
}

int main() {
    int result = 0;

    unordered_map<int, vector<int>> h2; //rules

    bool newLineSeen = false;
    string line;
    while (getline(cin, line)) {
        if (line.empty() && !newLineSeen) {
            newLineSeen = true;
            continue;
        }
        if (!newLineSeen) {
            // Read rules
            regex ruleStringPattern(R"((\d+)\|(\d+))");
            smatch match;
            regex_search(line, match, ruleStringPattern);
            int num1 = stoi(match[1].str());
            int num2 = stoi(match[2].str());
            // cout << "num1 = " << num1 << ", num2 = " << num2 << "\n";

            if (h2.find(num1) == h2.end()) h2[num1] = vector<int>(100, 0);
            if (h2.find(num2) == h2.end()) h2[num2] = vector<int>(100, 0);
            
            h2[num1][num2] = -1;  // num1 before num2
            h2[num2][num1] = 1;   // num2 after num1
        } else {
            // Read updates
            //cout << "Input line : " << line << endl;
            unordered_map<int, int> h1; //updates
            size_t start = 0, end;
            int idx = 0;
            vector<int> updates;
            
            while ((end = line.find(',', start)) != string::npos) {
                int num = stoi(line.substr(start, end - start));
                updates.push_back(num);
                h1[num] = idx;
                idx++;
                start = end + 1;
            }
            
            int num = stoi(line.substr(start, end - start));
            updates.push_back(num);
            h1[num] = idx;
            idx++;
            
            if (isValidUpdate(h1, h2)) {
                cout << "adding to sum : " << updates[(idx - 1) / 2] << endl; 
                result += updates[(idx - 1) / 2];
            }
        }
    }

    cout << "Final sum result = " << result << endl;
    return 0;
}
