#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

int main() {
    // Vector to store replacement rules as pairs (output, input)
    std::vector<std::pair<std::string, std::string>> replacements;
    std::string line;

    // Read replacement rules
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break; // Empty line indicates end of replacements
        }
        // Find the delimiter " => "
        size_t pos = line.find(" => ");
        if (pos != std::string::npos) {
            std::string input = line.substr(0, pos);
            std::string output = line.substr(pos + 4);
            // Store reversed replacements for greedy reduction
            replacements.emplace_back(output, input);
        }
    }

    // Read the medicine molecule
    std::string molecule;
    if (std::getline(std::cin, line)) {
        molecule = line;
    }

    // Sort the replacements by descending length of output to prioritize longer replacements
    std::sort(replacements.begin(), replacements.end(),
              [](const std::pair<std::string, std::string>& a,
                 const std::pair<std::string, std::string>& b) -> bool {
                  return a.first.size() > b.first.size();
              });

    size_t steps = 0;
    // Greedy approach: reduce the molecule by applying the replacements
    while (molecule != "e") {
        bool replaced = false;
        for (const auto& [output, input] : replacements) {
            size_t pos = molecule.find(output);
            if (pos != std::string::npos) {
                // Replace the first occurrence of output with input
                molecule.replace(pos, output.length(), input);
                steps++;
                replaced = true;
                break; // Restart from the first replacement
            }
        }
        if (!replaced) {
            // If no replacement was made, it's impossible to reach 'e'
            std::cerr << "No valid replacement found. Transformation failed.\n";
            return 1;
        }
    }

    // Output the number of steps
    std::cout << steps << "\n";

    return 0;
}

