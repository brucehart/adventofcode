#include <iostream>
#include <string>
#include <algorithm>

int main() {
  std::string line;
  long long total_sum = 0;

  while (std::getline(std::cin, line)) {
    // Iterate through the line, searching for numbers
    for (size_t i = 0; i < line.size(); ++i) {
      if (std::isdigit(line[i]) || line[i] == '-') {
        // Potential start of a number
        size_t j = i + 1;
        while (j < line.size() && std::isdigit(line[j])) {
          ++j;
        }

        // Extract the number substring and convert to long long
        try {
          total_sum += std::stoll(line.substr(i, j - i));
        } catch (const std::out_of_range& oor) {
          // Handle potential out-of-range errors during conversion
          // (although the problem statement doesn't suggest this is necessary, it's good practice)
        }

        // Move the index past the parsed number
        i = j - 1;
      }
    }
  }

  std::cout << total_sum << std::endl;

  return 0;
}
