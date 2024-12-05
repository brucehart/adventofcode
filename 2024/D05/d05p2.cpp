#include <iostream>
#include <sstream>
#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <algorithm> // For std::find_if and std::all_of
#include <cctype>    // For std::isspace

// Helper function to check if a line is empty or contains only whitespace
bool isLineEmpty(const std::string& line) {
    return std::all_of(line.begin(), line.end(), [](unsigned char c) { return std::isspace(c); });
}

// Function to order the page set
std::vector<int> orderPageSet(const std::vector<int>& ps, const std::unordered_map<int, std::set<int>>& pgCons) {
    std::vector<int> ord; // Ordered output

    // Step 1: Filter constraints to include only pages in the current page set
    std::unordered_map<int, std::set<int>> cons;
    std::unordered_map<int, std::set<int>> revCons; // Incoming edges

    for (const int pg : ps) {
        if (pgCons.find(pg) != pgCons.end()) {
            for (const int dep : pgCons.at(pg)) {
                if (std::find(ps.begin(), ps.end(), dep) != ps.end()) {
                    cons[pg].insert(dep);
                    revCons[dep].insert(pg);
                }
            }
        }
    }

    // Step 2: Identify pages with no incoming constraints
    std::set<int> noInc;
    for (const int pg : ps) {
        if (revCons.find(pg) == revCons.end() || revCons[pg].empty()) {
            noInc.insert(pg);
        }
    }

    // Step 3: Process pages with no incoming constraints
    while (!noInc.empty()) {
        int cur = *noInc.begin();
        noInc.erase(noInc.begin());

        ord.push_back(cur);

        if (cons.find(cur) != cons.end()) {
            for (const int dep : cons[cur]) {
                revCons[dep].erase(cur);
                if (revCons[dep].empty()) {
                    noInc.insert(dep);
                }
            }
            cons.erase(cur);
        }
    }

    // Step 4: Check for cycles or incomplete ordering
    if (ord.size() != ps.size()) {
        // Cycle detected or incomplete ordering
        return {}; // Indicate failure by returning an empty vector
    }

    return ord;
}

int main() {
    std::unordered_map<int, std::set<int>> pgCons; // Page constraints
    std::vector<std::vector<int>> psets; // Page sets

    std::string line;
    bool readingCons = true;

    // Read input from stdin
    while (std::getline(std::cin, line)) {
        if (isLineEmpty(line)) {
            if (readingCons) {
                readingCons = false; // Switch to reading page sets
            }
            continue; // Skip empty lines
        }

        if (readingCons) {
            // Parse constraints in the form X|Y
            std::stringstream ss(line);
            int key, val;
            char delim;

            if (ss >> key >> delim >> val && delim == '|') {
                pgCons[key].insert(val);
            } else {
                std::cerr << "Error parsing constraint line: " << line << std::endl;
            }
        } else {
            // Parse page sets separated by commas
            std::stringstream ss(line);
            std::vector<int> vals;
            std::string tok;

            while (std::getline(ss, tok, ',')) {
                // Trim whitespace
                tok.erase(tok.begin(), std::find_if(tok.begin(), tok.end(), [](unsigned char c) { return !std::isspace(c); }));
                tok.erase(std::find_if(tok.rbegin(), tok.rend(), [](unsigned char c) { return !std::isspace(c); }).base(), tok.end());

                if (!tok.empty()) {
                    try {
                        vals.push_back(std::stoi(tok));
                    } catch(const std::invalid_argument& e) {
                        std::cerr << "Invalid number '" << tok << "' in page set line: " << line << std::endl;
                    } catch(const std::out_of_range& e) {
                        std::cerr << "Number out of range '" << tok << "' in page set line: " << line << std::endl;
                    }
                }
            }

            if (!vals.empty()) {
                psets.push_back(vals);
            }
        }
    }

    // Ensure at least one page set is present
    if (psets.empty()) {
        std::cerr << "No page set provided in the input." << std::endl;
        return 1;
    }

    // Variables to track the sum of middle pages from correctly ordered updates
    long long midSum = 0; // To handle large sums

    for (const auto& ps : psets) {
        // Create a copy to avoid modifying the original
        std::vector<int> psCopy = ps;

        auto ord = orderPageSet(psCopy, pgCons);

        if (!ord.empty()) {
            bool isCorrect = (ord == ps);
            if (!isCorrect) {
                // Find the middle page
                int midIdx = ord.size() / 2;
                int midPg = ord[midIdx];
                midSum += midPg;
            }
        }
    }

    // Output the sum of middle pages from correctly ordered updates
    std::cout << midSum << std::endl;

    return 0;
}
