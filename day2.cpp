#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
using namespace std;

bool isSafe(vector<int> nums){
    for(int i = 0; i < nums.size(); i++){
        int removNum = nums[i];
        stack<int> mxSt; //max should be on top
        stack<int> mnSt; //min should be on top
        for(int j = 0; j < nums.size(); j++){
            if(i == j) continue;
            int num = nums[j];
            if(mxSt.size() == 0){
                mxSt.push(num);
                if(mnSt.size() == 0) mnSt.push(num);
                continue;
            }
            if(num >= mxSt.top()) {
                int diff = abs(num - mxSt.top());
                if(diff > 0 && diff <= 3)
                    mxSt.push(num);
            }
            if(num <= mnSt.top()){
                int diff = abs(num - mnSt.top());
                if(diff > 0 && diff <= 3)
                    mnSt.push(num);
            }
        }
        if(mxSt.size() == nums.size()-1 || mnSt.size() == nums.size()-1){
            return true;
        }
    }
    return false;
}

int main()
{
    string line;
    int result = 0;
    while(getline(cin, line)){
        vector<int> nums;
        stack<int> mxSt; //max should be on top
        stack<int> mnSt; //min should be on top
        stringstream ss(line);
        string nS;
        int total = 0;
        while(ss >> nS){
            int num = stoi(nS);
            if(mxSt.size() == 0){
                mxSt.push(num);
                if(mnSt.size() == 0) mnSt.push(num);
                nums.push_back(num);
                total++;
                continue;
            }
            if(num >= mxSt.top()) {
                int diff = abs(num - mxSt.top());
                if(diff > 0 && diff <= 3)
                    mxSt.push(num);
            }
            if(num <= mnSt.top()){
                int diff = abs(num - mnSt.top());
                if(diff > 0 && diff <= 3)
                    mnSt.push(num);
            }
            nums.push_back(num);
            total++;
        }
        if(mxSt.size() == total || mnSt.size() == total){
            result++;
            continue;
        }
        bool safe = isSafe(nums);
        if(safe) result++;
    }
    
    cout << result << endl;

    return 0;
}
