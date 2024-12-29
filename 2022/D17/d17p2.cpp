#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
#include <string>
#include <tuple>


static constexpr std::size_t WIDTH = 7;
static constexpr std::size_t TARGET = 1000000000000ULL; // 1 trillion

// A simple coordinate for placing pieces.
struct Coord {
    std::size_t x;
    std::size_t y;
};

// Jet directions
enum class Jet { Left, Right };

// A piece is just a list of (x, y) offsets relative to its bottom-left corner.
using Piece = std::vector<Coord>;

// Define our 5 shapes (similar to the puzzle). We'll define them from
// bottom-left so that the (0,0) offset is the piece's bottom-left cell.
static const std::vector<Piece> PIECES = {
    // Shape 1: Horizontal line of length 4
    { {0,0}, {1,0}, {2,0}, {3,0} },
    // Shape 2: Plus shape
    { {1,0}, {0,1}, {1,1}, {2,1}, {1,2} },
    // Shape 3: L-shape mirrored
    { {0,0}, {1,0}, {2,0}, {2,1}, {2,2} },
    // Shape 4: Vertical line of length 4
    { {0,0}, {0,1}, {0,2}, {0,3} },
    // Shape 5: 2x2 square
    { {0,0}, {1,0}, {0,1}, {1,1} }
};

// We’ll parse the input into a vector of Jet directions.
static std::vector<Jet> parseJets(const std::string &input) {
    std::vector<Jet> jets;
    jets.reserve(input.size());
    for (char c : input) {
        if (c == '<') {
            jets.push_back(Jet::Left);
        } else { // c == '>'
            jets.push_back(Jet::Right);
        }
    }
    return jets;
}

// A small helper to hash a pair of (size_t, size_t) so we can store
// it as the key in an unordered_map.
struct PairHash {
    std::size_t operator()(const std::pair<std::size_t,std::size_t>& p) const {
        // A common 64-bit mix: 
        // (p.first << 32) ^ p.second or a variation. 
        // For small puzzle inputs, even simpler is fine.
        auto h1 = std::hash<std::size_t>()(p.first);
        auto h2 = std::hash<std::size_t>()(p.second);
        // Some XOR + shift
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

// We’ll maintain a State struct, similar to the Rust code:
struct State {
    // Indices
    std::size_t jet_count = 0;    // which jet we are on
    std::size_t piece_count = 0;  // how many pieces have settled

    // Tower info
    std::size_t top = 0;          // topmost row (# of rows in the tower)
    std::vector<std::array<bool, WIDTH>> map; // map[y][x] = true if occupied

    // Current piece’s position (bottom-left corner)
    Coord curr{0,0};

    // Cycle detection
    // Key = (piece_idx, jet_idx)
    // Value = (count_of_times_seen, piece_count_when_seen, top_when_seen)
    std::unordered_map<std::pair<std::size_t,std::size_t>,
                       std::tuple<std::size_t,std::size_t,std::size_t>,
                       PairHash> seen;

    // Once we detect a cycle and skip forward, we track how many total
    // rows we “magically” added to the final height.
    std::size_t added_by_repeats = 0;

    // Check if placing the *piece* with bottom-left corner at *candidate*
    // is valid (i.e., does not collide with existing blocks and is within bounds).
    bool is_valid(const Coord &candidate, const Piece &piece) const {
        for (const auto &offset : piece) {
            std::size_t x = candidate.x + offset.x;
            std::size_t y = candidate.y + offset.y;
            if (x >= WIDTH) {
                return false; // out of horizontal bounds
            }
            // We can handle "floor check" by seeing if y < 0, but we store as size_t:
            // so if y would be 0 and we subtract, it could wrap. We handle that above in logic.
            // Here, just check if y is within the map and not colliding:
            if (y < map.size()) {
                if (map[y][x]) {
                    return false; // collision
                }
            }
        }
        return true;
    }
};

// The main function that does part 2 logic. Returns final tower height.
std::size_t solvePart2(const std::string &input) {
    std::vector<Jet> jets = parseJets(input);

    State state;
    state.map.reserve(200000); // to reduce re-allocations

    while (state.piece_count < TARGET) {
        // 1) Determine which piece we are about to drop.
        std::size_t piece_idx = state.piece_count % PIECES.size();
        const Piece &piece = PIECES[piece_idx];

        // 2) Position the new piece so that its bottom-left corner is
        //    (2, state.top + 3).
        state.curr.x = 2;
        state.curr.y = state.top + 3; // If top=10, piece bottom is at y=10+3=13.

        // 3) Let the piece fall until it stops.
        while (true) {
            // 3a) Apply jet push
            std::size_t j = state.jet_count % jets.size();
            Jet jet = jets[j];
            state.jet_count++;

            Coord candidate = state.curr;
            if (jet == Jet::Left) {
                if (candidate.x > 0) {
                    candidate.x -= 1;
                }
            } else {
                candidate.x += 1;
            }
            // Check valid
            if (state.is_valid(candidate, piece)) {
                state.curr = candidate;
            }

            // 3b) Move down one unit (if possible)
            if (state.curr.y == 0) {
                // Already at floor. Stop.
                break;
            }
            Coord down = { state.curr.x, state.curr.y - 1 };
            if (!state.is_valid(down, piece)) {
                // Collides if we go down; so we stop.
                break;
            }
            // Otherwise, we can fall one more unit
            state.curr = down;
        }

        // 4) The piece has landed. Mark its cells as occupied:
        for (const auto &offset : piece) {
            std::size_t x = state.curr.x + offset.x;
            std::size_t y = state.curr.y + offset.y;
            // Expand the map if necessary.
            while (y >= state.map.size()) {
                state.map.push_back({false,false,false,false,false,false,false});
            }
            state.map[y][x] = true;
            // Update top. 
            // "y+1" because if a piece occupies row y=12, the top is at least 13.
            if ((y+1) > state.top) {
                state.top = y + 1;
            }
        }

        // 5) Now we can do cycle detection.
        //    We only attempt to skip forward if we have not done so before
        //    (`added_by_repeats == 0`).
        //    The key is (piece_idx, jet_idx mod jets.size()) for the *next* piece.
        if (state.added_by_repeats == 0) {
            // The key is: which piece index *would* drop next, and what is the current jet index?
            std::size_t next_piece_idx = (state.piece_count + 1) % PIECES.size();
            std::size_t next_jet_idx   = state.jet_count % jets.size();
            std::pair<std::size_t,std::size_t> key(next_piece_idx, next_jet_idx);

            // Check if we've seen this key before:
            auto it = state.seen.find(key);
            if (it != state.seen.end()) {
                // (count_times, old_piece_count, old_top)
                auto & [count_times, old_pc, old_top] = it->second;

                // If this is the *third* time we see it (count_times == 2),
                // that means we can skip forward. The first time is count_times=1,
                // second time is count_times=2, so we skip at the third time.
                if (count_times == 2) {
                    // We have a cycle. 
                    // Delta in piece_count since last time.
                    std::size_t delta_pc = state.piece_count - old_pc;
                    // Delta in height
                    std::size_t delta_top = state.top - old_top;

                    // How many full cycles can we jump?
                    std::size_t remaining = TARGET - state.piece_count;
                    std::size_t repeats = remaining / delta_pc; 

                    // Jump
                    state.added_by_repeats = repeats * delta_top;
                    state.piece_count += repeats * delta_pc;
                }

                // Whether we skip or not, we update the record to reflect we've seen it again:
                count_times += 1;
                old_pc = state.piece_count;
                old_top = state.top;
            } else {
                // Not in map => first occurrence
                state.seen.insert({ key, {1, state.piece_count, state.top} });
            }
        }

        // 6) Finally, increment the piece count for the just-landed piece
        state.piece_count += 1;
        // If we've reached target, we’re done
        if (state.piece_count >= TARGET) {
            break;
        }
    }

    return state.top + state.added_by_repeats;
}

// Main reads the jet pattern from stdin, solves Part 2, and prints the result.
int main() {
    std::string input;
    std::cin >> input;

    std::size_t result = solvePart2(input);
    std::cout << result << std::endl;
    return 0;
}
