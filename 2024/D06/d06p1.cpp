#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

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

    void navigate() {
        pos = findCursorPos();
        if ((*this)(pos.first, pos.second) != '\0') board[pos.first][pos.second]= 'X';

        while(true)
        {
            pos.first += move[direction].first;
            pos.second += move[direction].second;


            if ((*this)(pos.first,pos.second) == '.' || (*this)(pos.first, pos.second) == 'X')
            {            
                board[pos.first][pos.second] = 'X';            
            }
            else if ((*this)(pos.first, pos.second) == '#')
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
    }
};

int main(){
    std::string line;
    PuzzleMap puzzle;

    while (std::getline(std::cin, line)) {
        puzzle.insertRow(line);
    }

    puzzle.navigate();
    
    // for (auto row : puzzle.board) {
    //     for (auto c : row) {
    //         std::cout << c;
    //     }
    //     std::cout << std::endl;
    // }

    std::cout << puzzle.countVisited() << std::endl;    
}