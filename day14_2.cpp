#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <fstream>

using namespace std;

class Robot
{
    int px, py, vx, vy;

public:
    Robot(int px, int py, int vx, int vy) : px(px), py(py), vx(vx), vy(vy) {}

    void move(int gridWidth, int gridHeight)
    {
        px = (px + vx + gridWidth) % gridWidth;
        py = (py + vy + gridHeight) % gridHeight;
    }

    pair<int, int> viewPos()
    {
        return {px, py};
    }
};

void parseLine(string &line, int &px, int &py, int &vx, int &vy)
{
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

void saveAsBMP(vector<vector<char>> &grid, int secondsPassed)
{
    int height = grid.size();
    int width = grid[0].size();

    string fileName = "grid_" + to_string(secondsPassed) + ".bmp";

    unsigned char bmpFileHeader[14] = {
        'B', 'M',   // Signature
        0, 0, 0, 0, // File size in bytes (placeholder)
        0, 0, 0, 0, // Reserved
        54, 0, 0, 0 // Pixel data offset
    };

    unsigned char bmpInfoHeader[40] = {
        40, 0, 0, 0, // DIB header size
        0, 0, 0, 0,  // Width (placeholder)
        0, 0, 0, 0,  // Height (placeholder)
        1, 0,        // Planes
        24, 0,       // Bits per pixel
        0, 0, 0, 0,  // Compression
        0, 0, 0, 0,  // Image size (no compression)
        0, 0, 0, 0,  // X pixels per meter
        0, 0, 0, 0,  // Y pixels per meter
        0, 0, 0, 0,  // Colors in color table
        0, 0, 0, 0   // Important color count
    };

    int rowSize = (3 * width + 3) & (~3);
    int fileSize = 54 + rowSize * height;

    bmpFileHeader[2] = fileSize;
    bmpFileHeader[3] = fileSize >> 8;
    bmpFileHeader[4] = fileSize >> 16;
    bmpFileHeader[5] = fileSize >> 24;

    bmpInfoHeader[4] = width;
    bmpInfoHeader[5] = width >> 8;
    bmpInfoHeader[6] = width >> 16;
    bmpInfoHeader[7] = width >> 24;

    bmpInfoHeader[8] = height;
    bmpInfoHeader[9] = height >> 8;
    bmpInfoHeader[10] = height >> 16;
    bmpInfoHeader[11] = height >> 24;

    ofstream outFile(fileName, ios::out | ios::binary);

    outFile.write(reinterpret_cast<char *>(bmpFileHeader), 14);
    outFile.write(reinterpret_cast<char *>(bmpInfoHeader), 40);

    for (int r = height - 1; r >= 0; --r)
    {
        for (int c = 0; c < width; ++c)
        {
            unsigned char color = (grid[r][c] == '#') ? 0 : 255;
            outFile.put(color);
            outFile.put(color);
            outFile.put(color);
        }
        for (int p = 0; p < rowSize - 3 * width; ++p)
        {
            outFile.put(0);
        }
    }

    outFile.close();
}

int main()
{
    int gridWidth = 101, gridHeight = 103, secondsPassed = 1;

    string line;
    vector<Robot> robots;

    while (getline(cin, line))
    {
        if (line.empty())
            break;

        int px, py, vx, vy;
        parseLine(line, px, py, vx, vy);
        Robot robot = Robot(px, py, vx, vy);
        robots.push_back(robot);
    }

    while (secondsPassed < 9000)
    {
        vector<vector<char>> grid(gridHeight, vector<char>(gridWidth, '.'));
        for (auto &robot : robots)
        {
            robot.move(gridWidth, gridHeight);

            auto robotPos = robot.viewPos();
            int px = robotPos.first;
            int py = robotPos.second;

            grid[py][px] = '#';
        }

        cout << "Seconds : " << secondsPassed << endl;

        saveAsBMP(grid, secondsPassed);

        secondsPassed++;
    }

    return 0;
}
