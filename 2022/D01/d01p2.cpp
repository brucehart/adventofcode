#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <cctype>

int main() {
    std::string line;
    long long current_sum = 0;
    std::vector<long long> calories_per_elf;

    // Read input line by line
    while (std::getline(std::cin, line)) {
        // Check if the line is empty (i.e., a blank line)
        if (line.empty()) {
            // Add the current_sum to the vector if it's greater than zero
            if (current_sum > 0) {
                calories_per_elf.push_back(current_sum);
                current_sum = 0; // Reset for the next Elf
            }
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

    // After the loop, check if there's a remaining sum to add
    if (current_sum > 0) {
        calories_per_elf.push_back(current_sum);
    }

    // Check if there are at least three Elves
    if (calories_per_elf.size() < 3) {
        std::cerr << "Not enough Elves to calculate the top three totals." << std::endl;
        return 1;
    }

    // Sort the vector in descending order to find the top three
    std::sort(calories_per_elf.begin(), calories_per_elf.end(), 
              [](const long long a, const long long b) -> bool {
                  return a > b;
              });

    // Sum the top three Calories
    long long top_three_sum = std::accumulate(calories_per_elf.begin(),
                                             calories_per_elf.begin() + 3, 0LL);

    // Output the result
    std::cout << top_three_sum << std::endl;

    return 0;
}

