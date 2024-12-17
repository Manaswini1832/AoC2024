/*
Doing a full 180 for this part. There's no base case termination condition like in pt1 where we were guaranteed < 100 clicks
Here there could be any no of clicks so recursing for those many clicks and checking each possibility
will lead to a hugeeeee time complexity

Simpler solution is just to solve the system of linear equations
Clue from subreddit : Cramer's rule to solve a system of linear equations
AX = B is a system of lin eqs => x = Dx/D and y = Dy/D where
D = det(A), Dx = det(Ax), Dy = det(Ay)
where Ax = A with first col replaced w B and Ay = A with second col replaced with B
*/

typedef long long ll;

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <climits>
using namespace std;

class ClawMachine {
    ll ax, ay, bx, by, prize_x, prize_y;
    ll atokens = 3, btokens = 1;

public:
    ClawMachine(ll ax, ll ay, ll bx, ll by, ll prize_x, ll prize_y) :
        ax(ax), ay(ay), bx(bx), by(by), prize_x(prize_x), prize_y(prize_y) {}

    void printValues() {
        cout << "Button A: (" << ax << ", " << ay << ")\n";
        cout << "Button B: (" << bx << ", " << by << ")\n";
        cout << "Prize: (" << prize_x << ", " << prize_y << ")\n";
    }

    ll getMinTokens(){
        //solve the system of lin eqs using Cramer's rule
        ll detA = ax*by - ay*bx;
        ll detAx = prize_x*by - prize_y*bx;
        ll detAy = ax*prize_y - ay*prize_x;
        if(detA == 0 || detAx % detA != 0 || detAy % detA != 0) return 0;
        ll x = detAx / detA;
        ll y = detAy / detA;
        // if(x > 100 || y > 100) return 0; //this should be uncommented for pt1
        return 3*x + y;
    }
};

void parseButtonLine(const string& line, ll& x, ll& y) {
    int xpos = line.find("X+");
    int ypos = line.find("Y+");
    x = stoi(line.substr(xpos + 2, line.find(",", xpos) - xpos - 2));
    y = stoi(line.substr(ypos + 2));
}

void parsePrizeLine(const string& line, ll& x, ll& y) {
    int xpos = line.find("X=");
    int ypos = line.find("Y=");
    x = stoi(line.substr(xpos + 2, line.find(",", xpos) - xpos - 2));
    y = stoi(line.substr(ypos + 2));
}

int main() {
    string line;
    vector<ClawMachine> clawMachines;
    
    ll errorNum = 10000000000000;
    
    while (getline(cin, line)) {
        if (line.empty()) break;
        
        ll ax, ay, bx, by, prize_x, prize_y;

        parseButtonLine(line, ax, ay);
        
        getline(cin, line);
        parseButtonLine(line, bx, by);
        
        getline(cin, line);
        parsePrizeLine(line, prize_x, prize_y);
        
        prize_x += errorNum;
        prize_y += errorNum;
        
        ClawMachine machine = ClawMachine(ax, ay, bx, by, prize_x, prize_y);
        clawMachines.push_back(machine);
        
        cin.ignore();
    }
    
    ll result = 0;
    for(auto machine: clawMachines){
        ll minTokens = machine.getMinTokens();
        result += minTokens;
        cout << "Minimum tokens: " << minTokens << "\n";
    }
    
    cout << "Final result : " << result << endl;
    
    return 0;
}
