#include <iostream>
#include <stack>
#include <string>
using namespace std;

long long orOp(long long op1, long long op2){
    string result = to_string(op1) + to_string(op2);
    return stoll(result);
}

long long makeOp(long long res, stack<long long>& ops){
    if(ops.top() > res) return 0;
    if(ops.size() == 1){
        if(ops.top() == res) return res;
        return 0;
    }
    
    stack<long long> originalOps = ops;
    
    long long op1 = ops.top(); ops.pop();
    long long op2 = ops.top(); ops.pop();
    
    //choose +
    ops.push(op1 + op2);
    long long ires = makeOp(res, ops);
    if(ires != 0) return ires;
    ops = originalOps;
    
    //choose *
    op1 = ops.top(); ops.pop();
    op2 = ops.top(); ops.pop();
    ops.push(op1 * op2);
    ires = makeOp(res, ops);
    if(ires != 0) return ires;
    ops = originalOps;
    
    //choose ||
    op1 = ops.top(); ops.pop();
    op2 = ops.top(); ops.pop();
    long long orRes = orOp(op1, op2);
    ops.push(orRes);
    ires = makeOp(res, ops);
    if(ires != 0) return ires;
    
    return 0;
}

int main() {
    long long result = 0;
    string line;
    
    while (getline(cin, line)) {
        long long res;
        stack<long long> ops;
        
        string sanLine;
        for (char c : line) {
            if (c == ':') continue;
            sanLine += c;
        }

        int start = 0;
        int end = sanLine.find(' ');
        
        res = stoll(sanLine.substr(start, end - start));

        start = end;
        while ((end = sanLine.find(' ', start + 1)) != string::npos) {
            long long op = stoll(sanLine.substr(start + 1, end - start - 1));
            ops.push(op);
            start = end;
        }
        if (start + 1 < sanLine.size()) {
            long long op = stoll(sanLine.substr(start + 1));
            ops.push(op);
        }
        
        //nother stack to reverse elementsT_T
        stack<long long> opsSt;
        while (!ops.empty()) {
            opsSt.push(ops.top());
            ops.pop();
        }

        result += makeOp(res, opsSt); 
    }

    cout << "Final result: " << result << endl;

    return 0;
}
