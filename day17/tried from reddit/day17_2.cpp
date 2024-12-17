/*
sol link : https://www.reddit.com/r/adventofcode/comments/1hg38ah/comment/m2gizvj/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button
*/

#include <iostream>
#include <vector>
#include <string>
#include <bitset>

std::vector<int> g{2, 4, 1, 2, 7, 5, 1, 7, 4, 4, 0, 3, 5, 5, 3, 0};

bool solve(int p, long r)
{
    if (p < 0)
    {
        std::cout << r << std::endl;
        return true;
    }
    for (int d = 0; d < 8; ++d)
    {
        long a = (r << 3) | d;
        int i = 0;
        long b = 0, c = 0, w = 0;

        while (i < g.size())
        {
            long o = 0;
            if (g[i + 1] <= 3)
                o = g[i + 1];
            else if (g[i + 1] == 4)
                o = a;
            else if (g[i + 1] == 5)
                o = b;
            else if (g[i + 1] == 6)
                o = c;

            if (g[i] == 0)
                a >>= o;
            else if (g[i] == 1)
                b ^= g[i + 1];
            else if (g[i] == 2)
                b = o & 7;
            else if (g[i] == 3)
                i = (a != 0) ? g[i + 1] - 2 : i;
            else if (g[i] == 4)
                b ^= c;
            else if (g[i] == 5)
            {
                w = o & 7;
                break;
            }
            else if (g[i] == 6)
                b = a >> o;
            else if (g[i] == 7)
                c = a >> o;

            i += 2;
        }

        if (w == g[p] && solve(p - 1, (r << 3) | d))
        {
            return true;
        }
    }
    return false;
}

int main()
{
    solve(g.size() - 1, 0);
    return 0;
}
