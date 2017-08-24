
#include "black_and_white.hpp"


int main() {
    int sz;
    cin >> sz;
    vector<string> board(sz);
    for (auto i = 0; i < sz; ++i) {
        cin >> board[i];
    }

    BlackAndWhiteGame game;
    auto res = game.makeConnected(board);
    Println(cout, res.size());
    for (auto& s : res) {
        Println(cout, s);
    }
    cout.flush();
}
