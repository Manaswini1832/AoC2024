/*ONLY WORKS FOR TEST INPUTt_t*/


/******************************************************************************
AoC day 5.2

twoPtrResolution function
    - Takes in h1, h2, ruleBroken
        - Uses a two pointer approach to satisfy the rule by moving around indices in h1
        
resolveRulesAndGiveSwaps
    - input : rulesBrokenCopy, h1, h2
    - output : swaps when we start from a specific rule
    - now until rulesBrokenCopy set becomes empty
        - resolve a rule using twoPtrResolution
        - erase rule from rulesBrokenCopy
        - check if any rules in rulesBrokenCopy can be eliminated due to this swap. if yes, eliminate them
        - count swaps taken

fixBrokenRules function
    - Takes in rulesBroken, h1, h2
    - int minSwaps, middleElement;
    - For each broken rule(chosenRule)
        - make a copy of rulesBroken into rulesBrokenCopy
        - make a copy of h1 into h1Copy
        - resolve the chosenRule using twoPtrResolution
        - erase chosenRule from rulesBrokenCopy
        - check if any rules in rulesBrokenCopy can be eliminated due to this swap. if yes, eliminate them
        - count swaps taken
        - int swaps = resolveRulesAndGiveSwaps with rulesBrokenCopy, h1, h2
        - if swaps < minSwaps, swaps = minSwaps, middleElement = h1Copy[(totalElemsInUpdate-1)/2] 
        
*******************************************************************************/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <regex>
#include <string>
#include <set>
#include <climits>
using namespace std;

int twoPtrResolution(unordered_map<int, int>& h1, unordered_map<int, vector<int>>& h2, vector<int>& ruleBroken) {
    int num1 = ruleBroken[0];
    int num2 = ruleBroken[1];
    
    int pos1 = h1[num1];  // position of num1
    int pos2 = h1[num2];  // position of num2
    int swaps = 0;

    if (h2[num1][num2] == -1 && pos1 > pos2) {
        while(pos1 > pos2){
            if (pos1 - pos2 <= 2) {
                h1[num1] = pos2;
                h1[num2] = pos1;
                swaps++;
            } else {
                //move num2 from pos2 to pos2+1
                //move num1 from pos1 to pos1-1
                int numAtPos2Plus1 = -1, numAtPos1Minus1 = -1;
                for(auto [num, index]: h1){
                    if(numAtPos2Plus1 != -1 &&numAtPos1Minus1 != -1) break;
                    if(index == pos2+1) numAtPos2Plus1 = num;
                    if(index == pos1-1) numAtPos1Minus1 = num;
                }
                
                int temp = h1[numAtPos2Plus1];
                h1[numAtPos2Plus1] = h1[num2];
                h1[num2] = temp;
                
                temp = h1[numAtPos1Minus1];
                h1[numAtPos1Minus1] = h1[num1];
                h1[num1] = temp;
                
                swaps++;
            }
            pos1 = h1[num1];
            pos2 = h1[num2];
        }
    } else if (h2[num1][num2] == 1 && pos1 < pos2) {
        while(pos1 < pos2){
            if (pos2 - pos1 <= 2) {
                h1[num1] = pos2;
                h1[num2] = pos1;
                swaps++;
            } else {
                //move num2 from pos2 to pos2-1
                //move num1 from pos1 to pos1+1
                int numAtPos2Minus1 = -1, numAtPos1Plus1 = -1;
                for(auto [num, index]: h1){
                    if(numAtPos2Minus1 != -1 &&numAtPos1Plus1 != -1) break;
                    if(index == pos2-1) numAtPos2Minus1 = num;
                    if(index == pos1+1) numAtPos1Plus1 = num;
                }
                
                int temp = h1[numAtPos2Minus1];
                h1[numAtPos2Minus1] = h1[num2];
                h1[num2] = temp;
                
                temp = h1[numAtPos1Plus1];
                h1[numAtPos1Plus1] = h1[num1];
                h1[num1] = temp;
                
                swaps++;
            }
            pos1 = h1[num1];
            pos2 = h1[num2];
        }
    }

    return swaps;
}



int resolveRulesAndGiveSwaps(set<vector<int>>& rulesBrokenCopy, unordered_map<int, int>& h1, unordered_map<int, vector<int>>& h2) {
    int totalSwaps = 0;

    while (!rulesBrokenCopy.empty()) {
        vector<int> ruleBroken = *rulesBrokenCopy.begin();
        rulesBrokenCopy.erase(rulesBrokenCopy.begin());

        // Resolve the rule using twoPtrResolution
        int swaps = twoPtrResolution(h1, h2, ruleBroken);
        totalSwaps += swaps;

        // Check if resolving this rule eliminates any other broken rules
        auto it = rulesBrokenCopy.begin();
        while (it != rulesBrokenCopy.end()) {
            int num1 = (*it)[0];
            int num2 = (*it)[1];

            if (h2[num1][num2] == -1 && h1[num1] <= h1[num2]) {
                it = rulesBrokenCopy.erase(it);  // Eliminate rule
            } else if (h2[num1][num2] == 1 && h1[num1] > h1[num2]) {
                it = rulesBrokenCopy.erase(it);  // Eliminate rule
            } else {
                ++it;  // Keep the rule
            }
        }
    }

    return totalSwaps;
}


int fixBrokenRules(set<vector<int>>& rulesBroken, unordered_map<int, int>& h1, unordered_map<int, vector<int>>& h2, int totalElemsInUpdate) {
    int minSwaps = INT_MAX;
    int middleElement = -1;

    for (auto chosenRule : rulesBroken) {
        // Make copies of h1 and rulesBroken for this chosen rule
        unordered_map<int, int> h1Copy = h1;  // Copy of the original state of h1
        set<vector<int>> rulesBrokenCopy = rulesBroken;  // Copy of rulesBroken

        // Resolve the chosen rule first
        int initialSwaps = twoPtrResolution(h1Copy, h2, chosenRule);
        
        //cout << "After resolving chosenRule " << chosenRule[0] << ", " << chosenRule[1] << endl;
        for (auto [num, index] : h1Copy) {
            //cout << num << " : " << index << "\n";
        }

        rulesBrokenCopy.erase(chosenRule);  // Remove the rule from the copy

        // Check if any rules in rulesBrokenCopy are satisfied after this swap
        auto it = rulesBrokenCopy.begin();
        while (it != rulesBrokenCopy.end()) {
            int num1 = (*it)[0];
            int num2 = (*it)[1];

            if (h2[num1][num2] == -1 && h1Copy[num1] <= h1Copy[num2]) {
                it = rulesBrokenCopy.erase(it);  // Eliminate rule
            } else if (h2[num1][num2] == 1 && h1Copy[num1] > h1Copy[num2]) {
                it = rulesBrokenCopy.erase(it);  // Eliminate rule
            } else {
                ++it;  // Keep the rule
            }
        }

        // Resolve the remaining rules and count the swaps
        int swaps = initialSwaps + resolveRulesAndGiveSwaps(rulesBrokenCopy, h1Copy, h2);

        // Check if this path required fewer swaps
        if (swaps < minSwaps) {
            minSwaps = swaps;
            for(auto [num, index] : h1Copy){
                if(index == (totalElemsInUpdate - 1) / 2){
                    middleElement = num;
                    break;
                }
            }
            //cout << "min swaps with " << chosenRule[0] << ", " << chosenRule[1] << " mid = " << middleElement << endl;
        }else if(swaps == minSwaps){
            int tempMid;
            for(auto [num, index] : h1Copy){
                if(index == (totalElemsInUpdate - 1) / 2){
                    tempMid = num;
                    break;
                }
            }
            if(tempMid < middleElement) middleElement = tempMid;
            //cout << "min swaps with " << chosenRule[0] << ", " << chosenRule[1] << " mid = " << middleElement  << endl;
        }
    }
    //cout << "Middle element : " << middleElement << endl; 
    return middleElement;
}


int getMidOfCorrectUpdates(unordered_map<int, int>& h1, unordered_map<int, vector<int>>& h2, int totalElemsInUpdate) {
    //cout << "update : " << "\n";
    for(auto [num, index]: h1){
        //cout << num << " : " << index << "\n";
    }
    set<vector<int>> rulesBroken;
    for (auto& [num, index] : h1) {
        // num appears at index. So check if all the rules of num from h2 are satisfied
        
        vector<int> rules(100, 0);
        rules = h2[num];

        for (int i = 11; i <= 99; i++) {
            if (rules[i] == -1) {
                // num appears before i
                if (h1.find(i) != h1.end() && h1[num] > h1[i]){
                    if(num > i) rulesBroken.insert({i, num});
                    else rulesBroken.insert({num, i});
                }
            } else if (rules[i] == 1) {
                // num appears after i
                if (h1.find(i) != h1.end() && h1[num] <= h1[i]){
                    if(num > i) rulesBroken.insert({i, num});
                    else rulesBroken.insert({num, i});
                }
            }
        }
    }
    if(rulesBroken.size() == 0) return 0;
    //cout << "Calling fixBrokenRules" << endl;
    return fixBrokenRules(rulesBroken, h1, h2, totalElemsInUpdate);
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
            
            result += getMidOfCorrectUpdates(h1, h2, idx);
        }
    }

    cout << "Final sum result = " << result << endl;
    return 0;
}
