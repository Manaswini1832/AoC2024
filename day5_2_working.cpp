/******************************************************************************
AoC day 5.2
I think instead of a hashmap mapping nums to indices, I should've used a simple vector. That would've prevented me from over-thinking the solution for sooo long
Saw on subreddit : keep swapping elements until correct order is reached. Implemented that and it workedT_T
*******************************************************************************/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <regex>
#include <string>
#include <set>
#include <climits>
using namespace std;

int swapToRightPlaceGiveMid(vector<int>& invalidOrder, unordered_map<int, vector<int>>& h2) {
    bool swapped = false;

    // swapping until all rules are satisfied
    while (!swapped) {
        swapped = true;
        
        for (int i = 0; i < invalidOrder.size(); ++i) {
            for (int j = i + 1; j < invalidOrder.size(); ++j) {
                int num1 = invalidOrder[i];
                int num2 = invalidOrder[j];

                if (h2[num1][num2] == -1 && i > j) {
                    swap(invalidOrder[i], invalidOrder[j]);
                    swapped = false;
                } 
                else if (h2[num1][num2] == 1 && i < j) {
                    swap(invalidOrder[i], invalidOrder[j]);
                    swapped = false;
                }
            }
        }
    }

    int midIndex = invalidOrder.size() / 2;
    return invalidOrder[midIndex];
}

int getMidOfCorrectUpdates(unordered_map<int, int>& h1, unordered_map<int, vector<int>>& h2, int totalElemsInUpdate) {
    unordered_map<int, int> resMap;
    int numRulesBroken = 0;
    
    for (auto& [num, index] : h1) {
      
        vector<int> rules(100, 0);
        rules = h2[num];

        for (int i = 11; i <= 99; i++) {
            if (rules[i] == -1) {
                if (h1.find(i) != h1.end() && h1[num] > h1[i]){
                    numRulesBroken++;
                    break;
                }
            } else if (rules[i] == 1) {
                if (h1.find(i) != h1.end() && h1[num] <= h1[i]){
                    break;
                }
            }
        }
    }
    
    if(numRulesBroken == 0) return 0;
    //make the correct invalid order vector using h1
    //repeatedly swap each element until all rules are satisified
    //return middle
    vector<int> invalidOrder(totalElemsInUpdate);
    for(auto [num, index]: h1) invalidOrder[index] = num;
    return swapToRightPlaceGiveMid(invalidOrder, h2);
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
