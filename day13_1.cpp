#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <climits>
using namespace std;

class ClawMachine {
    int ax, ay, bx, by, prize_x, prize_y;
    int atokens = 3, btokens = 1;

public:
    ClawMachine(int ax, int ay, int bx, int by, int prize_x, int prize_y) :
        ax(ax), ay(ay), bx(bx), by(by), prize_x(prize_x), prize_y(prize_y) {}

    void printValues() {
        cout << "Button A: (" << ax << ", " << ay << ")\n";
        cout << "Button B: (" << bx << ", " << by << ")\n";
        cout << "Prize: (" << prize_x << ", " << prize_y << ")\n";
    }

    string getHash(int x, int y) {
        return to_string(x) + "|" + to_string(y);
    }

    int Backtrack(int aClicks, int bClicks, unordered_map<string, int>& minToksToReachHere, int x, int y) {
        if (x == prize_x && y == prize_y) {
            //prize reached
            return 0;
        }
        if (aClicks == 100 || bClicks == 100 || x > prize_x || y > prize_y) {
            //max clicks reached
            return INT_MAX;
        }

        string key = getHash(x, y);
        if (minToksToReachHere.find(key) != minToksToReachHere.end())
            return minToksToReachHere[key];

        int tokensIfA = Backtrack(aClicks + 1, bClicks, minToksToReachHere, x + ax, y + ay);
        if (tokensIfA != INT_MAX) tokensIfA += atokens;
        
        int tokensIfB = Backtrack(aClicks, bClicks + 1, minToksToReachHere, x + bx, y + by);
        if (tokensIfB != INT_MAX) tokensIfB += btokens;

        minToksToReachHere[key] = min(tokensIfA, tokensIfB);
        return minToksToReachHere[key];
    }

    int getMinTokens() {
        int aClicks = 0, bClicks = 0;
        unordered_map<string, int> minToksToReachHere;
        int minToks = Backtrack(aClicks, bClicks, minToksToReachHere, 0, 0);
        return minToks == INT_MAX ? 0 : minToks;
    }
};

void parseButtonLine(const string& line, int& x, int& y) {
    size_t xpos = line.find("X+");
    size_t ypos = line.find("Y+");
    x = stoi(line.substr(xpos + 2, line.find(",", xpos) - xpos - 2));
    y = stoi(line.substr(ypos + 2));
}

void parsePrizeLine(const string& line, int& x, int& y) {
    size_t xpos = line.find("X=");
    size_t ypos = line.find("Y=");
    x = stoi(line.substr(xpos + 2, line.find(",", xpos) - xpos - 2));
    y = stoi(line.substr(ypos + 2));
}

int main() {
    string line;
    vector<ClawMachine> clawMachines;
    
    while (getline(cin, line)) {
        if (line.empty()) break;
        
        int ax, ay, bx, by, prize_x, prize_y;

        parseButtonLine(line, ax, ay);
        
        getline(cin, line);
        parseButtonLine(line, bx, by);
        
        getline(cin, line);
        parsePrizeLine(line, prize_x, prize_y);
        
        ClawMachine machine = ClawMachine(ax, ay, bx, by, prize_x, prize_y);
        clawMachines.push_back(machine);
    
        cin.ignore();
    }
    
    int result = 0;
    for(auto machine: clawMachines){
        int minTokens = machine.getMinTokens();
        result += minTokens;
        cout << "Minimum tokens: " << minTokens << "\n";
    }
    
    cout << "Final result : " << result << endl;
    
    return 0;
}
