#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

int main() {
    std::string line;
    long long current_sum = 0;
    long long max_calories = 0;

    // Read input line by line
    while (std::getline(std::cin, line)) {
        // Check if the line is empty (i.e., a blank line)
        if (line.empty()) {
            // Update max_calories if current_sum is greater
            max_calories = std::max(max_calories, current_sum);
            // Reset current_sum for the next Elf
            current_sum = 0;
        } else {
            // Convert the line to a number and add to current_sum
            // Using std::stoll for string to long long conversion
            try {
                current_sum += std::stoll(line);
            } catch (const std::invalid_argument& e) {
                // Handle the case where the line is not a valid number
                std::cerr << "Invalid input encountered: " << line << std::endl;
                return 1;
            } catch (const std::out_of_range& e) {
                // Handle the case where the number is out of range for long long
                std::cerr << "Number out of range: " << line << std::endl;
                return 1;
            }
        }
    }

    // After the loop, check the last Elf's calories
    max_calories = std::max(max_calories, current_sum);

    // Output the maximum Calories carried by any Elf
    std::cout << max_calories << std::endl;

    return 0;
}

