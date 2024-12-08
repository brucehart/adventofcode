#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

struct Galaxy {
    std::vector<std::vector<int>> grid;
    std::vector<int> expandedRows;
    std::vector<int> expandedCols;

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

    void expandRows() {
        for (int i = 0; i < rows(); ++i) {
            if (std::all_of(grid[i].begin(), grid[i].end(), [](int c) { return c == '.'; })) {
                expandedRows.push_back(i);
            }
        }
    }

    void expandCols() {
        for (int j = 0; j < cols(); j++)
        {
            if (std::all_of(grid.begin(), grid.end(), [j](const std::vector<int>& row) { return row[j] == '.'; })) {
                expandedCols.push_back(j);                
            }
        }
    }
    
    void expand() {
        expandRows();
        expandCols();
    }

    std::vector<std::pair<int, int>> getGalaxies() {
        std::vector<std::pair<int, int>> galaxies;
        for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
                if (grid[i][j] == '#') {
                    galaxies.push_back({i, j});
                }
            }
        }
        return galaxies;
    }

    unsigned long long getTotalDistance()
    {
        auto galaxies = getGalaxies();
        unsigned long long totalDistance = 0;

        for(int i = 0; i < galaxies.size()-1; i++)
        {
            for (int j = i+1; j < galaxies.size(); j++)
            {
                int r1 = galaxies[i].first;
                int r2 = galaxies[j].first;
                if (r1 > r2) std::swap(r1, r2);

                unsigned long long expandR = (1000000ull-1) * std::count_if(expandedRows.begin(), expandedRows.end(),
                    [r1, r2](int row) { return row > r1 && row < r2; });

                int c1 = galaxies[i].second;
                int c2 = galaxies[j].second;
                if (c1 > c2) std::swap(c1, c2);

                unsigned long long expandC = (1000000ull-1) * std::count_if(expandedCols.begin(), expandedCols.end(),
                    [c1, c2](int col) { return col > c1 && col < c2; });
                    
                totalDistance += std::abs(galaxies[i].first - galaxies[j].first) + expandR;
                totalDistance += std::abs(galaxies[i].second - galaxies[j].second) + expandC;
            }            
        }

        return totalDistance;
    }
};

int main() {
    Galaxy g;
    std::string line;

    while (std::getline(std::cin, line)) {
        g.insertRow(line);
    }

    g.expand();
    unsigned long long distance = g.getTotalDistance();

    std::cout << distance << std::endl;
}