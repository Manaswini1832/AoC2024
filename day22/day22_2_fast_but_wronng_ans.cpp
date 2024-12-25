// WORKS. To test for actual input

typedef long long ll;

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <climits>
#include <algorithm>
using namespace std;

string getHash(vector<ll> seq)
{
    string result = "";
    for (auto num : seq)
    {
        result += num;
        result += '|';
    }
    return result;
}

ll mix(ll num, ll secretnum)
{
    num = num ^ secretnum;
    return num;
}

ll prune(ll num)
{
    num = num % 16777216;
    return num;
}

ll secretNum(ll num)
{
    ll temp = num;

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

ll getLastDigit(ll num)
{
    return num % 10;
}

ll monkeyPrice(vector<ll> sequence, vector<ll> &prices, vector<ll> &diffs)
{
    ll l = 1, r = 4;
    ll maxPrice = 0;
    while (l <= diffs.size() - 4)
    {
        ll found = 0;
        ll ind = l;
        for (ll i = 0; i < 4; i++)
        {
            if (diffs[ind] != sequence[i])
                break;
            ind++;
            found++;
        }
        l++;
        r++;
        if (found < 4)
            continue;
        maxPrice = max(maxPrice, prices[ind - 1]);
    }
    return maxPrice;
}

int main()
{
    vector<ll> buyerInits;
    string line;
    while (getline(cin, line))
    {
        string numStr;
        for (auto c : line)
            numStr += c;
        buyerInits.push_back(stoi(numStr));
    }

    ll result = 0;
    ll maxSecNums = 2000;

    vector<vector<ll>> prices, diffs;
    unordered_map<ll, ll> secretNumCache;

    for (auto num : buyerInits)
    {
        vector<ll> pricesPart, diffsPart;
        for (ll i = 0; i < maxSecNums; i++)
        {
            ll price = getLastDigit(num);
            pricesPart.push_back(price);
            if (diffsPart.empty())
                diffsPart.push_back(0);
            else
                diffsPart.push_back(price - pricesPart[pricesPart.size() - 2]);

            if (secretNumCache.find(num) == secretNumCache.end())
            {
                ll secNum = secretNum(num);
                secretNumCache[num] = secNum;
                num = secNum;
            }
            else
            {
                num = secretNumCache[num];
            }
        }
        diffsPart[0] = LLONG_MIN;
        diffs.push_back(diffsPart);
        prices.push_back(pricesPart);
    }

    ll maxPrice = LLONG_MIN;
    unordered_map<string, long long> cache; // index+seq stored against no of max bananas

    for (ll i = 0; i < diffs.size(); i++)
    { // for each buyer
        vector<ll> buyerDiffs = diffs[i];
        cout << "at the " << i << "-th diff" << endl;
        unordered_map<string, bool> present;
        for (ll j = 1; j <= buyerDiffs.size() - 4; j++)
        { // make every possible 4-element sequence of diffs
            vector<ll> sequence;
            for (ll k = 0; k < 4; k++)
                sequence.push_back(buyerDiffs[k + j]);
            string hashKey = getHash(sequence);
            if (present.find(hashKey) != present.end())
                continue;
            present[hashKey] = true;
            cache[hashKey] += monkeyPrice(sequence, prices[i], diffs[i]); // cache the price obtained w each such sequence
        }
        present.clear();
    }

    for (auto &[_, totalPrice] : cache)
        maxPrice = max(maxPrice, totalPrice);

    cout << "We get " << maxPrice << " bananas" << endl;

    return 0;
}