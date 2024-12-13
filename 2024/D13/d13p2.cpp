#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

// Structure to hold the configuration of a claw machine
struct Machine {
    long long a_x;
    long long a_y;
    long long b_x;
    long long b_y;
    long long p_x;
    long long p_y;
};

// Helper function to extract integer values from a line
// It finds all occurrences of '+' or '=' followed by digits and extracts the numbers
std::vector<long long> extract_numbers(const std::string& line) {
    std::vector<long long> numbers;
    std::stringstream ss(line);
    char ch;
    long long num;
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
    const long long OFFSET = 10000000000000; // 10^13

    // Read input line by line and parse into machines
    while (std::getline(std::cin, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        Machine m;

        // Parse Button A line
        std::vector<long long> nums_a = extract_numbers(line);
        if (nums_a.size() != 2) {
            // Invalid format, skip this machine
            // Assuming input is well-formed as per problem statement
            continue;
        }
        m.a_x = nums_a[0];
        m.a_y = nums_a[1];

        // Read Button B line
        if (!std::getline(std::cin, line)) break;
        std::vector<long long> nums_b = extract_numbers(line);
        if (nums_b.size() != 2) {
            continue;
        }
        m.b_x = nums_b[0];
        m.b_y = nums_b[1];

        // Read Prize line
        if (!std::getline(std::cin, line)) break;
        std::vector<long long> nums_p = extract_numbers(line);
        if (nums_p.size() != 2) {
            continue;
        }
        // Add the OFFSET to both X and Y coordinates as per Part Two
        m.p_x = nums_p[0] + OFFSET;
        m.p_y = nums_p[1] + OFFSET;

        machines.push_back(m);
    }

    long long total_tokens = 0; // Use long long to handle large token counts

    // Iterate over each machine to find the minimal token cost using Cramer's Rule
    for (const auto& machine : machines) {
        // Compute the determinant of the coefficient matrix
        long long det = machine.a_x * machine.b_y - machine.a_y * machine.b_x;

        // If determinant is zero, the system has no unique solution
        if (det == 0) {
            continue;
        }

        // Compute the determinants for Cramer's Rule
        long long det_a = machine.p_x * machine.b_y - machine.p_y * machine.b_x;
        long long det_b = machine.a_x * machine.p_y - machine.a_y * machine.p_x;

        // Check if det divides det_a and det_b exactly
        if (det_a % det != 0 || det_b % det != 0) {
            continue; // No integer solution exists
        }

        long long a = det_a / det;
        long long b = det_b / det;

        // Ensure that a and b are non-negative
        if (a < 0 || b < 0) {
            continue;
        }

        // Calculate the token cost for this combination
        long long cost = 3 * a + b;

        // Accumulate the total tokens required
        total_tokens += cost;
    }

    // Output the total minimal token cost
    std::cout << total_tokens << std::endl;

    return 0;
}

