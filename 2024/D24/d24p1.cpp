#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <optional>
#include <algorithm>
#include <cctype>

// Helper to trim whitespace from both ends of a string
static void trim(std::string &s) {
    // Left trim
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        [](unsigned char c) { return !std::isspace(c); }));
    // Right trim
    s.erase(std::find_if(s.rbegin(), s.rend(),
        [](unsigned char c) { return !std::isspace(c); }).base(),
        s.end());
}

// We'll store each gate as a small struct for clarity.
struct Gate {
    std::string input1;
    std::string input2;
    std::string op;       // "AND", "OR", or "XOR"
    std::string output;
};

int main() {
    // Maps wireName -> optional<bool> value.
    // If std::optional is not set, that means we haven't determined that wire's value yet.
    std::unordered_map<std::string, std::optional<bool>> wireValues;

    // For each wire, keep a list of gates that depend on that wire as input.
    // We'll do a BFS-like simulation once we know a wire's value.
    std::unordered_map<std::string, std::vector<Gate>> wireToGates;

    // Keep a list of all gates so we can store them easily and also
    // fill wireToGates structures.
    std::vector<Gate> gates;

    // Read all lines from stdin until EOF.
    std::string line;
    while (true) {
        if(!std::getline(std::cin, line)) {
            break; // EOF
        }
        // Trim whitespace
        trim(line);
        if(line.empty()) {
            continue;
        }

        // Check if this line describes a wire initialization (wireName: 0/1)
        // or a gate (wireName OP wireName -> wireName).
        if (line.find(':') != std::string::npos) {
            // It's a wire initialization of the form "x00: 1"
            auto pos = line.find(':');
            std::string wireName = line.substr(0, pos);
            std::string valueStr = line.substr(pos + 1);

            trim(wireName);
            trim(valueStr);

            // Convert the value to bool
            bool val = (std::stoi(valueStr) != 0);
            wireValues[wireName] = val;
        } 
        else {
            // It's a gate line of the form "x00 AND y00 -> z00"
            std::stringstream ss(line);
            Gate g;
            std::string arrow;
            ss >> g.input1 >> g.op >> g.input2 >> arrow >> g.output;
            gates.push_back(g);
        }
    }

    // Build adjacency: which gates depend on each wire?
    for(const auto &g : gates) {
        wireToGates[g.input1].push_back(g);
        wireToGates[g.input2].push_back(g);
    }

    // We'll do a BFS-like simulation:
    // Start with a queue of all wires that have known values.
    // Whenever we discover a gate whose both inputs are known, compute the output
    // and if that output wasn't already known, set it and enqueue the output wire.
    std::queue<std::string> q;

    // Initialize queue with known wires
    for (const auto &kv : wireValues) {
        if (kv.second.has_value()) {
            q.push(kv.first);
        }
    }

    // Process known wires until no more outputs can be resolved
    while(!q.empty()) {
        std::string w = q.front();
        q.pop();

        // For each gate that depends on w, check if both inputs are known
        if (!wireToGates.count(w)) {
            continue;
        }

        const auto &dependentGates = wireToGates[w];
        for(const auto &gate : dependentGates) {
            // Check if gate's inputs are both known
            auto itVal1 = wireValues.find(gate.input1);
            auto itVal2 = wireValues.find(gate.input2);

            if (itVal1 == wireValues.end() || !itVal1->second.has_value()) {
                continue; // first input not known yet
            }
            if (itVal2 == wireValues.end() || !itVal2->second.has_value()) {
                continue; // second input not known yet
            }

            // Both inputs are known, compute the output
            bool v1 = itVal1->second.value();
            bool v2 = itVal2->second.value();
            bool outVal = false;

            if (gate.op == "AND") {
                outVal = (v1 && v2);
            } else if (gate.op == "OR") {
                outVal = (v1 || v2);
            } else if (gate.op == "XOR") {
                outVal = (v1 ^ v2);
            }

            // If we haven't already determined gate.output, set it
            auto &outOpt = wireValues[gate.output];
            if (!outOpt.has_value()) {
                outOpt = outVal;
                q.push(gate.output);
            }
        }
    }

    // Gather all wires that start with 'z'. 
    // The puzzle states: "z00 is the least significant bit, z01 is next, and so on."
    // We'll store them in a vector of (wireName, index), then sort by index.
    std::vector<std::pair<std::string, int>> zWires;
    for(const auto &kv : wireValues) {
        const std::string &wireName = kv.first;
        if(wireName.size() >= 2 && wireName[0] == 'z') {
            // parse the substring after 'z' as an integer
            std::string idxStr = wireName.substr(1); 
            int idx = std::stoi(idxStr);
            zWires.push_back({ wireName, idx });
        }
    }

    // Sort by the integer index so z00 < z01 < z02 ...
    std::sort(zWires.begin(), zWires.end(), 
        [](auto &a, auto &b){
            return a.second < b.second;
        });

    // Build the binary number, LSB = z00, next = z01, etc.
    long long result = 0;
    long long place = 1;

    for(const auto &zw : zWires) {
        bool bitVal = false;
        // If a z-wire didn't get a value, treat it as 0
        if(wireValues[zw.first].has_value()) {
            bitVal = wireValues[zw.first].value();
        }
        if (bitVal) {
            result += place;
        }
        place <<= 1; // go to next bit
    }

    // Output the result in decimal, followed by a newline.
    std::cout << result << std::endl;
    return 0;
}
