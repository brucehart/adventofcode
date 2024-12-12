#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

int main() {
  // Initialize a 2D vector to represent the grid of lights (all initially at brightness 0).
  std::vector<std::vector<int>> grid(1000, std::vector<int>(1000, 0));

  std::string line;
  while (std::getline(std::cin, line)) {
    // Parse the instruction line.
    std::string command;
    int x1, y1, x2, y2;

    size_t first_space = line.find(' ');

    if (line.substr(0, first_space) == "turn") {
      size_t second_space = line.find(' ', first_space + 1);
      command = line.substr(0, second_space);
      size_t through_pos = line.find("through");
      sscanf(line.substr(second_space + 1, through_pos - second_space - 1).c_str(), "%d,%d", &x1, &y1);
      sscanf(line.substr(through_pos + 8).c_str(), "%d,%d", &x2, &y2);

    } else {
      command = line.substr(0, first_space);
      size_t through_pos = line.find("through");
      sscanf(line.substr(first_space + 1, through_pos - first_space - 1).c_str(), "%d,%d", &x1, &y1);
      sscanf(line.substr(through_pos + 8).c_str(), "%d,%d", &x2, &y2);
    }

    // Apply the instruction to the specified range of lights.
    for (int i = x1; i <= x2; ++i) {
      for (int j = y1; j <= y2; ++j) {
        if (command == "turn on") {
          grid[i][j] += 1;
        } else if (command == "turn off") {
          grid[i][j] = std::max(0, grid[i][j] - 1);
        } else if (command == "toggle") {
          grid[i][j] += 2;
        }
      }
    }
  }

  // Calculate the total brightness of all lights.
  long long total_brightness = 0;
  for (const auto& row : grid) {
    total_brightness += std::accumulate(row.begin(), row.end(), 0LL);
  }

  std::cout << total_brightness << std::endl;

  return 0;
}
