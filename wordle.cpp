// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(string & in, string::size_type left, string remainingFloating, const set<string>& dict, set<string>& ret);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    string inCopy = in;
    set<string> ret;
    wordleHelper(inCopy, 0, floating, dict, ret);
    return ret;
}

// Define any helper functions here
void wordleHelper(string & in, string::size_type left, string remainingFloating, const set<string>& dict, set<string>& ret)
{
    if (left == in.size())
    {
        if (remainingFloating.size() == 0) {
            if (dict.find(in) != dict.end()) {
                ret.insert(in);
            }
        }
        return;
    }
    if (in[left] != '-') {
        wordleHelper(in, left+1, remainingFloating, dict, ret);
        return;
    }
    for(char i = 'a'; i <= 'z'; i++) {
        in[left] = i;
        string newRemainingFloating = remainingFloating;
        string::size_type index = newRemainingFloating.find(i);
        if (index != string::npos) newRemainingFloating.erase(index, 1);
        wordleHelper(in, left+1, newRemainingFloating, dict, ret);
    }
    in[left] = '-';
}