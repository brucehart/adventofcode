#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <vector>
#include <optional>

// Define the MonkeyMap class to handle the cube walking logic
class MonkeyMap {
public:
    // Constructor to initialize the map and related parameters
    MonkeyMap(const std::vector<std::string>& map_data) 
        : map(map_data),
          height(map_data.size()),
          width(map_data.empty() ? 0 : map_data[0].size()),
          facings(">v<^") 
    {
        face_size = find_face_size(height, width);
        face_map.resize(height / face_size, std::vector<std::optional<std::pair<char, std::string>>>(width / face_size, std::nullopt));
        build_face_map();
    }

    // Function to walk the map based on the path instructions with cube wrapping
    int walk_map(const std::vector<std::string>& path) {
        // Initialize starting position and facing direction
        int row = 0;
        int column = map[0].find('.');
        char facing = '>';

        for (const auto& p : path) {
            if (std::isdigit(p[0])) {
                int steps = std::stoi(p);
                for (int i = 0; i < steps; ++i) {
                    int new_row = row;
                    int new_col = column;
                    char new_facing = facing;

                    // Compute the next position and facing direction
                    std::tie(new_row, new_col, new_facing) = next_position_3d(row, column, facing);

                    // If the next cell is open, update the position and facing
                    if (map[new_row][new_col] == '.') {
                        row = new_row;
                        column = new_col;
                        facing = new_facing;
                    } else {
                        // Encountered a wall; stop moving in this direction
                        break;
                    }
                }
            } else {
                // Handle turning instructions
                if (p == "L") {
                    facing = rotate_left(facing);
                } else if (p == "R") {
                    facing = rotate_right(facing);
                }
            }
        }

        // Calculate the final password based on the final position and facing
        return 1000 * (row + 1) + 4 * (column + 1) + facings.find(facing);
    }

private:
    std::vector<std::string> map;
    int height;
    int width;
    int face_size;
    std::string facings;
    std::vector<std::vector<std::optional<std::pair<char, std::string>>>> face_map;

    // Determine the size of each face on the cube
    int find_face_size(int h, int w) const {
        // Possible face size candidates based on the problem constraints
        std::vector<std::pair<int, int>> candidates = { {2, 5}, {3, 4}, {4, 3}, {5, 2} };
        for (const auto& [x, y] : candidates) {
            // Removed exact divisibility checks to match Python logic
            if ((h / x) == (w / y)) {
                return h / x;
            }
        }
        // Fallback in case no suitable face size is found
        return 50; // Example default face size
    }

    // Build the mapping of faces on the cube
    void build_face_map() {
        // Define adjacency rules for each face direction
        const std::vector<std::pair<char, std::string>> adjacent = {
            {'u', "rflb"}, {'d', "rblf"},
            {'f', "rdlu"}, {'b', "ldru"},
            {'l', "fdbu"}, {'r', "bdfu"}
        };
        // Lambda to add sides recursively
        std::function<void(int, int)> add_sides = [&](int r_in, int c_in) {
            auto current = face_map[r_in][c_in];
            if (!current) return;
            char cf = current->first;
            const std::string& ce = current->second;

            // Directions: up, down, left, right
            std::vector<std::tuple<int, int, int, int>> directions = {
                {r_in - 1, c_in, 3, 1},
                {r_in + 1, c_in, 1, 3},
                {r_in, c_in - 1, 2, 0},
                {r_in, c_in + 1, 0, 2}
            };

            for (const auto& [r, c, d, e] : directions) {
                if (r >= 0 && r < static_cast<int>(face_map.size()) &&
                    c >= 0 && c < static_cast<int>(face_map[0].size()) &&
                    !face_map[r][c] &&
                    map[r * face_size][c * face_size] != ' ') {
                    
                    char nf = ce[d];
                    auto it = std::find_if(adjacent.begin(), adjacent.end(),
                                           [&](const std::pair<char, std::string>& pair) { return pair.first == nf; });
                    if (it == adjacent.end()) continue;
                    std::string ne = it->second;

                    // Rotate the adjacency string until the edge matches the current face
                    while (ne[e] != cf) {
                        std::rotate(ne.begin(), ne.begin() + 1, ne.end());
                    }

                    face_map[r][c] = std::make_pair(nf, ne);
                    add_sides(r, c);
                }
            }
        };

        // Find the first face and initialize the face_map
        int first_col = map[0].find_first_of(".#") / face_size;
        face_map[0][first_col] = std::make_pair('u', "rflb");
        add_sides(0, first_col);
    }

    // Calculate the next position and facing direction on the cube
    std::tuple<int, int, char> next_position_3d(int r, int c, char f) const {
        int nr = r;
        int nc = c;
        char nf = f;

        // Attempt to move in the current facing direction
        if (f == '>' && c + 1 < width && map[r][c + 1] != ' ') {
            nc += 1;
        }
        else if (f == '<' && c > 0 && map[r][c - 1] != ' ') {
            nc -= 1;
        }
        else if (f == 'v' && r + 1 < height && map[r + 1][c] != ' ') {
            nr += 1;
        }
        else if (f == '^' && r > 0 && map[r - 1][c] != ' ') {
            nr -= 1;
        }
        else {
            // Handle cube wrapping when moving off the edge
            int current_face_r = r / face_size;
            int current_face_c = c / face_size;
            auto current_face = face_map[current_face_r][current_face_c];
            if (!current_face) return { r, c, f }; // Invalid face

            char cf_name = current_face->first;
            const std::string& cf_edges = current_face->second;
            // Determine the new face based on the current facing direction
            size_t facing_index = facings.find(f);
            if (facing_index == std::string::npos) return { r, c, f };
            char nf_name = cf_edges[facing_index];

            // Find the new face coordinates
            auto new_face = find_face(nf_name);
            if (!new_face.has_value()) return { r, c, f };

            int nf_map_r = new_face->first;
            int nf_map_c = new_face->second;
            const auto& nf_edges = face_map[nf_map_r][nf_map_c]->second;

            // Determine the new facing direction based on edge transitions
            size_t edge_index = nf_edges.find(cf_name);
            if (edge_index == std::string::npos) return { r, c, f };
            char new_f = facings[(edge_index + 2) % facings.size()]; // Adjust facing based on edge

            // Calculate the new row and column based on the new facing
            nr = nf_map_r * face_size;
            nc = nf_map_c * face_size;

            if (new_f == '^') {
                nr += face_size - 1;
            }
            else if (new_f == '<') {
                nc += face_size - 1;
            }

            // Adjust position within the face based on the original position
            if ((f == '>' && new_f == 'v') || (f == '<' && new_f == '^')) {
                nc += (face_size - 1 - (r % face_size));
            }
            else if ((f == 'v' && new_f == '<') || (f == '^' && new_f == '>')) {
                nr += (c % face_size);
            }
            else if ((f == '>' && new_f == '^') || (f == '<' && new_f == 'v')) {
                nc += (r % face_size);
            }
            else if ((f == 'v' && new_f == '>') || (f == '^' && new_f == '<')) {
                nr += (face_size - 1 - (c % face_size));
            }
            else if ((f == '>' && new_f == '<') || (f == '<' && new_f == '>')) {
                nr += (face_size - 1 - (r % face_size));
            }
            else if ((f == 'v' && new_f == '^') || (f == '^' && new_f == 'v')) {
                nc += (face_size - 1 - (c % face_size));
            }
            else if ((f == '<' && new_f == '<') || (f == '>' && new_f == '>')) {
                nr += (r % face_size);
            }
            else if ((f == '^' && new_f == '^') || (f == 'v' && new_f == 'v')) {
                nc += (c % face_size);
            }

            nf = new_f;
        }

        return { nr, nc, nf };
    }

    // Find the face coordinates based on the face name
    std::optional<std::pair<int, int>> find_face(char target_face) const {
        for (size_t i = 0; i < face_map.size(); ++i) {
            for (size_t j = 0; j < face_map[i].size(); ++j) {
                if (face_map[i][j] && face_map[i][j]->first == target_face) {
                    return std::make_pair(i, j);
                }
            }
        }
        return std::nullopt;
    }

    // Rotate the facing direction to the left
    char rotate_left(char f) const {
        size_t pos = facings.find(f);
        return facings[(pos + 3) % 4];
    }

    // Rotate the facing direction to the right
    char rotate_right(char f) const {
        size_t pos = facings.find(f);
        return facings[(pos + 1) % 4];
    }
};

int main() {
    std::vector<std::string> map_data;
    std::string line;

    // Read map data lines until an empty line is encountered
    while (std::getline(std::cin, line) && !line.empty()) {
        map_data.push_back(line);
    }

    // Determine the maximum width of the map for padding
    size_t max_width = 0;
    for (const auto& m : map_data) {
        if (m.size() > max_width) {
            max_width = m.size();
        }
    }

    // Pad each map line to the maximum width with spaces
    for (auto& m : map_data) {
        if (m.size() < max_width) {
            m.append(max_width - m.size(), ' ');
        }
    }

    // Read the path data line
    if (!std::getline(std::cin, line)) {
        // If no path data is provided, exit
        return 0;
    }

    // Use regex to parse the path into numbers and 'L'/'R'
    std::regex path_regex(R"((\d+|[LR]))");
    std::sregex_iterator iter(line.begin(), line.end(), path_regex);
    std::sregex_iterator end;
    std::vector<std::string> path_data;
    for (; iter != end; ++iter) {
        path_data.push_back(iter->str());
    }

    // Initialize the MonkeyMap with the map data
    MonkeyMap monkey_map(map_data);

    // Compute and output the result for Part 2 (cube walking)
    int result = monkey_map.walk_map(path_data);
    std::cout << result << std::endl;

    return 0;
}
