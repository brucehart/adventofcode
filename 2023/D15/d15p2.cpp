#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

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
    steps.push_back(current_step);

    // Represent boxes as a vector of vectors of pairs (label, focal length)
    std::vector<std::vector<std::pair<std::string, int>>> boxes(256);

    for (const std::string& step : steps) {
        std::string label;
        char operation;
        int focal_length = 0;
        size_t i = 0;
       
        while (i < step.length() && step[i] != '=' && step[i] != '-') {
            label += step[i];
            i++;
        }
        operation = step[i];
       
        if (operation == '=') {
            focal_length = std::stoi(step.substr(i + 1));
        }

        int box_index = hash_algorithm(label);

        if (operation == '=') {
            // Check if the lens already exists in the box
            auto it = std::find_if(boxes[box_index].begin(), boxes[box_index].end(),
                                   [&label](const std::pair<std::string, int>& lens) {
                                       return lens.first == label;
                                   });

            if (it != boxes[box_index].end()) {
                // Replace the existing lens
                it->second = focal_length;
            } else {
                // Add the new lens
                boxes[box_index].push_back({label, focal_length});
            }
        } else { 
            // Remove the lens if it exists
            boxes[box_index].erase(std::remove_if(boxes[box_index].begin(), boxes[box_index].end(),
                                                  [&label](const std::pair<std::string, int>& lens) {
                                                      return lens.first == label;
                                                  }),
                                  boxes[box_index].end());
        }
    }

    // Calculate the focusing power
    long long total_focusing_power = 0;
    for (int i = 0; i < boxes.size(); ++i) {
        for (int j = 0; j < boxes[i].size(); ++j) {
            total_focusing_power += (i + 1) * (j + 1) * boxes[i][j].second;
        }
    }

    std::cout << total_focusing_power << std::endl;

    return 0;
}
