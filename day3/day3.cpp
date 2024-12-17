/*
Lessons
(i) didn't handle multi-line input. thought the entire input was a continuous string so got wrong answers until I realized this
(ii) mistake : didn't persist do or don't state of one line to its next line
*/

#include <iostream>
#include <string>
#include <regex>
#include <sstream>

using namespace std;

//takes in a string and returns the sum of valid mults
int doMult(const string& word) {
    int result = 0;
    regex mulRegex(R"(mul\((\d{1,3}),(\d{1,3})\))");
    smatch match;
    auto it = word.cbegin();
    while (regex_search(it, word.cend(), match, mulRegex)) {
        int op1 = stoi(match[1]);
        int op2 = stoi(match[2]);
        result += op1 * op2;
        it = match.suffix().first;
    }
    return result;
}

int multWords(const string& word, bool& inDontMode) {
    /* My algorithm:
        1. Start at the beginning of string in the previous line's inDontMode mode(
        if previous line ended with a do, inDontMode=false else it's true)
        2. Build the substring until a "don't" is seen
           2.1 Process substring using the doMult if coming from do Mode
        3. After seeing "don't", set inDontMode flag to true
        4. If "do" is seen, reset the flag and continue without any processing
    */
    int result = 0;
    int l = 0, r = 0;

    while (r < word.length()) {
        if (word.substr(r, 5) == "don't") {
            if (!inDontMode) {
                result += doMult(word.substr(l, r - l));
                r += 5;
                l = r;
            } else {
                r++;
            }
            inDontMode = true;
        }
        else if (word.substr(r, 2) == "do") {
            if (inDontMode) {
                r += 2;
                l = r;
            } else {
                r++;
            }
            inDontMode = false;
        } 
        else {
            r++;
        }
    }

    if (!inDontMode) {
        result += doMult(word.substr(l, word.length() - l));
    }

    return result;
}

int main() {
    string word;
    int result = 0;
    bool inDontMode = false;

    while (getline(cin, word)) {
        result += multWords(word, inDontMode);
    }

    cout << result << endl;
    return 0;
}
