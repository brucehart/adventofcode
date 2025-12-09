#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::string line;
    if (!std::getline(std::cin, line)) return 0;

    std::vector<int> draws;
    std::stringstream draw_stream(line);
    std::string number;
    while (std::getline(draw_stream, number, ',')) draws.push_back(std::stoi(number));

    std::vector<std::vector<int>> boards;
    std::vector<std::vector<bool>> marks;

    int value;
    std::vector<int> board;
    while (std::cin >> value) {
        board.push_back(value);
        if (board.size() == 25) {
            boards.push_back(board);
            marks.emplace_back(25, false);
            board.clear();
        }
    }

    std::vector<bool> won(boards.size(), false);
    int boards_won = 0;
    int last_score = 0;

    for (int draw : draws) {
        for (size_t b = 0; b < boards.size(); ++b) {
            if (won[b]) continue;

            for (int i = 0; i < 25; ++i)
                if (boards[b][i] == draw) marks[b][i] = true;

            bool win = false;
            for (int r = 0; r < 5 && !win; ++r) {
                bool row = true;
                for (int c = 0; c < 5; ++c) row = row && marks[b][r * 5 + c];
                if (row) win = true;
            }
            for (int c = 0; c < 5 && !win; ++c) {
                bool col = true;
                for (int r = 0; r < 5; ++r) col = col && marks[b][r * 5 + c];
                if (col) win = true;
            }

            if (win) {
                won[b] = true;
                boards_won++;

                int sum_unmarked = 0;
                for (int i = 0; i < 25; ++i)
                    if (!marks[b][i]) sum_unmarked += boards[b][i];

                last_score = sum_unmarked * draw;
                if (boards_won == static_cast<int>(boards.size())) {
                    std::cout << last_score << std::endl;
                    return 0;
                }
            }
        }
    }

    if (boards_won) std::cout << last_score << std::endl;
    return 0;
}
