#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Function to calculate the HASH value of a string
int hash_algorithm(const std::string& str) {
    int current_value = 0;
    for (char c : str) {
        current_value += static_cast<int>(c);
        current_value *= 17;
        current_value %= 256;
    }
    return current_value;
}

int main() {
    std::string line;
    std::getline(std::cin, line);

    // Split the line into steps using ',' as the delimiter
    std::vector<std::string> steps;
    std::string current_step;
    for (char c : line) {
        if (c == ',') {
            steps.push_back(current_step);
            current_step = "";
        } else {
            current_step += c;
        }
    }
    steps.push_back(current_step); // Add the last step

    // Calculate the sum of HASH values for each step
    long long sum_of_hashes = 0;
    for (const std::string& step : steps) {
        sum_of_hashes += hash_algorithm(step);
    }

    std::cout << sum_of_hashes << std::endl;

    return 0;
}
