#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main() {
  // Initialize a 2D vector to represent the grid of lights (all initially off).
  std::vector<std::vector<bool>> grid(1000, std::vector<bool>(1000, false));

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
      sscanf(line.substr(second_space + 1, through_pos - second_space -1).c_str(), "%d,%d", &x1, &y1);
      sscanf(line.substr(through_pos + 8).c_str(), "%d,%d", &x2, &y2);

    } else {
      command = line.substr(0, first_space);
      size_t through_pos = line.find("through");
      sscanf(line.substr(first_space + 1, through_pos - first_space -1).c_str(), "%d,%d", &x1, &y1);
      sscanf(line.substr(through_pos + 8).c_str(), "%d,%d", &x2, &y2);
    }
    

    // Apply the instruction to the specified range of lights.
    for (int i = x1; i <= x2; ++i) {
      for (int j = y1; j <= y2; ++j) {
        if (command == "turn on") {
          grid[i][j] = true;
        } else if (command == "turn off") {
          grid[i][j] = false;
        } else if (command == "toggle") {
          grid[i][j] = !grid[i][j];
        }
      }
    }
  }

  // Count the number of lights that are turned on.
  int count = 0;
  for(const auto& row : grid) {
    count += std::count(row.begin(), row.end(), true);
  }

  std::cout << count << std::endl;

  return 0;
}
