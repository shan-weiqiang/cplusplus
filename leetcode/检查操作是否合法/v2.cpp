#include <utility>
#include <vector>
#include <tuple>
#include <iostream>

using namespace std;

class Solution {
  private:
    bool isValid(vector<vector<char>> &board, vector<tuple<int, int>> &seq,
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
    bool checkMove(vector<vector<char>> &board, int rMove, int cMove,
                   char color) {
        vector<int> dr = {-1, -1, 0, 1, 1, 1, 0, -1};
        vector<int> dc = {0, 1, 1, 1, 0, -1, -1, -1};
        for (int i = 0; i < 8; ++i) {
            vector<std::tuple<int, int>> tuple;
            int x = rMove;
            int y = cMove;
            while (x >= 0 && x < 8 && y >= 0 && y < 8) {
                tuple.push_back(std::make_pair(x, y));
                x += dr[i];
                y += dc[i];
            }
            if (isValid(board, tuple, color)) {
                return true;
            }
        }
        return false;
    }
};

int main() {
    std::vector<std::vector<char>> test = {
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', 'B', '.', '.', 'W', '.', '.', '.'},
        {'.', '.', 'W', '.', '.', '.', '.', '.'},
        {'.', '.', '.', 'W', 'B', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', 'B', 'W', '.', '.'},
        {'.', '.', '.', '.', '.', '.', 'W', '.'},
        {'.', '.', '.', '.', '.', '.', '.', 'B'}};

    std::cout << test[4][4] << std::endl;

    std::cout << Solution().checkMove(test, 4, 4, 'W') << std::endl;
}