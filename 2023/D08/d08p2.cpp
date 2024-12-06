#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <set>
#include <numeric>

// Define the Node structure
struct Node {
    std::string left;
    std::string right;
};

int main() {
    std::string directions;
    std::unordered_map<std::string, Node> nodeMap;
    std::vector<std::string> currentNode;

    // Read the first line for directions
    std::getline(std::cin, directions);
    // Trim any whitespace from directions
    directions.erase(0, directions.find_first_not_of(" \t\n\r"));
    directions.erase(directions.find_last_not_of(" \t\n\r") + 1);

    // Skip the blank line
    std::string blankLine;
    std::getline(std::cin, blankLine);

    // Process the remaining lines
    std::string line;
    while (std::getline(std::cin, line)) {
        // Ignore empty lines
        if (line.empty()) continue;

        // Parse the line in the format "AAA = (BBB, CCC)"
        std::istringstream iss(line);
        std::string key, equals, left, right;

        // Extract the key and the rest of the line
        if (!(iss >> key >> equals)) {
            std::cerr << "Error: Invalid input format." << std::endl;
            return 1;
        }

        // Remove the '=' and parse the left and right values
        std::getline(iss, line, '(');
        std::getline(iss, left, ',');
        std::getline(iss, right, ')');

        // Trim any whitespace from left and right
        left.erase(0, left.find_first_not_of(" \t\n\r"));
        left.erase(left.find_last_not_of(" \t\n\r") + 1);
        right.erase(0, right.find_first_not_of(" \t\n\r"));
        right.erase(right.find_last_not_of(" \t\n\r") + 1);

        // Store in the unordered_map
        nodeMap[key] = {left, right};
        if (key[key.size() - 1] == 'A') currentNode.push_back(key);
        
    }

    int steps = 0;
    int dirIdx = 0;   
    unsigned long long totalSteps = 1;
    
    for(auto node: currentNode) {            
        steps = 0;
        dirIdx = 0;       

        while(true){            
            if (node.back() == 'Z'){
                //verified manually that allow loops cycle back on themselves
                totalSteps = std::lcm(totalSteps, steps);                 
                break;
            }

            node = (directions[dirIdx] == 'L') ? nodeMap[node].left : nodeMap[node].right;            
            steps++;
            dirIdx = (dirIdx + 1) % directions.size();            
        }           
    }

    std::cout << totalSteps << std::endl;
    return 0;
}
