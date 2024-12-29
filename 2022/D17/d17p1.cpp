#include <bits/stdc++.h>

int main() {
    // Read the jet pattern from standard input
    std::string jet_pattern;
    std::cin >> jet_pattern;
    size_t jet_length = jet_pattern.length();
    size_t jet_index = 0; // Current index in the jet pattern

    // Define the five rock shapes using relative (x, y) coordinates
    const std::vector<std::vector<std::pair<int, int>>> rock_shapes = {
        // Rock 1: Horizontal Line
        { {0,0}, {1,0}, {2,0}, {3,0} },
        // Rock 2: Plus Shape
        { {1,0}, {0,1}, {1,1}, {2,1}, {1,2} },
        // Rock 3: L Shape
        { {0,0}, {1,0}, {2,0}, {2,1}, {2,2} },
        // Rock 4: Vertical Line
        { {0,0}, {0,1}, {0,2}, {0,3} },
        // Rock 5: Square
        { {0,0}, {1,0}, {0,1}, {1,1} }
    };

    // Initialize the chamber with the floor at y = 0
    // Each row is represented by a 7-bit integer (bits 0 to 6 correspond to x = 0 to x = 6)
    std::vector<uint8_t> chamber;
    chamber.push_back(0b1111111); // Floor

    int highest_y = 0; // Tracks the highest occupied y-coordinate

    const int total_rocks = 2022; // Number of rocks to simulate

    // Simulate each rock falling
    for(int rock_num = 0; rock_num < total_rocks; ++rock_num) {
        // Determine the current rock shape
        const auto& shape = rock_shapes[rock_num % rock_shapes.size()];

        // Initial position: 2 units from the left wall and 3 units above the highest rock
        int rock_x = 2;
        int rock_y = highest_y + 4;

        // Ensure the chamber has enough rows to accommodate the new rock
        while(static_cast<int>(chamber.size()) <= rock_y + 4) {
            chamber.push_back(0);
        }

        bool settled = false; // Flag to indicate if the rock has come to rest

        while(!settled) {
            // ----- Jet Push Phase -----
            char jet = jet_pattern[jet_index % jet_length];
            jet_index++; // Move to the next jet direction for the subsequent push

            // Determine the horizontal movement based on the jet direction
            int delta_x = (jet == '<') ? -1 : 1;

            bool can_move_horizontally = true;
            // Check each block of the rock for possible horizontal movement
            for(const auto& [dx, dy] : shape) {
                int new_x = rock_x + dx + delta_x;
                int new_y = rock_y + dy;
                // Check boundaries
                if(new_x < 0 || new_x >= 7) {
                    can_move_horizontally = false;
                    break;
                }
                // Check for collision with existing rocks
                if(new_y < chamber.size() && (chamber[new_y] & (1 << new_x))) {
                    can_move_horizontally = false;
                    break;
                }
            }

            // Apply horizontal movement if possible
            if(can_move_horizontally) {
                rock_x += delta_x;
            }

            // ----- Fall Down Phase -----
            bool can_move_down = true;
            for(const auto& [dx, dy] : shape) {
                int new_x = rock_x + dx;
                int new_y = rock_y + dy - 1; // Move down by one unit
                if(new_y < 0) {
                    can_move_down = false;
                    break;
                }
                if(new_y < chamber.size() && (chamber[new_y] & (1 << new_x))) {
                    can_move_down = false;
                    break;
                }
            }

            if(can_move_down) {
                rock_y -= 1; // Move the rock down by one unit
            } else {
                // Rock has come to rest; update the chamber
                for(const auto& [dx, dy] : shape) {
                    int final_x = rock_x + dx;
                    int final_y = rock_y + dy;
                    // Ensure the chamber has enough rows
                    while(final_y >= static_cast<int>(chamber.size())) {
                        chamber.push_back(0);
                    }
                    chamber[final_y] |= (1 << final_x); // Mark the position as occupied
                    // Update the highest y-coordinate if necessary
                    highest_y = std::max(highest_y, final_y);
                }
                settled = true; // Proceed to the next rock
            }
        }
    }

    // Output the final height of the tower
    std::cout << highest_y << std::endl;

    return 0;
}

