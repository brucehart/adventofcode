#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

struct Galaxy {
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

    void expandRows() {
        for (int i = 0; i < rows(); ++i) {
            if (std::all_of(grid[i].begin(), grid[i].end(), [](int c) { return c == '.'; })) {
                grid.insert(grid.begin() + i + 1, grid[i]);
                i++;
            }
        }
    }

    void expandCols() {
        for (int j = 0; j < cols(); j++)
        {
            if (std::all_of(grid.begin(), grid.end(), [j](const std::vector<int>& row) { return row[j] == '.'; })) {
                for (int i = 0; i < rows(); i++) {
                    grid[i].insert(grid[i].begin() + j + 1, '.');
                }
                j++;
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
                totalDistance += std::abs(galaxies[i].first - galaxies[j].first) + std::abs(galaxies[i].second - galaxies[j].second);
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