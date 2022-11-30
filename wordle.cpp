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
void wordleHelper(string & in, string::size_type left, string::size_type blanks, string remainingFloating, const set<string>& dict, set<string>& ret);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    string inCopy = in;
    set<string> ret;
    int blanks = 0;
    for (char c : in) {
        if (c == '-') ++blanks;
    }
    wordleHelper(inCopy, 0, blanks, floating, dict, ret);
    return ret;
}

// Define any helper functions here
void wordleHelper(string & in, string::size_type left, string::size_type blanks, string remainingFloating, const set<string>& dict, set<string>& ret)
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
        wordleHelper(in, left+1, blanks, remainingFloating, dict, ret);
        return;
    }

    // check floating letters first
    for(int i = remainingFloating.size()-1; i >= 0; i--) {
        in[left] = remainingFloating[i];
        string newRemainingFloating = remainingFloating;
        newRemainingFloating.erase(i, 1);
        wordleHelper(in, left+1, blanks-1, newRemainingFloating, dict, ret);
    }

    if (remainingFloating.size() > blanks-1) {
        in[left] = '-';
        return;
    }

    // then check the rest
    for(char i = 'a'; i <= 'z'; i++) {
        string::size_type index = remainingFloating.find(i);
        if (index == string::npos) { // if it's a floating letter, we've already checked it
            in[left] = i;
            wordleHelper(in, left+1, blanks-1, remainingFloating, dict, ret);
        }
    }
    in[left] = '-';
}