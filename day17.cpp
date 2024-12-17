#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <set>
#include <algorithm>
using namespace std;

class Computer
{
    long long A, B, C;
    long long ip;
    vector<int> opcodes, operands;

    bool firstOutput;
    vector<int> fullOutput;

public:
    Computer(long long A, long long B, long long C) : A(A), B(B), C(C), ip(0), firstOutput(true) {}

    long long literal(long long operand)
    {
        return operand;
    }

    long long combo(long long operand)
    {
        switch (operand)
        {
        case 4:
            return A;
        case 5:
            return B;
        case 6:
            return C;
        default:
            return operand;
        }
    }

    void adv(long long operand)
    {
        long long numerator = A;
        long long denominator = pow(2, combo(operand));
        A = numerator / denominator;
    }

    void bxl(long long operand)
    {
        B ^= literal(operand);
    }

    void bst(long long operand)
    {
        B = combo(operand) % 8;
    }

    void jnz(long long operand)
    {
        if (A != 0)
        {
            ip = literal(operand) / 2;
        }
        else
        {
            ip++;
        }
    }

    void bxc(int)
    {
        B ^= C;
    }

    void out(long long operand)
    {
        long long output = combo(operand) % 8;
        fullOutput.push_back(output);
        // if (!firstOutput) {
        //     cout << ",";
        // }
        // // cout << "A = " << A << endl;
        // cout << output;
        // firstOutput = false;
        // for(auto o: fullOutput) cout << o << ", ";
        // cout << endl;
    }

    void bdv(long long operand)
    {
        long long numerator = A;
        long long denominator = pow(2, combo(operand));
        B = numerator / denominator;
    }

    void cdv(long long operand)
    {
        long long numerator = A;
        long long denominator = pow(2, combo(operand));
        C = numerator / denominator;
    }

    void runInstruction(long long opcode, long long operand)
    {
        switch (opcode)
        {
        case 0:
            adv(operand);
            break;
        case 1:
            bxl(operand);
            break;
        case 2:
            bst(operand);
            break;
        case 3:
            jnz(operand);
            return; // spl case: ip changed
        case 4:
            bxc(operand);
            break;
        case 5:
            out(operand);
            break;
        case 6:
            bdv(operand);
            break;
        case 7:
            cdv(operand);
            break;
        default:
            break;
        }
    }

    void parseProgram(const string &line)
    {
        cout << line << endl;
        int i = line.find(':') + 2;
        string numStr;
        long long found = 0;

        while (i < line.length())
        {
            if (line[i] != ',')
            {
                numStr += line[i];
            }
            else
            {
                if (found % 2 == 0)
                {
                    opcodes.push_back(stoi(numStr));
                }
                else
                {
                    operands.push_back(stoi(numStr));
                }
                numStr.clear();
                found++;
            }
            i++;
        }

        if (!numStr.empty())
        {
            operands.push_back(stoi(numStr));
        }
    }

    vector<int> run(string &program)
    {
        parseProgram(program);
        ip = 0;

        while (ip < opcodes.size())
        {
            long long currentOpcode = opcodes[ip];
            long long currentOperand = operands[ip];
            runInstruction(currentOpcode, currentOperand);

            if (currentOpcode != 3)
            {
                ip++;
            }
        }
        return fullOutput;
    }

    long long reconstructA()
    {
        vector<int> pgm = {2, 4, 1, 2, 7, 5, 1, 7, 4, 4, 0, 3, 5, 5, 3, 0};
        string program = "2,4,1,2,7,5,1,7,4,4,0,3,5,5,3,0";

        set<int> candidates;
        candidates.insert(0);

        for (long long it = pgm.size() - 1; it >= 0; it--)
        {
            set<int> new_candidates;

            for (long long candidate : candidates)
            {
                for (long long n = 0; n < 8; ++n)
                {
                    long long new_value = (candidate << 3) + n;
                    A = new_value;
                    fullOutput.clear();
                    firstOutput = true;
                    string subPgm = "";
                    for (int m = it; m < pgm.size(); m++)
                    {
                        if (subPgm.length() > 0)
                            subPgm += ',';
                        subPgm += pgm[m];
                    }

                    cout << "subpgm : " << subPgm << endl;
                    vector<int> pgmOut = run(subPgm);
                    long long num = 0;
                    cout << "Program out : " << endl;
                    for (auto po : pgmOut)
                        cout << po << ", " << endl;

                    for (long long i = it; i < pgm.size(); i++)
                    {
                        if (pgmOut[i - it] == pgm[i])
                        {
                            num++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    if (num == pgm.size() - it)
                        new_candidates.insert(new_value);
                }
            }

            candidates = new_candidates;
        }

        return *min_element(candidates.begin(), candidates.end());
    }

    void setA(long long a)
    {
        A = a;
    }
};

void parseRegs(const string &line, long long &A, long long &B, long long &C)
{
    size_t colon = line.find(':') + 2;
    string numStr = line.substr(colon);

    if (line.find("A") != string::npos)
    {
        A = stoi(numStr);
    }
    else if (line.find("B") != string::npos)
    {
        B = stoi(numStr);
    }
    else if (line.find("C") != string::npos)
    {
        C = stoi(numStr);
    }
}

int main()
{
    string line;
    long long A = 0, B = 0, C = 0;
    string program;

    while (getline(cin, line))
    {
        if (line.empty())
            continue;

        if (line.find("Register") != string::npos)
        {
            parseRegs(line, A, B, C);
        }
        else
        {
            program = line;
        }
    }

    Computer c(A, B, C);
    vector<int> outttt = c.run(program);
    for (auto o : outttt)
        cout << o << ", ";
    cout << "\n";
    long long estA = c.reconstructA();
    cout << "estA = " << estA << endl;

    cout << "Program ran with estimated A = " << estA << " : ";
    Computer c2(estA, 0, 0);
    c2.run(program);

    return 0;
}
