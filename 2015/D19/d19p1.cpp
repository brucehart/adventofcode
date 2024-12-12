#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

int main() {
    std::vector<std::pair<std::string, std::string>> replacements;
    std::string line;
    
    // Read replacement rules
    while (std::getline(std::cin, line) && !line.empty()) {
        size_t pos = line.find(" => ");
        if (pos != std::string::npos) {
            replacements.push_back({line.substr(0, pos), line.substr(pos + 4)});
        }
    }

    // Read the medicine molecule
    std::string medicineMolecule;
    std::getline(std::cin, medicineMolecule);

    std::unordered_set<std::string> distinctMolecules;

    // Iterate through the medicine molecule
    for (size_t i = 0; i < medicineMolecule.size(); ++i) {
        // Iterate through each replacement rule
        for (const auto& replacement : replacements) {
            const std::string& from = replacement.first;
            const std::string& to = replacement.second;

            // Check if the current position in the molecule matches the 'from' part of the replacement
            if (medicineMolecule.substr(i, from.size()) == from) {
                // Create a new molecule by replacing 'from' with 'to'
                std::string newMolecule = medicineMolecule.substr(0, i) + to + medicineMolecule.substr(i + from.size());
                // Add the new molecule to the set of distinct molecules
                distinctMolecules.insert(newMolecule);
            }
        }
    }

    // Output the number of distinct molecules generated
    std::cout << distinctMolecules.size() << std::endl;

    return 0;
}
