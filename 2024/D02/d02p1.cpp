#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <numeric>

bool isSafe(std::vector<int> report)
{
    if (report.size() < 2) return false;
    
    bool isIncreasing = (report[0] < report[1]);

    std::adjacent_difference(report.begin(), report.end(), report.begin());

    if (!isIncreasing)
        std::for_each(report.begin(), report.end(), [](int& x) { x = -x; });    
    
    //count the number of unsafe transitions and return false if there are any
    //skip the first element since that is equal to the difference between zero and the first element
    return std::count_if(std::next(report.begin()), report.end(), [&](int x) {
        return (x < 1 || x > 3);
    }) == 0;
}

int main() {
    std::vector<std::vector<int>> data;
    std::string line;

    // Read from stdin until EOF
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int num;

        // Extract integers from the line
        while (iss >> num) {
            row.push_back(num);
        }

        // Add the row to the data
        if (!row.empty()) {
            data.push_back(row);
        }
    }

    int count = std::count_if(data.begin(), data.end(), isSafe);
    std::cout << count << std::endl;    

    return 0;
}
