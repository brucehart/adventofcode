#include <iostream>
#include <vector>
#include <algorithm>

int main() {
  std::vector<std::string> platform;
  std::string line;

  // Read the platform from stdin
  while (std::getline(std::cin, line)) {
    platform.push_back(line);
  }

  int rows = platform.size();
  int cols = platform[0].size();

  // Tilt the platform to the north
  for (int j = 0; j < cols; ++j) {
    int next_available_row = 0;
    for (int i = 0; i < rows; ++i) {
      if (platform[i][j] == 'O') {
        if (next_available_row != i) {
            platform[next_available_row][j] = 'O';
            platform[i][j] = '.';
        }
        next_available_row++;
      } else if (platform[i][j] == '#') {
        next_available_row = i + 1;
      }
    }
  }

  // Calculate the total load on the north support beams
  long long total_load = 0;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (platform[i][j] == 'O') {
        total_load += (rows - i);
      }
    }
  }

  // Output the total load
  std::cout << total_load << std::endl;

  return 0;
}
