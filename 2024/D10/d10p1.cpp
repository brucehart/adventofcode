#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <set>

struct Point {
    int row;
    int col;

    Point(int r, int c) : row(r), col(c) {}

    bool operator==(const Point& other) const {
        return row == other.row && col == other.col;
    }
};

struct TopoMap {
    std::vector<std::vector<int>> grid;

    int rows() const {
        return grid.size();
    }

    int cols() const {
        return (grid.size() > 0) ? grid[0].size() : 0;
    }

    void insertRow(std::string row) {        
        std::vector<int> newRow(row.begin(), row.end());
        grid.push_back(newRow);
    }
       
    char get(int row, int col) const {
        if (row < 0 || row >= rows() || col < 0 || col >= cols()) {
            return '\0';
        }

        return grid[row][col];
    }

    char get(Point p) const {
        return get(p.row, p.col);
    }

    std::vector<Point> findTrailHeads() {
        std::vector<Point> trailHeads;

        for (int r = 0; r < rows(); ++r) {
            for (int c = 0; c < cols(); ++c) {
                if (grid[r][c] == '0') {
                    trailHeads.push_back(Point(r, c));
                }
            }
        }

        return trailHeads;
    }

    int traverseTrail(const Point& start, const Point& current, std::set<unsigned long long>& foundSet) {
        int score = 0;       
        
        char trailValue = get(current);

        if (trailValue == '9')            
            foundSet.insert(start.row*1000000000 + start.col*1000000 + current.row*1000 + current.col);
        
        auto nextSteps = {Point(current.row - 1, current.col),
                            Point(current.row + 1, current.col),
                            Point(current.row, current.col - 1),
                            Point(current.row, current.col + 1)};
        
        std::for_each(nextSteps.begin(), nextSteps.end(), [&](Point p) {
            if (get(p) == trailValue + 1)
                traverseTrail(start, p, foundSet);
        });        

        return score;
    }

};

int main() {
    TopoMap tm;
    std::string line;

    while (std::getline(std::cin, line)) {
        tm.insertRow(line);
    }

    auto trailHeads = tm.findTrailHeads();
    int totalScore = 0;
    std::set<unsigned long long> foundSet;

    std::for_each(trailHeads.begin(), trailHeads.end(), [&](Point p) {
        tm.traverseTrail(p,p,foundSet);
    });

    std::cout << foundSet.size() << std::endl;    
}