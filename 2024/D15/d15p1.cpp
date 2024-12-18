#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

// Function to calculate GPS coordinates of boxes
int calculate_gps(const std::vector<std::string>& warehouse) {
    int total_gps = 0;
    for (size_t r = 0; r < warehouse.size(); ++r) {
        for (size_t c = 0; c < warehouse[r].size(); ++c) {
            if (warehouse[r][c] == 'O') {
                total_gps += static_cast<int>(100 * r + c);
            }
        }
    }
    return total_gps;
}

// Function to attempt to push a chain of boxes in a given direction
// Returns true if successful, false otherwise.
bool push_boxes(std::vector<std::string>& warehouse, int start_r, int start_c, int dr, int dc) {
    // Collect all boxes in a chain
    std::vector<std::pair<int,int>> boxes;
    int r = start_r;
    int c = start_c;

    while (true) {
        if (r < 0 || r >= (int)warehouse.size() || c < 0 || c >= (int)warehouse[0].size()) {
            // Out of bounds: can't push
            return false;
        }

        if (warehouse[r][c] == 'O') {
            // Keep going along the chain
            boxes.push_back({r, c});
            r += dr;
            c += dc;
        } else {
            // Found something that's not a box
            break;
        }
    }

    // Now (r,c) is the cell after the chain of boxes.
    // Check what this cell is:
    // We need it to be empty '.' to push the boxes into it.
    // If it's '#' or 'O', or out of bounds, we cannot push.
    if (r < 0 || r >= (int)warehouse.size() || c < 0 || c >= (int)warehouse[0].size()) {
        // Out of bounds
        return false;
    }

    if (warehouse[r][c] != '.') {
        // We cannot push if the next cell isn't empty
        return false;
    }

    // We can push. Move all boxes forward, starting from the last one
    // to avoid overwriting cells prematurely.
    // For the last box in the chain:
    warehouse[r][c] = 'O';
    for (int i = (int)boxes.size()-1; i > 0; i--) {
        auto prev_box = boxes[i-1];
        warehouse[boxes[i].first][boxes[i].second] = 'O'; // Move the i-th box into the previous box's spot
    }
    // The first box moves into the cell originally adjacent to the robot
    warehouse[boxes[0].first][boxes[0].second] = 'O';

    return true;
}

// Function to move the robot and boxes
void move_robot(std::vector<std::string>& warehouse, char direction) {
    int robot_row = -1, robot_col = -1;
    for (size_t r = 0; r < warehouse.size(); ++r) {
        for (size_t c = 0; c < warehouse[r].size(); ++c) {
            if (warehouse[r][c] == '@') {
                robot_row = static_cast<int>(r);
                robot_col = static_cast<int>(c);
                break;
            }
        }
        if (robot_row != -1) break;
    }

    int dr = 0, dc = 0;
    if (direction == '^') dr = -1;
    else if (direction == 'v') dr = 1;
    else if (direction == '<') dc = -1;
    else if (direction == '>') dc = 1;

    int new_robot_row = robot_row + dr;
    int new_robot_col = robot_col + dc;

    // Check if the new robot location is valid
    if (new_robot_row < 0 || new_robot_row >= (int)warehouse.size() ||
        new_robot_col < 0 || new_robot_col >= (int)warehouse[0].size()) {
        // Out of bounds: no move
        return;
    }

    // If we run into a wall, we can't move
    if (warehouse[new_robot_row][new_robot_col] == '#') {
        return;
    }

    // If the cell is empty, just move the robot
    if (warehouse[new_robot_row][new_robot_col] == '.') {
        warehouse[new_robot_row][new_robot_col] = '@';
        warehouse[robot_row][robot_col] = '.';
        return;
    }

    // If there's a box, we need to check for a chain of boxes
    if (warehouse[new_robot_row][new_robot_col] == 'O') {
        // Attempt to push the chain of boxes
        if (push_boxes(warehouse, new_robot_row, new_robot_col, dr, dc)) {
            // If successful, move the robot into the position previously occupied by the first box
            warehouse[new_robot_row][new_robot_col] = '@';
            warehouse[robot_row][robot_col] = '.';
        } else {
            // If we cannot push, do nothing
            return;
        }
    }
}

int main() {
    std::vector<std::string> warehouse;
    std::string line;
    
    // Read the warehouse layout
    while (std::getline(std::cin, line) && !line.empty() && line[0] == '#') {
        warehouse.push_back(line);
    }
    
    std::string moves;
    while (std::getline(std::cin, line)) {
        // Ignore empty lines or non-move lines if any
        moves += line;
    }

    // Process moves
    for (char move : moves) {
        move_robot(warehouse, move);
    }

    // Calculate and print the final GPS
    int final_gps = calculate_gps(warehouse);
    std::cout << final_gps << std::endl;

    return 0;
}
