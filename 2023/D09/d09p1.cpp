#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>

std::vector<int> computeDiffs(std::vector<int> row) {
    std::vector<int> diffs;
    std::adjacent_difference(row.begin(), row.end(), std::back_inserter(diffs));
    diffs.erase(diffs.begin());
    return diffs;
}

int extrapolate(std::vector<int> row) {
    std::vector<std::vector<int>> data;
    data.push_back(row);
    
    while(std::any_of(data.back().begin(), data.back().end(), [](int val) { return val != 0; })) {
        data.push_back(computeDiffs(data.back()));
    }
        
    for(int i = data.size() - 2; i >= 0; i--) {
        data[i].push_back(data[i].back() + data[i + 1].back());
    }
 
    return data[0].back();
}

int main() {
    std::vector<std::vector<int>> sensorData;
    std::string line;   

    while (std::getline(std::cin, line)) {
        
        // Stop reading if the line is empty
        if (line.empty()) {
            break;
        }

        std::vector<int> row;
        std::stringstream ss(line);
        int value;

        // Parse integers from the line and add to the row vector
        while (ss >> value) {
            row.push_back(value);
        }

        // Add the row to the sensorData vector
        sensorData.push_back(row);
    }    

    int totalSum = 0;

    std::for_each(sensorData.begin(), sensorData.end(), [&](std::vector<int> row) {
        totalSum += extrapolate(row);
    });

    std::cout << totalSum << std::endl;

    return 0;
}
