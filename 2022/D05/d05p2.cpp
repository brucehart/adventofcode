#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>


// Function to trim leading and trailing spaces from a string
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" ");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" ");
    return s.substr(start, end - start + 1);
}

int main() {
    std::string line;
    std::vector<std::string> crate_lines;         // Stores the initial crate configuration lines
    std::vector<int> stack_positions;             // Stores the column indices of each stack
    std::vector<std::vector<char>> stacks;        // Represents each stack as a vector of chars

    // Step 1: Read the initial crate configuration
    while (std::getline(std::cin, line)) {
        // Identify the stack number line (contains digits)
        if (line.find('1') != std::string::npos) {
            // Determine stack positions by finding indices of digits
            for (size_t i = 0; i < line.size(); ++i) {
                if (std::isdigit(line[i])) {
                    stack_positions.push_back(static_cast<int>(i));
                }
            }
            // Initialize stacks based on the number of stacks found
            stacks.resize(stack_positions.size(), std::vector<char>());
            break; // Proceed to process crate lines
        }
        // Store the crate lines for later processing
        crate_lines.push_back(line);
    }

    // Step 2: Populate the stacks by reading crate lines in reverse order (bottom to top)
    for (auto it = crate_lines.rbegin(); it != crate_lines.rend(); ++it) {
        const std::string& crate_line = *it;
        for (size_t i = 0; i < stack_positions.size(); ++i) {
            int pos = stack_positions[i];
            // Ensure the position is within the line length
            if (pos < static_cast<int>(crate_line.size())) {
                char c = crate_line[pos];
                if (c != ' ') { // If there's a crate at this position
                    stacks[i].push_back(c);
                }
            }
        }
    }

    // Step 3: Read the blank line separating crate configuration and move commands
    std::getline(std::cin, line); // This should be the blank line

    // Step 4: Process each move command
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue; // Skip any empty lines
        }

        std::stringstream ss(line);
        std::string move_str, from_str, to_str;
        int quantity, from_stack, to_stack;

        // Parse the move command format: "move X from Y to Z"
        ss >> move_str >> quantity >> from_str >> from_stack >> to_str >> to_stack;

        // Adjust stack indices from 1-based to 0-based
        int source = from_stack - 1;
        int destination = to_stack - 1;

        // Validate stack indices
        if (source < 0 || source >= static_cast<int>(stacks.size()) ||
            destination < 0 || destination >= static_cast<int>(stacks.size())) {
            std::cerr << "Invalid stack number in command: " << line << std::endl;
            continue; // Skip invalid commands
        }

        // Validate quantity
        if (quantity <= 0 || quantity > static_cast<int>(stacks[source].size())) {
            std::cerr << "Invalid quantity in command: " << line << std::endl;
            continue; // Skip invalid commands
        }

        // Extract the crates to move while preserving their order
        std::vector<char> crates_to_move;
        crates_to_move.reserve(quantity); // Reserve space for efficiency

        // Extract the last 'quantity' crates from the source stack
        for (int i = 0; i < quantity; ++i) {
            crates_to_move.push_back(stacks[source].back());
            stacks[source].pop_back();
        }

        // Since we extracted them in reverse order, reverse to maintain original sequence
        std::reverse(crates_to_move.begin(), crates_to_move.end());

        // Append the crates to the destination stack
        stacks[destination].insert(stacks[destination].end(),
                                   crates_to_move.begin(),
                                   crates_to_move.end());
    }

    // Step 5: Collect the top crate of each stack
    std::string result;
    for (const auto& stack : stacks) {
        if (!stack.empty()) {
            result += stack.back();
        } else {
            result += ' '; // Use space if the stack is empty
        }
    }

    // Output the final result followed by a newline
    std::cout << result << std::endl;

    return 0;
}

