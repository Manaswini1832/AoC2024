#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;

class Robot {
    int px, py, vx, vy;
public:
    Robot(int px, int py, int vx, int vy) : px(px), py(py), vx(vx), vy(vy) {}

    void move(int gridWidth, int gridHeight) {
        px = (px + vx + gridWidth) % gridWidth;
        py = (py + vy + gridHeight) % gridHeight;
    }

    pair<int, int> viewPos() {
        return {px, py};
    }
};

void parseLine(const string& line, int& px, int& py, int& vx, int& vy) {
    int pPos = line.find("p=");
    int vPos = line.find("v=");

    string pSub = line.substr(pPos + 2, vPos - pPos - 3);
    int pComm = pSub.find(",");

    px = stoi(pSub.substr(0, pComm));
    py = stoi(pSub.substr(pComm + 1));

    string vSub = line.substr(vPos + 2);
    int vComm = vSub.find(",");

    vx = stoi(vSub.substr(0, vComm));
    vy = stoi(vSub.substr(vComm + 1));
}

int main() {
    const int gridWidth = 11, gridHeight = 7, secondsPassed = 100;
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;

    string line;
    vector<Robot> robots;

    while (getline(cin, line)) {
        if (line.empty()) break;

        int px, py, vx, vy;
        parseLine(line, px, py, vx, vy);
        Robot robot = Robot(px, py, vx, vy);
        robots.push_back(robot);
    }

    for (auto& robot : robots) {
        for (int i = 0; i < secondsPassed; ++i) {
            robot.move(gridWidth, gridHeight);
        }
    }

    for (auto& robot : robots) {
        auto robotPos = robot.viewPos();
        int px = robotPos.first;
        int py = robotPos.second;

        if (px != gridWidth / 2 && py != gridHeight / 2) {
            if (px < gridWidth / 2 && py < gridHeight / 2) {
                ++q1;
            } else if (px > gridWidth / 2 && py < gridHeight / 2) {
                ++q2;
            } else if (px > gridWidth / 2 && py > gridHeight / 2) {
                ++q3;
            } else if (px < gridWidth / 2 && py > gridHeight / 2) {
                ++q4;
            }
        }
    }

    int result = q1 * q2 * q3 * q4;
    cout << "Final result: " << result << endl;

    return 0;
}
