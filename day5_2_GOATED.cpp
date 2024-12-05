/******************************************************************************
AoC day 5.2
Lol the hashmap h2 actually HELPED xD
Edits made compared to 5.2_working.cpp : 
1) changed up the hashmap h1 to a simple vector since I thought the former was over-complicating things and this rep is actually mucchhh better
2) h2 is still the same though I think if it were mapping ints to hashmaps, that would've been great but it's fine. this
one works too but it sacrifices on space complexity of storing rules. I assumed that each int maps to a 99length array storing rules
3) Hint seen on subreddit : too cleverT_T : to find the midpoint of an invalid order, you just have to find that element in the array
which acc to the rules will have an equal no of elements(from the invalidOrder array) to its and left and to its right(very defn of midpoint)
*******************************************************************************/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <regex>
#include <string>
using namespace std;

int getMidOfCorrectUpdates(vector<int>& update, unordered_map<int, vector<int>>& h2) {
    int numRulesBroken = 0;
    
    for(int i = 0; i < update.size(); i++){
        int num1 = update[i];
        for(int j = i+1; j < update.size(); j++){
            int num2 = update[j];
            if(h2[num1][num2] == -1) continue;
            numRulesBroken++;
            break;
        }
    }
    
    if(numRulesBroken == 0) return 0;
    
    for(int i = 0; i < update.size(); i++){
        int num1 = update[i];
        int left = 0, right = 0;
        for(int j = 0; j < update.size(); j++){
            if(i == j) continue;
            int num2 = update[j];
            if(h2[num1][num2] == -1) right++;
            else if(h2[num1][num2] == 1) left++;
        }
        if(left == right && left == (update.size()-1)/2) return update[i];
        left = 0, right = 0;
    }
    
    return -1;
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

            if (h2.find(num1) == h2.end()) h2[num1] = vector<int>(100, 0);
            if (h2.find(num2) == h2.end()) h2[num2] = vector<int>(100, 0);
            
            h2[num1][num2] = -1;  // num1 before num2
            h2[num2][num1] = 1;   // num2 after num1
        } else {
            // Read updates
            size_t start = 0, end;
            vector<int> update;
            
            while ((end = line.find(',', start)) != string::npos) {
                int num = stoi(line.substr(start, end - start));
                update.push_back(num);
                start = end + 1;
            }
            
            int num = stoi(line.substr(start, end - start));
            update.push_back(num);
            
            result += getMidOfCorrectUpdates(update, h2);
        }
    }

    cout << "Final sum result = " << result << endl;
    return 0;
}
