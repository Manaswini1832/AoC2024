#include <iostream>
#include <string>
#include <vector>
using namespace std;

long long mix(long long num, long long secretnum)
{
    num = num ^ secretnum;
    return num;
}

long long prune(long long num)
{
    num = num % 16777216;
    return num;
}

long long secretNum(long long num)
{
    long long temp = num;

    temp *= 64;
    temp = mix(temp, num);
    temp = prune(temp);

    num = temp;
    temp /= 32;
    temp = mix(temp, num);
    temp = prune(temp);

    num = temp;
    temp *= 2048;
    temp = mix(temp, num);
    temp = prune(temp);

    return temp;
}

int main()
{
    vector<int> initNums;
    string line;
    while (getline(cin, line))
    {
        string numStr;
        for (auto c : line)
            numStr += c;
        initNums.push_back(stoi(numStr));
    }

    long long result = 0;
    int maxSecNums = 10;

    for (auto num : initNums)
    {
        for (long long i = 0; i < maxSecNums; i++)
        {
            num = secretNum(num);
        }
        result += num;
    }

    cout << "Final result : " << result << endl;

    return 0;
}