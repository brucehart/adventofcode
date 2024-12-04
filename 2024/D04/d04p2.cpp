#include <iostream>
#include <vector>

#include <vector>
#include <string>

struct Puzzle {
    std::vector<std::vector<char>> board;

    int rows() const {
        return board.size();
    }

    int cols() const {
        return (board.size() > 0) ? board[0].size() : 0;
    }

    // Direction defined as an enum and an accompanying lookup table for deltas
    enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        UP_LEFT,
        UP_RIGHT,
        DOWN_LEFT,
        DOWN_RIGHT
    };

    const std::pair<int, int> directionDeltas[8] = {
        {-1, 0}, // UP
        {1, 0},  // DOWN
        {0, -1}, // LEFT
        {0, 1},  // RIGHT
        {-1, -1}, // UPLEFT
        {-1, 1},  // UPRIGHT
        {1, -1}, // DOWNLEFT
        {1, 1}   // DOWNRIGHT
    };

    // Overloaded operator for safe access to the board
    char operator()(int i, int j) const {
        if (i < 0 || i >= rows() || j < 0 || j >= cols()) {
            return '\0';
        }
        return board[i][j];
    }

    // Inserts a row into the board
    void insertRow(const std::string& rowData) {
        std::vector<char> row(rowData.begin(), rowData.end());
        board.push_back(row);
    }

    // Gets a word starting at (row, col) in a specific direction
    std::string getWord(int row, int col, int length, Direction dir) {
        std::string word;
        const auto& delta = directionDeltas[dir];
        for (int i = 0; i < length; ++i) {
            char c = (*this)(row + i * delta.first, col + i * delta.second);
            if (c == '\0') break; // Stop if out of bounds
            word.push_back(c);
        }
        return word;
    }

    bool evaluateLetter(int row, int col) {
        auto w = getWord(row-1, col-1, 3, DOWN_RIGHT);        
        if (!(w == "SAM" || w == "MAS")) return false;

        w = getWord(row+1, col-1, 3, UP_RIGHT);
        if (!(w == "SAM" || w == "MAS")) return false;

        return true;
    }
};


int main() {
    Puzzle p;
    int count = 0;
    
    std::string line;
    while (std::getline(std::cin, line)) {
        p.insertRow(line);
    }

    for (int i = 0; i < p.rows(); i++) {
        for (int j = 0; j < p.cols(); j++) {
            if (p(i, j) == 'A' && p.evaluateLetter(i, j)) 
                count++;
        }        
    }

    std::cout << count << std::endl;
    
    return 0;
}