#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <set>

struct PuzzleMap {
    enum DIRECTIONS {UP, RIGHT, DOWN, LEFT};
    std::vector<std::pair<int, int>> move = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};    
    std::vector<std::vector<char>> board;
    std::pair<int, int> pos;
    int direction;
    const std::string directions = "^>v<";

    int rows() const {
        return board.size();
    }

    int cols() const {
        return (board.size() > 0) ? board[0].size() : 0;
    }

    char operator()(int i, int j) const {
        if (i < 0 || i >= rows() || j < 0 || j >= cols()) {
            return '\0';
        }
        return board[i][j];
    }

    PuzzleMap& operator=(const PuzzleMap& other) {
        board = other.board;
        return *this;
    }

    void insertRow(const std::string& rowData) {
        std::vector<char> row(rowData.begin(), rowData.end());
        board.push_back(row);
    }

    int countVisited() {
        int count = 0;
        for (const auto& row : board) {
            count += std::count(row.begin(), row.end(), 'X');
        }
        return count;
    }

    std::pair<int, int> findCursorPos() {        
        for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                if (directions.find(board[i][j]) != std::string::npos){
                    direction = directions.find(board[i][j]);
                    return {i, j};
                }
            }
        }
        return {-1, -1};
    }

    bool navigate() {
        std::set<unsigned long long> loopFinder;        
        pos = findCursorPos();
        if ((*this)(pos.first, pos.second) != '\0') board[pos.first][pos.second]= 'X';

        while(true)
        {
            unsigned long long hash = pos.first * (1 << 20) + pos.second * (1 << 10) + direction;
            if (loopFinder.find(hash) != loopFinder.end()) return true;
            loopFinder.insert(hash);
            
            pos.first += move[direction].first;
            pos.second += move[direction].second;


            if ((*this)(pos.first,pos.second) == '.' || (*this)(pos.first, pos.second) == 'X')
            {            
                board[pos.first][pos.second] = 'X';            
            }
            else if ((*this)(pos.first, pos.second) == '#' || (*this)(pos.first, pos.second) == 'O')
            {
                pos.first -= move[direction].first;
                pos.second -= move[direction].second;
                direction = (direction + 1) % 4;                
            }
            else if ((*this)(pos.first, pos.second)== '\0')
            {
                break;
            }
        }

        return false;
    }
};

int main(){
    std::string line;
    PuzzleMap puzzle, originalPuzzle;
    int stuckCount = 0;

    while (std::getline(std::cin, line)) {
        originalPuzzle.insertRow(line);
    }

    for(int i = 0; i < originalPuzzle.rows(); ++i) {
        for(int j = 0; j < originalPuzzle.cols(); ++j) {
            if (originalPuzzle(i, j) == '.') {
                puzzle = originalPuzzle;
                puzzle.board[i][j] = 'O';
                if (puzzle.navigate()) stuckCount++;
            }
        }
    }
    
    // for (auto row : puzzle.board) {
    //     for (auto c : row) {
    //         std::cout << c;
    //     }
    //     std::cout << std::endl;
    // }

    std::cout << stuckCount << std::endl;    
}