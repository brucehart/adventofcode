#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

// Define the Node structure
struct Node {
    std::string left;
    std::string right;
};

int main() {
    std::string directions;
    std::unordered_map<std::string, Node> nodeMap;

    // Read the first line for directions
    std::getline(std::cin, directions);

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
    }

    int steps = 0;
    int dirIdx = 0;
    std::string currentNode = "AAA";

    while(currentNode != "ZZZ")
    {
        currentNode = (directions[dirIdx] == 'L') ? nodeMap[currentNode].left : nodeMap[currentNode].right;
        steps++;
        dirIdx = (dirIdx + 1) % directions.size();
    }

    std::cout << steps << std::endl;
    
    return 0;
}
