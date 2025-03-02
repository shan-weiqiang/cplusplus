#include <vector>
#include <tuple>
#include <iostream>

using namespace std;

class Solution {
private:
    bool isValid(vector<vector<char>>& board, vector<tuple<int, int>>& seq,
                 char color) {
        auto it = ++seq.begin();
        for (; it != seq.end(); ++it) {
            auto value = board[std::get<0>(*it)][std::get<1>(*it)];
            if (value != color) {
                if (value == '.') {
                    return false;
                } else {
                    continue;
                }
            } else {
                if (it - seq.begin() < 2) {
                    return false;
                }
                return true;
            }
        }
        return false;
    }

public:
    bool checkMove(vector<vector<char>>& board, int rMove, int cMove,
                   char color) {
        if (board[rMove][cMove] != '.') {
            return false;
        }
        vector<tuple<int, int>> seq;
        for (int i = rMove; i < 8; ++i) {
            seq.push_back(std::make_tuple(i, cMove));
        }
        if (isValid(board, seq, color)) {
            return true;
        }
        seq.clear();
        for (int i = rMove; i >= 0; --i) {
            seq.push_back(std::make_tuple(i, cMove));
        }
        if (isValid(board, seq, color)) {
            return true;
        }
        seq.clear();
        for (int i = cMove; i < 8; ++i) {
            seq.push_back(std::make_tuple(rMove, i));
        }
        if (isValid(board, seq, color)) {
            return true;
        }
        seq.clear();
        for (int i = cMove; i >= 0; --i) {
            seq.push_back(std::make_tuple(rMove, i));
        }
        if (isValid(board, seq, color)) {
            return true;
        }
        seq.clear();
        for (int i = rMove, j = cMove; i >= 0 && j >= 0; --i, --j) {
            seq.push_back(std::make_tuple(i, j));
        }
        if (isValid(board, seq, color)) {
            return true;
        }
        seq.clear();
        for (int i = rMove, j = cMove; i < 8 && j >= 0; ++i, --j) {
            seq.push_back(std::make_tuple(i, j));
        }
        if (isValid(board, seq, color)) {
            return true;
        }
        seq.clear();
        for (int i = rMove, j = cMove; i < 8 && j < 8; ++i, ++j) {
            seq.push_back(std::make_tuple(i, j));
        }
        if (isValid(board, seq, color)) {
            return true;
        }
        seq.clear();
        for (int i = rMove, j = cMove; i >= 0 && j < 8; --i, ++j) {
            seq.push_back(std::make_tuple(i, j));
        }
        if (isValid(board, seq, color)) {
            return true;
        }
        seq.clear();
        return false;
    }
};

int main(){
        std::vector<std::vector<char>> test = {
    { '.', '.', '.', '.', '.', '.', '.', '.' }, 
    { '.', 'B', '.', '.', 'W', '.', '.', '.' }, 
    { '.', '.', 'W', '.', '.', '.', '.', '.' }, 
    { '.', '.', '.', 'W', 'B', '.', '.', '.' }, 
    { '.', '.', '.', '.', '.', '.', '.', '.' }, 
    { '.', '.', '.', '.', 'B', 'W', '.', '.' }, 
    { '.', '.', '.', '.', '.', '.', 'W', '.' }, 
    { '.', '.', '.', '.', '.', '.', '.', 'B' } };

    std::cout << test[4][4] << std::endl;

    std::cout << Solution().checkMove(test, 4, 4, 'W') << std::endl;
}