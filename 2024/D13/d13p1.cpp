#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <algorithm>

// Structure to hold the configuration of a claw machine
struct Machine {
    int a_x;
    int a_y;
    int b_x;
    int b_y;
    int p_x;
    int p_y;
};

// Helper function to extract integer values from a line
// It finds all occurrences of '+' or '=' followed by digits and extracts the numbers
std::vector<int> extract_numbers(const std::string& line) {
    std::vector<int> numbers;
    std::stringstream ss(line);
    char ch;
    int num;
    while (ss >> ch) {
        if (ch == '+' || ch == '=') {
            if (ss >> num) {
                numbers.push_back(num);
            }
        }
    }
    return numbers;
}

int main() {
    std::vector<Machine> machines;
    std::string line;
    
    // Read input line by line and parse into machines
    while (std::getline(std::cin, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        
        Machine m;
        
        // Parse Button A line
        std::vector<int> nums_a = extract_numbers(line);
        if (nums_a.size() != 2) {
            // Invalid format, skip this machine
            // Optionally, handle error
            // For this problem, we assume input is well-formed
            continue;
        }
        m.a_x = nums_a[0];
        m.a_y = nums_a[1];
        
        // Read Button B line
        if (!std::getline(std::cin, line)) break;
        std::vector<int> nums_b = extract_numbers(line);
        if (nums_b.size() != 2) {
            continue;
        }
        m.b_x = nums_b[0];
        m.b_y = nums_b[1];
        
        // Read Prize line
        if (!std::getline(std::cin, line)) break;
        std::vector<int> nums_p = extract_numbers(line);
        if (nums_p.size() != 2) {
            continue;
        }
        m.p_x = nums_p[0];
        m.p_y = nums_p[1];
        
        machines.push_back(m);
    }
    
    long long total_tokens = 0; // Use long long to prevent potential overflow
    
    // Iterate over each machine to find the minimal token cost
    for (const auto& machine : machines) {
        int min_cost = std::numeric_limits<int>::max();
        bool found = false;
        
        // Iterate over possible presses of Button A (0 to 100)
        for (int a = 0; a <= 100; ++a) {
            // Calculate remaining X and Y after pressing Button A 'a' times
            int rem_x = machine.p_x - a * machine.a_x;
            int rem_y = machine.p_y - a * machine.a_y;
            
            // If remaining X or Y is negative, this 'a' is not feasible
            if (rem_x < 0 || rem_y < 0) {
                continue;
            }
            
            // Since Button B moves the claw by b_x and b_y, solve for 'b'
            // We need to find 'b' such that:
            // b * B_x = rem_x
            // b * B_y = rem_y
            // Both equations must hold simultaneously
            if (machine.b_x == 0 && rem_x != 0) {
                continue; // No solution since B_x cannot contribute to remaining X
            }
            if (machine.b_y == 0 && rem_y != 0) {
                continue; // No solution since B_y cannot contribute to remaining Y
            }
            
            // If B_x is not zero, calculate b from X equation
            // Similarly for Y equation and ensure consistency
            int b_from_x = -1, b_from_y = -1;
            if (machine.b_x != 0) {
                if (rem_x % machine.b_x != 0) continue; // 'b' must be integer
                b_from_x = rem_x / machine.b_x;
            }
            if (machine.b_y != 0) {
                if (rem_y % machine.b_y != 0) continue; // 'b' must be integer
                b_from_y = rem_y / machine.b_y;
            }
            
            // Determine 'b' based on which buttons are used
            int b = -1;
            if (machine.b_x != 0 && machine.b_y != 0) {
                if (b_from_x != b_from_y) continue; // 'b' must satisfy both equations
                b = b_from_x;
            } else if (machine.b_x != 0) {
                b = b_from_x;
            } else if (machine.b_y != 0) {
                b = b_from_y;
            }
            
            // 'b' must be non-negative and within limits
            if (b < 0 || b > 100) {
                continue;
            }
            
            // Verify that the calculated 'b' actually satisfies both equations
            if (machine.a_x * a + machine.b_x * b != machine.p_x ||
                machine.a_y * a + machine.b_y * b != machine.p_y) {
                continue;
            }
            
            // Calculate the token cost for this combination
            int cost = 3 * a + b;
            if (cost < min_cost) {
                min_cost = cost;
                found = true;
            }
        }
        
        if (found) {
            total_tokens += min_cost;
        }
    }
    
    // Output the total minimal token cost
    std::cout << total_tokens << "\n";
    
    return 0;
}
