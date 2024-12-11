#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

void trimLeadingZeros(string &num){
    while (!num.empty() && num[0] == '0') {
        num.erase(0, 1);
    }
    if (num.empty()) num = "0";
}

vector<string> getTwoStones(string stone) {
    long long mid = stone.length() / 2;
    string num1 = stone.substr(0, mid);
    string num2 = stone.substr(mid);
    
    trimLeadingZeros(num1);
    trimLeadingZeros(num2);
    
    return {num1, num2};
}

long long blink(vector<string> stone, int depth, unordered_map<string, vector<string>> &transformn, int neededDepth) {
    if (depth == neededDepth) return stone.size();
    
    long long result = 0;
    vector<string> nextStones;
    
    for (const string &backStone : stone) {
        if (transformn.find(backStone) == transformn.end()) {
            vector<string> backStoneTransformns;
            
            if (backStone.length() % 2 == 1) {
                long long num = stoll(backStone) * 2024;
                backStoneTransformns.push_back(to_string(num));
            } else { // even digits
                vector<string> twoStones = getTwoStones(backStone);
                backStoneTransformns.insert(backStoneTransformns.end(), twoStones.begin(), twoStones.end());
            }
            transformn[backStone] = backStoneTransformns;
        }
        nextStones.insert(nextStones.end(), transformn[backStone].begin(), transformn[backStone].end());
    }
    result += blink(nextStones, depth + 1, transformn, neededDepth);
    return result;
}

int main() {
    string line;
    vector<string> stones;
    while (getline(cin, line)) {
        stringstream ss(line);
        string numstr;
        while (ss >> numstr) {
            stones.push_back(numstr);
        }
    }
    
    unordered_map<string, vector<string>> transformn;
    transformn["0"].push_back("1");
    
    long long totalStones = 0;
    for (auto stone : stones) {
        vector<string> tempStone = {stone};
        long long numStones = blink(tempStone, 0, transformn, 75);
        totalStones += numStones;
    }
    
    cout << "Total stones: " << totalStones << endl;
    return 0;
}
