#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// Function to trim leading and trailing spaces from a string
std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" ");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" ");
    return s.substr(start, end - start + 1);
}

int main() {
    std::string line;
    std::vector<std::string> crate_lines;
    std::vector<int> stack_positions;
    std::vector<std::vector<char>> stacks;

    // Step 1: Read the initial crate configuration
    while (std::getline(std::cin, line)) {
        // If the line contains '1', it's the stack number line
        if (line.find('1') != std::string::npos) {
            // Determine stack positions by finding indices of digits
            for (size_t i = 0; i < line.size(); ++i) {
                if (std::isdigit(line[i])) {
                    stack_positions.push_back(static_cast<int>(i));
                }
            }
            // Initialize stacks based on the number of stacks
            stacks.resize(stack_positions.size(), std::vector<char>());
            break;
        }
        // Otherwise, store the crate lines
        crate_lines.push_back(line);
    }

    // Step 2: Populate the stacks by reading crate lines in reverse order
    for (auto it = crate_lines.rbegin(); it != crate_lines.rend(); ++it) {
        const std::string& crate_line = *it;
        for (size_t i = 0; i < stack_positions.size(); ++i) {
            int pos = stack_positions[i];
            // Ensure the position is within the line length
            if (pos < static_cast<int>(crate_line.size())) {
                char c = crate_line[pos];
                if (c != ' ') {
                    // Add the crate label to the corresponding stack
                    stacks[i].push_back(c);
                }
            }
        }
    }

    // Step 3: Read the blank line separating crates and move commands
    std::getline(std::cin, line); // This should be the blank line

    // Step 4: Process each move command
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue; // Skip empty lines if any
        }

        std::stringstream ss(line);
        std::string move_str, from_str, to_str;
        int quantity, from_stack, to_stack;

        // Parse the move command
        ss >> move_str >> quantity >> from_str >> from_stack >> to_str >> to_stack;

        // Adjust stack indices (1-based to 0-based)
        from_stack -= 1;
        to_stack -= 1;

        // Perform the move one crate at a time
        for (int i = 0; i < quantity; ++i) {
            if (!stacks[from_stack].empty()) {
                char crate = stacks[from_stack].back();
                stacks[from_stack].pop_back();
                stacks[to_stack].push_back(crate);
            }
        }
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

