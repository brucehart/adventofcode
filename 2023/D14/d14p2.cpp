#include <iostream>
#include <vector>
#include <string>
#include <map>

// Function to tilt the platform in a given direction
void tilt(std::vector<std::string>& platform, int dir_row, int dir_col) {
    int rows = platform.size();
    int cols = platform[0].size();
    
    if (dir_row == -1) { // North
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
    } else if (dir_row == 1) { // South
        for (int j = 0; j < cols; ++j) {
            int next_available_row = rows - 1;
            for (int i = rows - 1; i >= 0; --i) {
                if (platform[i][j] == 'O') {
                    if (next_available_row != i) {
                        platform[next_available_row][j] = 'O';
                        platform[i][j] = '.';
                    }
                    next_available_row--;
                } else if (platform[i][j] == '#') {
                    next_available_row = i - 1;
                }
            }
        }
    } else if (dir_col == -1) { // West
        for (int i = 0; i < rows; ++i) {
            int next_available_col = 0;
            for (int j = 0; j < cols; ++j) {
                if (platform[i][j] == 'O') {
                    if (next_available_col != j) {
                        platform[i][next_available_col] = 'O';
                        platform[i][j] = '.';
                    }
                    next_available_col++;
                } else if (platform[i][j] == '#') {
                    next_available_col = j + 1;
                }
            }
        }
    } else if (dir_col == 1) { // East
        for (int i = 0; i < rows; ++i) {
            int next_available_col = cols - 1;
            for (int j = cols - 1; j >= 0; --j) {
                if (platform[i][j] == 'O') {
                    if (next_available_col != j) {
                        platform[i][next_available_col] = 'O';
                        platform[i][j] = '.';
                    }
                    next_available_col--;
                } else if (platform[i][j] == '#') {
                    next_available_col = j - 1;
                }
            }
        }
    }
}

// Function to perform one spin cycle
void spin_cycle(std::vector<std::string>& platform) {
    tilt(platform, -1, 0); // North
    tilt(platform, 0, -1); // West
    tilt(platform, 1, 0);  // South
    tilt(platform, 0, 1);  // East
}

// Function to calculate the total load on the north support beams
long long calculate_load(const std::vector<std::string>& platform) {
    long long total_load = 0;
    int rows = platform.size();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < platform[0].size(); ++j) {
            if (platform[i][j] == 'O') {
                total_load += (rows - i);
            }
        }
    }
    return total_load;
}

int main() {
    std::vector<std::string> platform;
    std::string line;

    // Read the platform from stdin
    while (std::getline(std::cin, line)) {
        platform.push_back(line);
    }

    std::map<std::vector<std::string>, int> seen_platforms;
    std::vector<std::vector<std::string>> platform_history;

    long long cycle_count = 0;
    while (cycle_count < 1000000000) {
        if (seen_platforms.count(platform)) {
            int loop_start = seen_platforms[platform];
            int loop_length = cycle_count - loop_start;
            int remaining_cycles = 1000000000 - cycle_count;
            int cycles_after_loop = remaining_cycles % loop_length;
            
            platform = platform_history[loop_start + cycles_after_loop];
            break;
        } else {
            seen_platforms[platform] = cycle_count;
            platform_history.push_back(platform);
            spin_cycle(platform);
            cycle_count++;
        }
    }

    // Output the total load
    std::cout << calculate_load(platform) << std::endl;

    return 0;
}